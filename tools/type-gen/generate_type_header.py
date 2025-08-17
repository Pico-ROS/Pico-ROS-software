#!/usr/bin/env python3
"""
This script generates pico-ROS macro header file from ROS2 interface definitions.
"""

import argparse
import json
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import Dict, List, Optional, Set, Tuple


def setup_python_path():
    """Set up PYTHONPATH to include necessary rosidl packages."""
    script_dir = Path(__file__).parent.absolute()
    rosidl_dir = script_dir / 'rosidl'

    # Add paths to PYTHONPATH
    paths_to_add = [
        str(script_dir),  # For our mock ament_index_python
        str(rosidl_dir / 'rosidl_generator_type_description'),
        str(rosidl_dir / 'rosidl_cli'),
        str(rosidl_dir / 'rosidl_parser'),
        str(rosidl_dir / 'rosidl_adapter'),
        str(rosidl_dir / 'rosidl_pycommon'),
    ]

    current_pythonpath = os.environ.get('PYTHONPATH', '')
    if current_pythonpath:
        paths_to_add.append(current_pythonpath)

    os.environ['PYTHONPATH'] = ':'.join(paths_to_add)


def discover_packages(parent_dir: str, interface_packages_only: bool = True) -> List[str]:
    """
    Recursively discover ROS packages in a directory by looking for package.xml files.

    :param parent_dir: Directory to search for ROS packages
    :param interface_packages_only: If True, only return packages with interface files
    :return: List of package directory paths
    """
    parent_path = Path(parent_dir).absolute()
    if not parent_path.exists():
        return []

    packages = []

    # Search for package.xml files recursively
    for package_xml in parent_path.rglob('package.xml'):
        package_dir = package_xml.parent

        if interface_packages_only:
            # Only include packages with interface files
            pkg_info = parse_package_xml(str(package_dir))
            if pkg_info['has_interfaces']:
                packages.append(str(package_dir))
        else:
            # Include all packages with package.xml
            packages.append(str(package_dir))

    return sorted(packages)


def expand_include_paths(include_paths: List[str]) -> List[str]:
    """
    Expand include paths to discover all packages with interface files.

    :param include_paths: List of paths to search for packages
    :return: List of individual package directory paths with interface files
    """
    expanded = []

    for include_path in include_paths:
        discovered_packages = discover_packages(include_path, interface_packages_only=True)
        expanded.extend(discovered_packages)

    # Remove duplicates while preserving order
    seen = set()
    result = []
    for path in expanded:
        if path not in seen:
            seen.add(path)
            result.append(path)

    return result




def parse_package_xml(package_dir: str) -> dict:
    """
    Parse package.xml to extract package information and dependencies.

    :param package_dir: Path to package directory
    :return: Dictionary with package name, dependencies, and interface status
    """
    package_xml_path = Path(package_dir) / 'package.xml'
    result = {
        'name': Path(package_dir).name,  # fallback to directory name
        'dependencies': [],
        'has_interfaces': False
    }

    if not package_xml_path.exists():
        return result

    try:
        import xml.etree.ElementTree as ET
        tree = ET.parse(package_xml_path)
        root = tree.getroot()

        # Extract package name
        name_elem = root.find('name')
        if name_elem is not None and name_elem.text:
            result['name'] = name_elem.text.strip()

        # Extract dependencies (only interface-relevant ones)
        interface_deps = set()
        for dep_type in ['depend', 'build_depend', 'exec_depend', 'build_export_depend']:
            for dep in root.findall(dep_type):
                if dep.text and dep.text.strip():
                    dep_name = dep.text.strip()
                    # print(dep_name)
                    # Filter for interface packages (exclude build tools, etc.)
                    if not dep_name.startswith('ament_') and not dep_name.startswith('rosidl_') and dep_name not in ['cmake', 'python3-pytest']:
                        interface_deps.add(dep_name)
                    if dep_name == "rosidl_default_runtime":
                        interface_deps.add("action_msgs")
                        interface_deps.add("service_msgs")
                        interface_deps.add("unique_identifier_msgs")


        result['dependencies'] = list(interface_deps)

        # Check if this package has interface files
        package_path = Path(package_dir)
        result['has_interfaces'] = any(
            (package_path / subdir).exists() and list((package_path / subdir).glob('*.' + ext))
            for subdir in ['msg', 'srv', 'action']
            for ext in ['msg', 'srv', 'action', 'idl']
        )

    except Exception:
        pass  # Fall back to defaults

    return result


