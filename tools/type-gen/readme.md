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
```

1: 'nested'
2: 'int8'
3: 'uint8'
4: 'int16'
5: 'uint16'
6: 'int32'
7: 'uint32'
8: 'int64'
9: 'uint64'
10: 'float32'
11: 'float64'
13: 'char'
15: 'bool'
16: 'byte'
17: 'string'

50: 'int8[]'
51: 'uint8[]'
52: 'int16[]'
53: 'uint16[]'
54: 'int32[]'
55: 'uint32[]'
56: 'int64[]'
57: 'uint64[]'
58: 'float32[]'
59: 'float64[]'
61: 'char[]'
63: 'bool[]'
64: 'byte[]'
65: 'string[]'

# Implemented as bounded sequence
145: 'nested_unbounded_sequence'
146: 'int8_unbounded_sequence'
147: 'uint8_unbounded_sequence'
148: 'int16_unbounded_sequence'
149: 'uint16_unbounded_sequence'
150: 'int32_unbounded_sequence'
151: 'uint32_unbounded_sequence'
152: 'int64_unbounded_sequence'
153: 'uint64_unbounded_sequence'
154: 'float32_unbounded_sequence'
155: 'float64_unbounded_sequence'
157: 'char_unbounded_sequence'
159: 'bool_unbounded_sequence'
160: 'byte_unbounded_sequence'
161: 'string_unbounded_sequence',
```

## example_types.h 
Run `./make_example_types.sh` to generate.