# Pico-ROS type header file generator
- Uses rosidl to generate .idl and .json type descriptors from msg and srv files
- Parses created .json files to generate Pico-ROS user_type header file.
- Runs on host without ROS installation with virtual enviroment

## Usage
```
pyton generate_type_header.py
```
- `--packages-dir` path(s) to interfaces and their dependancies, searched recursively
- `--output-dir` output directrory to store generated .json files
- `--header-file` name of generated header file

## Limitations
Uses only ROS `type_id`'s supported by picoserdes: 
- basic types
- strings
- fixed arrays.

## example_types.h 
Run `./make_example_types.sh` to generate.