def topological_sort_packages(package_dirs: List[str]) -> List[str]:
    """
    Sort packages using topological sorting based on their dependencies.

    :param package_dirs: List of package directory paths
    :return: Topologically sorted list of package directory paths
    """
    # Parse all packages and build dependency graph
    packages = {}
    name_to_dir = {}

    for pkg_dir in package_dirs:
        pkg_info = parse_package_xml(pkg_dir)
        pkg_name = pkg_info['name']
        packages[pkg_name] = pkg_info
        name_to_dir[pkg_name] = pkg_dir

    # Build dependency graph (only include deps that exist in our package set)
    graph = {}
    in_degree = {}

    for pkg_name in packages:
        graph[pkg_name] = []
        in_degree[pkg_name] = 0

    for pkg_name, pkg_info in packages.items():
        for dep_name in pkg_info['dependencies']:
            if dep_name in packages:  # Only dependencies within our package set
                graph[dep_name].append(pkg_name)  # dep_name -> pkg_name
                in_degree[pkg_name] += 1

    # Topological sort using Kahn's algorithm
    queue = [pkg for pkg in packages if in_degree[pkg] == 0]
    sorted_packages = []

    while queue:
        # Sort the queue to ensure deterministic ordering
        queue.sort()
        current = queue.pop(0)
        sorted_packages.append(current)

        # Update in-degrees of dependent packages
        for dependent in graph[current]:
            in_degree[dependent] -= 1
            if in_degree[dependent] == 0:
                queue.append(dependent)

    # Check for circular dependencies
    if len(sorted_packages) != len(packages):
        remaining = [pkg for pkg in packages if pkg not in sorted_packages]
        print(f"Warning: Circular dependencies detected in packages: {remaining}", file=sys.stderr)
        # Add remaining packages in alphabetical order as fallback
        sorted_packages.extend(sorted(remaining))

    # Convert package names back to directory paths
    return [name_to_dir[pkg_name] for pkg_name in sorted_packages]


def process_dependencies(discovered_packages: List[str], shared_output_dir: str) -> None:
    """
    Automatically generate type descriptions for all discovered dependency packages.
    Processes packages in dependency order to avoid circular dependency issues.

    :param discovered_packages: List of package directory paths
    :param shared_output_dir: Directory where dependency .json files will be placed
    """
    if not discovered_packages:
        return

    print(f"Auto-generating type descriptions for {len(discovered_packages)} dependency package(s)...")

    shared_output_path = Path(shared_output_dir)
    shared_output_path.mkdir(parents=True, exist_ok=True)

    # Sort packages using topological sorting to process dependencies first
    sorted_packages = topological_sort_packages(discovered_packages)

    print(f"  Processing packages in dependency order...")

    for pkg_dir in sorted_packages:
        pkg_info = parse_package_xml(pkg_dir)
        pkg_name = pkg_info['name']

        print(f"  Processing dependency: {pkg_name}")

        try:
            # Create a persistent directory for this package's IDL conversion
            pkg_temp_dir = os.path.join(shared_output_dir, f"_temp_{pkg_name}_idl")
            os.makedirs(pkg_temp_dir, exist_ok=True)

            # Convert this package's .msg/.srv files to .idl
            idl_files = process_package_directory(pkg_dir, pkg_name, pkg_temp_dir)

            if idl_files:
                # Use only previously processed packages as include paths
                processed_include_paths = []
                for already_processed in sorted_packages:
                    if already_processed == pkg_dir:
                        break  # Don't include packages after this one
                    processed_include_paths.append(already_processed)

                dep_generator_args_file = create_generator_arguments_file(
                    pkg_name,
                    idl_files,
                    processed_include_paths,  # Only previously processed packages
                    shared_output_dir  # All dependencies go to shared output dir
                )

                # Run the generator for this dependency
                script_dir = Path(__file__).parent.absolute()
                generator_bin = script_dir / 'rosidl' / 'rosidl_generator_type_description' / 'bin' / 'rosidl_generator_type_description'

                result = subprocess.run([
                    sys.executable,
                    str(generator_bin),
                    '--generator-arguments-file',
                    dep_generator_args_file
                ], capture_output=True, text=True)

                if result.returncode != 0:
                    print(f"    Error: Failed to generate types for {pkg_name}: {result.stderr}")
                    exit(1)
                print(f"    Generated type descriptions for {pkg_name}")

                # Clean up temporary generator args file
                try:
                    os.unlink(dep_generator_args_file)
                except FileNotFoundError:
                    pass

                # Clean up temporary IDL directory after successful generation
                try:
                    import shutil
                    shutil.rmtree(pkg_temp_dir)
                except Exception:
                    pass  # Don't fail if cleanup fails

        except Exception as e:
            print(f"    Warning: Error processing dependency {pkg_name}: {e}")


