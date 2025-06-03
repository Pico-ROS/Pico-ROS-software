# Pico-ROS

## Overview
**Pico-ROS** is a lightweight ROS (Robot Operating System) client implementation designed for resource-constrained devices. Built on top of [zenoh-pico](https://github.com/eclipse-zenoh/zenoh-pico) and working in conjunction with [rmw-zenoh](https://github.com/ros2/rmw_zenoh), it provides an easy-to-use interface to ROS host from embedded systems.

## Features

### Core Components

1. **picoros**
   - A lightweight ROS client implementation providing core abstractions:
     - Node
     - Publisher
     - Subscriber
     - Service server

2. **picoserdes**
   - CDR serialization/deserialization for ROS messages
   - Custom type generation from header files.

3. **picoparams**
   - Parameter server implementation built using picoros and picoserdes
   - Customizable parameter backend interface
   - Compatible with ROS 2 parameter services

## Getting Started

### Prerequisites
- C compiler and CMake build system
- [zenoh-pico](https://github.com/eclipse-zenoh/zenoh-pico)
- [Micro-CDR](https://github.com/eProsima/Micro-CDR)
- [rmw-zenoh](https://github.com/ros2/rmw_zenoh) (for ROS 2 integration)

### Building the Project

   ```sh
   git clone <repo>
   cd <repo_path>
   git submodule update --init --recursive
   mkdir build
   cd build
   cmake ..
   make
   ```

#### Build Options
- Custom type definitions: `-DUSER_TYPE_FILE="user_types.h"`
- Disable examples: `-DPICOROS_BUILD_EXAMPLES=OFF`

### Examples

The project includes several example applications demonstrating different features:

- **Basic Communication**
  - `talker.c`: Basic publisher example
  - `listener.c`: Basic subscriber example

- **Advanced Features**
  - `srv_add2ints.c`: Service server example
  - `params_server.c`: Parameter server implementation
  - `odometry_publisher.c` & `odometry_listener.c`: ROS odometry message handling

#### Running the Examples

1. Ensure ROS 2 is running with `rmw_zenoh` ([Installation Guide](https://github.com/ros2/rmw_zenoh?tab=readme-ov-file#installation))

2. In separate terminals, run the example nodes:

   ```sh
   # Terminal 1: Run publisher
   ./talker -m client -a <zenoh-router-address>

   # Terminal 2: Run subscriber
   ./listener -m client -a <zenoh-router-address>

   # Terminal 3: Run service server
   ./srv_add2ints -m client -a <zenoh-router-address>

   # Terminal 4: Run parameter server
   ./param_server -m client -a <zenoh-router-address>
   ```

3. Interact with nodes from ROS 2:
   ```sh
   # Subscribe to messages
   ros2 topic echo /picoros/chatter

   # Call service
   ros2 service call /picoros/add2 example_interfaces/srv/AddTwoInts '{a: 1, b: 2}'

   # List parameters
   ros2 param list /picoros
   ```

## Documentation

The project uses Doxygen for API documentation. To generate the documentation:

1. Install dependencies:
   ```sh
   # For Ubuntu/Debian
   sudo apt-get install doxygen graphviz

   # For Arch Linux
   sudo pacman -S doxygen graphviz
   ```

2. Generate documentation:
   ```sh
   cd docs
   ./generate.sh
   ```

3. View documentation:
   ```sh
   # Open in your default browser
   xdg-open docs/generated/html/index.html
   ```

The documentation includes:
- API reference for all components
- Code examples and usage
- Architecture overview
- Implementation details

## Use Cases

Pico-ROS is ideal for resource-constrained microcontroller-based ROS nodes and edge devices in ROS networks.

While it provides core ROS functionality, it's not intended to replace full ROS implementations like [rclc](https://github.com/ros2/rclc) or [rclcpp](https://github.com/ros2/rclcpp) for systems with more resources.

## Using in your project

See the examples directory for implementation references. When adding new types, use the `example_types.h` as a template for the required format.

## License

See [LICENSE](LICENSE) file for details.

