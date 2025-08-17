#!/usr/bin/env python3
"""
Mock ament_index_python module for standalone rosidl usage.

This module provides a minimal implementation of ament_index_python
that returns the correct paths for rosidl packages without requiring
a full ROS2 installation.
"""

import os


def get_package_share_directory(package_name):
    """
    Mock implementation of get_package_share_directory.
    
    Returns the package directory path for known rosidl packages.
    """
    # Base path where the rosidl packages are located
    base_path = os.path.dirname(os.path.abspath(__file__))
    rosidl_base = os.path.join(base_path, 'rosidl')
    
    if package_name == 'rosidl_generator_type_description':
        return os.path.join(rosidl_base, 'rosidl_generator_type_description')
    elif package_name == 'rosidl_generator_c':
        return os.path.join(rosidl_base, 'rosidl_generator_c')
    elif package_name == 'rosidl_generator_cpp':
        return os.path.join(rosidl_base, 'rosidl_generator_cpp')
    elif package_name == 'rosidl_typesupport_introspection_c':
        return os.path.join(rosidl_base, 'rosidl_typesupport_introspection_c')
    elif package_name == 'rosidl_typesupport_introspection_cpp':
        return os.path.join(rosidl_base, 'rosidl_typesupport_introspection_cpp')
    else:
        raise ValueError(f"Package '{package_name}' not found in mock ament_index")


def get_package_prefix(package_name):
    """
    Mock implementation of get_package_prefix.
    
    Returns the same as get_package_share_directory for our purposes.
    """
    return get_package_share_directory(package_name)


def get_packages_with_prefixes():
    """
    Mock implementation of get_packages_with_prefixes.
    
    Returns a dictionary of available packages.
    """
    base_path = os.path.dirname(os.path.abspath(__file__))
    rosidl_base = os.path.join(base_path, 'rosidl')
    
    return {
        'rosidl_generator_type_description': os.path.join(rosidl_base, 'rosidl_generator_type_description'),
        'rosidl_generator_c': os.path.join(rosidl_base, 'rosidl_generator_c'),
        'rosidl_generator_cpp': os.path.join(rosidl_base, 'rosidl_generator_cpp'),
        'rosidl_typesupport_introspection_c': os.path.join(rosidl_base, 'rosidl_typesupport_introspection_c'),
        'rosidl_typesupport_introspection_cpp': os.path.join(rosidl_base, 'rosidl_typesupport_introspection_cpp'),
    }