def process_package_directory(package_dir: str, package_name: str, temp_dir: str) -> List[str]:
    """Process a package directory to convert all .msg and .srv files to .idl format."""
    # Import here to avoid import issues during setup
    sys.path.insert(0, str(Path(__file__).parent / 'rosidl' / 'rosidl_adapter'))
    from rosidl_adapter.msg import convert_msg_to_idl as ros_convert_msg_to_idl
    from rosidl_adapter.srv import convert_srv_to_idl as ros_convert_srv_to_idl

    package_path = Path(package_dir).absolute()
    output_path = Path(temp_dir)
    output_path.mkdir(parents=True, exist_ok=True)

    if not package_path.exists():
        raise RuntimeError(f"Package directory does not exist: {package_path}")

    converted_files = []

    # Process .msg files
    msg_dir = package_path / 'msg'
    if msg_dir.exists():
        for msg_file in msg_dir.glob('*.msg'):
            relative_input = Path('msg') / msg_file.name
            try:
                output_file = ros_convert_msg_to_idl(
                    package_dir=package_path,
                    package_name=package_name,
                    input_file=relative_input,
                    output_dir=output_path
                )
                converted_files.append(str(output_file.absolute()))
            except Exception as e:
                print(f"Warning: Failed to convert {msg_file}: {e}", file=sys.stderr)

    # Process .srv files
    srv_dir = package_path / 'srv'
    if srv_dir.exists():
        for srv_file in srv_dir.glob('*.srv'):
            relative_input = Path('srv') / srv_file.name
            try:
                output_file = ros_convert_srv_to_idl(
                    package_dir=package_path,
                    package_name=package_name,
                    input_file=relative_input,
                    output_dir=output_path
                )
                converted_files.append(str(output_file.absolute()))
            except Exception as e:
                print(f"Warning: Failed to convert {srv_file}: {e}", file=sys.stderr)

    # Also include any existing .idl files
    for subdir in ['msg', 'srv', 'action']:
        idl_dir = package_path / subdir
        if idl_dir.exists():
            for idl_file in idl_dir.glob('*.idl'):
                converted_files.append(str(idl_file.absolute()))

    if not converted_files:
        raise RuntimeError(f"No interface files found in {package_path}/msg/ or {package_path}/srv/")

    return converted_files


def create_generator_arguments_file(
    package_name: str,
    idl_files: List[str],
    include_paths: List[str],
    output_dir: str
) -> str:
    """Create a temporary generator arguments file."""
    script_dir = Path(__file__).parent.absolute()
    rosidl_dir = script_dir / 'rosidl'
    templates_path = rosidl_dir / 'rosidl_generator_type_description' / 'resource'

    # Convert IDL files to the expected tuple format
    idl_tuples = []
    for idl_file in idl_files:
        idl_path = Path(idl_file)
        if idl_path.is_absolute():
            # If absolute path, use it as is
            prefix_path = idl_path.parent
            relative_path = idl_path.name
        else:
            # If relative path, assume it's relative to current directory
            prefix_path = Path.cwd()
            relative_path = idl_file

        idl_tuples.append(f"{prefix_path}:{relative_path}")

    # Expand include paths to discover packages in parent directories
    expanded_include_paths = expand_include_paths(include_paths)

    # Convert include paths to the expected format
    # For dependency resolution, we need to point to the output directory where .json files are located
    include_path_tuples = []
    for include_path in expanded_include_paths:
        pkg_info = parse_package_xml(include_path)
        package_name_from_xml = pkg_info['name']
        # Point to the output directory where dependency JSON files are generated
        include_path_tuples.append(f"{package_name_from_xml}:{output_dir}")

    args = {
        'package_name': package_name,
        'output_dir': str(Path(output_dir).absolute()),
        'template_dir': str(templates_path.absolute()),
        'idl_tuples': idl_tuples,
        'include_paths': include_path_tuples,
        'ros_interface_dependencies': [],  # Empty for standalone usage
        'target_dependencies': []  # Empty for standalone usage
    }

    # Create temporary file
    with tempfile.NamedTemporaryFile(mode='w', suffix='.json', delete=False) as f:
        json.dump(args, f, indent=2)
        return f.name


def parse_type_descriptions(output_dir: str) -> Dict[str, Dict]:
    """
    Parse generated JSON type description files to extract type information.

    :param output_dir: Directory containing generated .json files
    :return: Dictionary mapping type names to their descriptions
    """
    type_info = {}
    output_path = Path(output_dir)

    # ROS type ID to type name mapping (from rosidl_runtime_c)
    ros_type_id_to_name = {
        # Basic types
        1: 'nested',  # FIELD_TYPE_NESTED_TYPE
        2: 'int8',    # FIELD_TYPE_INT8
        3: 'uint8',   # FIELD_TYPE_UINT8
        4: 'int16',   # FIELD_TYPE_INT16
        5: 'uint16',  # FIELD_TYPE_UINT16
        6: 'int32',   # FIELD_TYPE_INT32
        7: 'uint32',  # FIELD_TYPE_UINT32
        8: 'int64',   # FIELD_TYPE_INT64
        9: 'uint64',  # FIELD_TYPE_UINT64
        10: 'float32', # FIELD_TYPE_FLOAT
        11: 'float64', # FIELD_TYPE_DOUBLE
        # 12: 'long double', # FIELD_TYPE_LONG_DOUBLE
        13: 'char',   # FIELD_TYPE_CHAR
        # 14: 'wchar',  # FIELD_TYPE_WCHAR
        15: 'bool',   # FIELD_TYPE_BOOLEAN
        16: 'byte',   # FIELD_TYPE_BYTE
        17: 'string', # FIELD_TYPE_STRING
        # 18: 'wstring', # FIELD_TYPE_WSTRING
        # 19: 'fixed_string', # FIELD_TYPE_FIXED_STRING
        # 20: 'fixed_wstring', # FIELD_TYPE_FIXED_WSTRING
        # 21: 'bounded_string', # FIELD_TYPE_BOUNDED_STRING
        # 22: 'bounded_wstring', # FIELD_TYPE_BOUNDED_WSTRING

        # Fixed size arrays (49-70)
        # 49: 'nested[]',  # FIELD_TYPE_NESTED_TYPE_ARRAY
        50: 'int8[]',    # FIELD_TYPE_INT8_ARRAY
        51: 'uint8[]',   # FIELD_TYPE_UINT8_ARRAY
        52: 'int16[]',   # FIELD_TYPE_INT16_ARRAY
        53: 'uint16[]',  # FIELD_TYPE_UINT16_ARRAY
        54: 'int32[]',   # FIELD_TYPE_INT32_ARRAY
        55: 'uint32[]',  # FIELD_TYPE_UINT32_ARRAY
        56: 'int64[]',   # FIELD_TYPE_INT64_ARRAY
        57: 'uint64[]',  # FIELD_TYPE_UINT64_ARRAY
        58: 'float32[]', # FIELD_TYPE_FLOAT_ARRAY
        59: 'float64[]', # FIELD_TYPE_DOUBLE_ARRAY
        # 60: 'long double[]', # FIELD_TYPE_LONG_DOUBLE_ARRAY
        61: 'char[]',    # FIELD_TYPE_CHAR_ARRAY
        # 62: 'wchar[]',   # FIELD_TYPE_WCHAR_ARRAY
        63: 'bool[]',    # FIELD_TYPE_BOOLEAN_ARRAY
        64: 'byte[]',    # FIELD_TYPE_BYTE_ARRAY
        65: 'string[]',  # FIELD_TYPE_STRING_ARRAY
        # 66: 'wstring[]', # FIELD_TYPE_WSTRING_ARRAY
        # 67: 'fixed_string[]', # FIELD_TYPE_FIXED_STRING_ARRAY
        # 68: 'fixed_wstring[]', # FIELD_TYPE_FIXED_WSTRING_ARRAY
        # 69: 'bounded_string[]', # FIELD_TYPE_BOUNDED_STRING_ARRAY
        # 70: 'bounded_wstring[]', # FIELD_TYPE_BOUNDED_WSTRING_ARRAY

        # Bounded sequences (97-118)
        # 97: 'nested_bounded_sequence',  # FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE
        # 98: 'int8_bounded_sequence',    # FIELD_TYPE_INT8_BOUNDED_SEQUENCE
        # 99: 'uint8_bounded_sequence',   # FIELD_TYPE_UINT8_BOUNDED_SEQUENCE
        # 100: 'int16_bounded_sequence',  # FIELD_TYPE_INT16_BOUNDED_SEQUENCE
        # 101: 'uint16_bounded_sequence', # FIELD_TYPE_UINT16_BOUNDED_SEQUENCE
        # 102: 'int32_bounded_sequence',  # FIELD_TYPE_INT32_BOUNDED_SEQUENCE
        # 103: 'uint32_bounded_sequence', # FIELD_TYPE_UINT32_BOUNDED_SEQUENCE
        # 104: 'int64_bounded_sequence',  # FIELD_TYPE_INT64_BOUNDED_SEQUENCE
        # 105: 'uint64_bounded_sequence', # FIELD_TYPE_UINT64_BOUNDED_SEQUENCE
        # 106: 'float32_bounded_sequence', # FIELD_TYPE_FLOAT_BOUNDED_SEQUENCE
        # 107: 'float64_bounded_sequence', # FIELD_TYPE_DOUBLE_BOUNDED_SEQUENCE
        # 108: 'long_double_bounded_sequence', # FIELD_TYPE_LONG_DOUBLE_BOUNDED_SEQUENCE
        # 109: 'char_bounded_sequence',   # FIELD_TYPE_CHAR_BOUNDED_SEQUENCE
        # 110: 'wchar_bounded_sequence',  # FIELD_TYPE_WCHAR_BOUNDED_SEQUENCE
        # 111: 'bool_bounded_sequence',   # FIELD_TYPE_BOOLEAN_BOUNDED_SEQUENCE
        # 112: 'byte_bounded_sequence',   # FIELD_TYPE_BYTE_BOUNDED_SEQUENCE
        # 113: 'string_bounded_sequence', # FIELD_TYPE_STRING_BOUNDED_SEQUENCE
        # 114: 'wstring_bounded_sequence', # FIELD_TYPE_WSTRING_BOUNDED_SEQUENCE
        # 115: 'fixed_string_bounded_sequence', # FIELD_TYPE_FIXED_STRING_BOUNDED_SEQUENCE
        # 116: 'fixed_wstring_bounded_sequence', # FIELD_TYPE_FIXED_WSTRING_BOUNDED_SEQUENCE
        # 117: 'bounded_string_bounded_sequence', # FIELD_TYPE_BOUNDED_STRING_BOUNDED_SEQUENCE
        # 118: 'bounded_wstring_bounded_sequence', # FIELD_TYPE_BOUNDED_WSTRING_BOUNDED_SEQUENCE

        # Unbounded sequences (145-166)
        # 145: 'nested_unbounded_sequence', # FIELD_TYPE_NESTED_TYPE_UNBOUNDED_SEQUENCE
        # 146: 'int8_unbounded_sequence',   # FIELD_TYPE_INT8_UNBOUNDED_SEQUENCE
        # 147: 'uint8_unbounded_sequence',  # FIELD_TYPE_UINT8_UNBOUNDED_SEQUENCE
        # 148: 'int16_unbounded_sequence',  # FIELD_TYPE_INT16_UNBOUNDED_SEQUENCE
        # 149: 'uint16_unbounded_sequence', # FIELD_TYPE_UINT16_UNBOUNDED_SEQUENCE
        # 150: 'int32_unbounded_sequence',  # FIELD_TYPE_INT32_UNBOUNDED_SEQUENCE
        # 151: 'uint32_unbounded_sequence', # FIELD_TYPE_UINT32_UNBOUNDED_SEQUENCE
        # 152: 'int64_unbounded_sequence',  # FIELD_TYPE_INT64_UNBOUNDED_SEQUENCE
        # 153: 'uint64_unbounded_sequence', # FIELD_TYPE_UINT64_UNBOUNDED_SEQUENCE
        # 154: 'float32_unbounded_sequence', # FIELD_TYPE_FLOAT_UNBOUNDED_SEQUENCE
        # 155: 'float64_unbounded_sequence', # FIELD_TYPE_DOUBLE_UNBOUNDED_SEQUENCE
        # 156: 'long_double_unbounded_sequence', # FIELD_TYPE_LONG_DOUBLE_UNBOUNDED_SEQUENCE
        # 157: 'char_unbounded_sequence',   # FIELD_TYPE_CHAR_UNBOUNDED_SEQUENCE
        # 158: 'wchar_unbounded_sequence',  # FIELD_TYPE_WCHAR_UNBOUNDED_SEQUENCE
        # 159: 'bool_unbounded_sequence',   # FIELD_TYPE_BOOLEAN_UNBOUNDED_SEQUENCE
        # 160: 'byte_unbounded_sequence',   # FIELD_TYPE_BYTE_UNBOUNDED_SEQUENCE
        # 161: 'string_unbounded_sequence', # FIELD_TYPE_STRING_UNBOUNDED_SEQUENCE
        # 162: 'wstring_unbounded_sequence', # FIELD_TYPE_WSTRING_UNBOUNDED_SEQUENCE
        # 163: 'fixed_string_unbounded_sequence', # FIELD_TYPE_FIXED_STRING_UNBOUNDED_SEQUENCE
        # 164: 'fixed_wstring_unbounded_sequence', # FIELD_TYPE_FIXED_WSTRING_UNBOUNDED_SEQUENCE
        # 165: 'bounded_string_unbounded_sequence', # FIELD_TYPE_BOUNDED_STRING_UNBOUNDED_SEQUENCE
        # 166: 'bounded_wstring_unbounded_sequence', # FIELD_TYPE_BOUNDED_WSTRING_UNBOUNDED_SEQUENCE
    }

    for json_file in output_path.rglob('*.json'):
        try:
            with open(json_file, 'r') as f:
                data = json.load(f)

            if 'type_hashes' in data and 'type_description_msg' in data:
                type_hashes = {th['type_name']: th['hash_string'].replace('RIHS01_', '') for th in data['type_hashes']}

                # Process main type description
                main_desc = data['type_description_msg']['type_description']
                type_name = main_desc['type_name']
                skip = False
                if 'referenced_type_descriptions' in data['type_description_msg']:
                    ref_types = data['type_description_msg']['referenced_type_descriptions']
                    for ref_type in ref_types:
                        for field in ref_type['fields']:
                            type_id_n = field["type"]['type_id']
                            if type_id_n not in ros_type_id_to_name.keys():
                                print(f"Skipping {type_name}, incompatible ref field type {type_id_n} - {ref_type['type_name']}")
                                skip = True
                                break

                if type_name in type_hashes and skip == False:
                    fields = parse_json_fields(main_desc['fields'], ros_type_id_to_name)

                    if fields != None:
                        print(f"Adding {type_name}")
                        type_info[type_name] = {
                            'hash': type_hashes[type_name],
                            'fields': fields,
                            'original_name': type_name
                        }
                    else:
                        print(f"Skipping {type_name}, incompatible field types")

        except Exception as e:
            print(f"Warning: Failed to parse {json_file}: {e}", file=sys.stderr)

    return type_info


def parse_json_fields(fields: List[Dict], type_id_map: Dict[int, str]) -> List[Dict]:
    """
    Parse JSON field definitions to extract field information.

    :param fields: List of field dictionaries from JSON
    :param type_id_map: Mapping from ROS type IDs to type names
    :return: List of field dictionaries with type and name
    """
    parsed_fields = []

    for field in fields:
        field_name = field['name']
        field_type_info = field['type']
        type_id = field_type_info['type_id']
        capacity = field_type_info.get('capacity', 0)
        nested_type_name = field_type_info.get('nested_type_name', '')

        if type_id == 1 and nested_type_name:
            # Nested type
            parsed_fields.append({
                'type': nested_type_name,
                'name': field_name,
                'is_array': False,
                'array_size': None
            })
        elif type_id in type_id_map:
            type_name = type_id_map[type_id]

            if type_name.endswith('[]') and capacity > 0:
                # Fixed-size array
                base_type = type_name[:-2]  # Remove '[]'
                parsed_fields.append({
                    'type': base_type,
                    'name': field_name,
                    'is_array': True,
                    'array_size': capacity
                })
            else:
                # Regular field
                parsed_fields.append({
                    'type': type_name,
                    'name': field_name,
                    'is_array': False,
                    'array_size': None
                })
        else:
            print(f"Field {field_name} incompatible ID {type_id}")
            return None

    return parsed_fields


def convert_ros_to_c_type_name(ros_type_name: str) -> str:
    """
    Convert ROS type name to valid C identifier.

    :param ros_type_name: ROS type name like "nav_msgs/srv/GetMap"
    :return: C-compatible type name like "ros_nav_msgs_srv_GetMap"
    """
    type_parts = ros_type_name.split("/")
    return f"ros_{type_parts[2]}"

def convert_ros_to_dds_type_name(ros_type_name: str) -> str:
    """
    Convert ROS type name to valid RMW identifier.

    :param ros_type_name: ROS type name like "nav_msgs/srv/GetMap"
    :return: RWM-compatible type name like "nav_msgs::srv::dds_::GetMap"
    """
    type_parts = ros_type_name.split("/")

    return f"{type_parts[0]}::{type_parts[1]}::dds_::{type_parts[2]}"


def convert_ros_to_service_c_name(ros_type_name: str) -> str:
    """
    Convert ROS service type name to valid C identifier for services.

    :param ros_type_name: ROS service type name like "nav_msgs/srv/GetMap"
    :return: C-compatible service name like "srv_GetMap"
    """
    type_parts = ros_type_name.split("/")
    return f"srv_{type_parts[2]}"


def group_service_types(type_info: Dict[str, Dict]) -> Dict[str, Dict]:
    """
    Group service-related types (Request, Response, base service) together.

    :param type_info: Dictionary of all type information
    :return: Dictionary of grouped service information
    """
    service_groups = {}

    for type_name, info in type_info.items():
        if "/srv/" not in type_name:
            continue

        # Skip Event types - not needed for C library
        if type_name.endswith("_Event"):
            continue

        # Extract base service name
        if type_name.endswith("_Request"):
            base_name = type_name[:-8]  # Remove "_Request"
            group_key = base_name
            field_key = "request"
        elif type_name.endswith("_Response"):
            base_name = type_name[:-9]  # Remove "_Response"
            group_key = base_name
            field_key = "response"
        else:
            # This is the base service type
            base_name = type_name
            group_key = base_name
            field_key = "base"

        if group_key not in service_groups:
            service_groups[group_key] = {}

        service_groups[group_key][field_key] = info

    # Filter out incomplete service groups (need at least request and response)
    complete_services = {}
    for service_name, group in service_groups.items():
        if "request" in group and "response" in group:
            complete_services[service_name] = group

    return complete_services


def classify_type(fields: List[Dict]) -> str:
    """
    Classify a type as BTYPE, CTYPE, or TTYPE based on its fields.

    :param fields: List of field dictionaries
    :return: Type classification string
    """
    if not fields:
        return "BTYPE"  # Empty type, treat as basic

    if len(fields) == 1 and not fields[0]['is_array']:
        # Single non-array field - could be BTYPE or TTYPE
        field_type = fields[0]['type']

        # Check if it's a primitive type (BTYPE)
        primitive_types = {
            'bool', 'int8', 'uint8', 'int16', 'uint16',
            'int32', 'uint32', 'int64', 'uint64',
            'float32', 'float64', 'string', 'wstring'
        }

        if field_type in primitive_types:
            return "BTYPE"
        else:
            return "TTYPE"  # Alias for another ROS type

    # Multiple fields or array fields - compound type
    return "CTYPE"


def get_c_primitive_type(ros_type: str) -> str:
    """
    Map ROS primitive types to C types.

    :param ros_type: ROS type name
    :return: Corresponding C type
    """
    type_mapping = {
        'bool': 'bool',
        'int8': 'int8_t',
        'uint8': 'uint8_t',
        'int16': 'int16_t',
        'uint16': 'uint16_t',
        'int32': 'int32_t',
        'uint32': 'uint32_t',
        'int64': 'int64_t',
        'uint64': 'uint64_t',
        'float32': 'float',
        'float64': 'double',
        'string': 'rstring',
        'wstring': 'rwstring',
        'byte' : 'uint8_t'
    }

    return type_mapping.get(ros_type, ros_type)


def create_type_name_mapping(type_info: Dict[str, Dict]) -> Dict[str, str]:
    """
    Create mapping from ROS type names to C type names for exact lookup.

    :param type_info: Dictionary of all type information
    :return: Dictionary mapping ROS type names to C type names
    """
    mapping = {}
    for type_name in type_info.keys():
        if "/srv/" not in type_name:  # Only include messages for field types
            c_name = convert_ros_to_c_type_name(type_name)
            mapping[type_name] = c_name
    return mapping


def generate_fields_string(fields: List[Dict], type_info: Dict[str, Dict]) -> Optional[str]:
    """
    Generate field definitions string for a type.

    :param fields: List of field dictionaries
    :param type_info: Dictionary of all type information for validation
    :return: String with field definitions or None if missing types
    """
    # Handle empty request/reply (dummy fields)
    if (len(fields) == 1 and
        fields[0]['name'] == 'structure_needs_at_least_one_member'):
        return f'            FIELD(uint8_t, empty) \\\n'

    # Create type name mapping for exact lookup
    type_name_mapping = create_type_name_mapping(type_info)
    field_lines = []

    for field in fields:
        field_type = field['type']
        field_name = field['name']

        # Check if field type exists in our type_info (exact lookup)
        if field_type in type_name_mapping:
            field_type = type_name_mapping[field_type]
        else:
            # Try primitive type mapping
            primitive_type = get_c_primitive_type(field_type)
            if primitive_type == field_type and field_type != 'bool':
                # Not a primitive type and not found in type_info
                return None  # Missing type - can't generate this service
            field_type = primitive_type

        if field['is_array'] and field['array_size'] is not None:
            field_lines.append(f'            ARRAY({field_type}, {field_name}, {field["array_size"]}) \\\n')
        else:
            field_lines.append(f'            FIELD({field_type}, {field_name}) \\\n')

    return ''.join(field_lines)


def generate_srv_list(service_groups: Dict[str, Dict], type_info: Dict[str, Dict]) -> str:
    """
    Generate SRV_LIST macro content for services.

    :param service_groups: Dictionary of grouped service information
    :param type_info: Dictionary of all type information for field validation
    :return: String with SRV_LIST macro content
    """
    if not service_groups:
        return ""

    entries = []
    dropped_services = []

    for service_name, group in service_groups.items():
        # Get hash from base service or fallback to request hash
        service_hash = group.get("base", {}).get("hash", group["request"]["hash"])

        c_name = convert_ros_to_service_c_name(service_name)
        dds_name = convert_ros_to_dds_type_name(service_name)

        # Generate request fields
        request_fields = generate_fields_string(group["request"]["fields"], type_info)

        # Generate response fields
        response_fields = generate_fields_string(group["response"]["fields"], type_info)

        # Check if field generation failed (missing types)
        if request_fields is None or response_fields is None:
            dropped_services.append(service_name)
            continue
        if request_fields != "":
            request_fields + '\\\n'
        if response_fields != "":
            response_fields + '\\\n'

        entry =  f'    SRV({c_name}, \\\n'
        entry += f'        "{dds_name}", \\\n'
        entry += f'        "{service_hash}", \\\n'
        entry += f'        REQUEST( \\\n'
        entry += f'{request_fields}'
        entry += f'        ), \\\n'
        entry += f'        REPLY( \\\n'
        entry += f'{response_fields}'
        entry += f'        ) \\\n'
        entry += f'    )'

        entries.append(entry)

    # Print warnings for dropped services
    if dropped_services:
        print(f"Warning: Dropped {len(dropped_services)} service(s) due to unsupported field types:")
        for service in dropped_services:
            print(f"  - {service}")

    if entries:
        srv_list = '#define SRV_LIST(SRV, REQUEST, REPLY, FIELD, ARRAY) \\\n'
        srv_list += ' \\\n'.join(entries)
        srv_list += '\n'
        return srv_list

    return ""


def generate_c_header(type_info: Dict[str, Dict], output_file: str) -> None:
    """
    Generate C header file with type definitions in the required format.

    :param type_info: Dictionary of type information from parse_type_descriptions
    :param output_file: Path to output header file
    """
    # Build dependency graph for topological sorting
    type_names = list(type_info.keys())
    dependencies = {}

    for type_name, info in type_info.items():
        deps = set()
        for field in info['fields']:
            field_type = field['type']
            # Check if field type is another type in our set
            for other_type_name in type_names:
                other_c_name = convert_ros_to_c_type_name(other_type_name)
                if field_type == other_c_name or field_type.endswith(other_type_name.split('::')[-1]):
                    deps.add(other_type_name)
        dependencies[type_name] = deps

    # Perform topological sort
    sorted_types = topological_sort_types(type_names, dependencies)

    # Generate header content
    with open(output_file, 'w') as f:
        f.write('/* Generated ROS message type definitions */\n')
        f.write('#ifndef GENERATED_TYPES_H\n')
        f.write('#define GENERATED_TYPES_H\n\n')

        f.write('#define MSG_LIST(BTYPE, CTYPE, TTYPE, FIELD, ARRAY) \\\n')

        entries = []
        for type_name in sorted_types:
            if "srv" in type_name:
                continue
            info = type_info[type_name]
            c_name = convert_ros_to_c_type_name(type_name)
            dds_name = convert_ros_to_dds_type_name(type_name)
            classification = classify_type(info['fields'])

            entry = f'    {classification}({c_name}, \\\n'
            entry += f'        "{dds_name}", \\\n'
            entry += f'        "{info["hash"]}", \\\n'

            if classification == 'BTYPE':
                # Single primitive type
                primitive_type = get_c_primitive_type(info['fields'][0]['type'])
                entry += f'        {primitive_type} \\\n'
            elif classification == 'TTYPE':
                # Typedef to another ROS type
                field_type = info['fields'][0]['type']
                # Convert field type to C name if it's a ROS type
                for other_type_name in type_names:
                    if field_type.endswith(other_type_name.split('::')[-1]):
                        field_type = convert_ros_to_c_type_name(other_type_name)
                        break
                    else:
                        field_type = get_c_primitive_type(field_type)
                entry += f'        {field_type} \\\n'
            else:  # CTYPE
                # Compound type with multiple fields
                for field in info['fields']:
                    field_type = field['type']
                    field_name = field['name']

                    # Convert field type to C name if it's a ROS type
                    for other_type_name in type_names:
                        if field_type.endswith(other_type_name.split('::')[-1]):
                            field_type = convert_ros_to_c_type_name(other_type_name)
                            break
                    else:
                        # Map primitive types
                        field_type = get_c_primitive_type(field_type)

                    if field['is_array']:
                        entry += f'        ARRAY({field_type}, {field_name}, {field["array_size"]}) \\\n'
                    else:
                        entry += f'        FIELD({field_type}, {field_name}) \\\n'

            entry += '    )'
            entries.append(entry)

        f.write(' \\\n'.join(entries))
        f.write('\n\n')

        # Generate service list
        service_groups = group_service_types(type_info)
        if service_groups:
            srv_list_content = generate_srv_list(service_groups, type_info)
            f.write(srv_list_content)
            f.write('\n')

        f.write('#endif /* GENERATED_TYPES_H */\n')


def topological_sort_types(type_names: List[str], dependencies: Dict[str, Set[str]]) -> List[str]:
    """
    Topologically sort types based on their dependencies.

    :param type_names: List of type names to sort
    :param dependencies: Dictionary mapping type names to their dependencies
    :return: Topologically sorted list of type names
    """
    # Build reverse dependency graph (dependents)
    dependents = {name: set() for name in type_names}
    in_degree = {name: 0 for name in type_names}

    for type_name, deps in dependencies.items():
        for dep in deps:
            if dep in dependents:  # Only count dependencies within our type set
                dependents[dep].add(type_name)
                in_degree[type_name] += 1

    # Kahn's algorithm
    queue = [name for name in type_names if in_degree[name] == 0]
    result = []

    while queue:
        queue.sort()  # Ensure deterministic ordering
        current = queue.pop(0)
        result.append(current)

        for dependent in dependents[current]:
            in_degree[dependent] -= 1
            if in_degree[dependent] == 0:
                queue.append(dependent)

    # Add any remaining types (circular dependencies)
    remaining = [name for name in type_names if name not in result]
    result.extend(sorted(remaining))

    return result


def main():
    parser = argparse.ArgumentParser(
        description='Generate type hashes for ROS2 interface definitions',
        formatter_class=argparse.ArgumentDefaultsHelpFormatter
    )

    parser.add_argument(
        '--packages-dir',
        required=True,
        nargs='*',
        help='Path to package directory (single package) or workspace root (multiple packages)'
    )

    parser.add_argument(
        '--output-dir',
        default='./type_descriptions',
        help='Output directory for generated type descriptions'
    )

    parser.add_argument(
        '--header-file',
        default='generated_types.h',
        help='Output header file name'
    )

    args = parser.parse_args()

    # Setup environment
    setup_python_path()
    if True:
        # Create output directory
        output_path = Path(args.output_dir)
        output_path.mkdir(parents=True, exist_ok=True)

        # Process include paths and auto-generate dependencies first
        dependency_packages = []
        if args.packages_dir:
            print(f"Discovering dependency packages from include paths...")

            for include_path in args.packages_dir:
                found_deps = discover_packages(include_path, interface_packages_only=True)
                dependency_packages.extend(found_deps)

            # Remove duplicates and target packages from dependencies
            dependency_packages = [
                pkg for pkg in list(set(dependency_packages))
            ]

            print(f"Discovered {len(dependency_packages)} dependency package(s):")
            for pkg_dir in dependency_packages:
                pkg_info = parse_package_xml(pkg_dir)
                print(f"  {pkg_info['name']} ({pkg_dir})")

            # Auto-generate type descriptions for dependencies
            if dependency_packages:
                process_dependencies(dependency_packages, args.output_dir)

    # Parse generated JSON files and create C header
    print(f"\nGenerating C header file from type descriptions...")

    type_info = parse_type_descriptions(args.output_dir)

    if not type_info:
        print("Error: No type information found in generated files", file=sys.stderr)
        sys.exit(1)

    # Determine output header file path
    if os.path.isabs(args.header_file):
        header_path = args.header_file
    else:
        header_path = os.path.join(args.output_dir, args.header_file)

    generate_c_header(type_info, header_path)



if __name__ == '__main__':
    main()
