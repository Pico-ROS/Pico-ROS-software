# Pico-ROS

## What is this?
**Pico-ROS** is a small layer above [zenoh-pico](https://github.com/eclipse-zenoh/zenoh-pico) working with [rmw-zenoh](https://github.com/ros2/rmw_zenoh)
designed to provide easy interaction with ROS.

This repository provides the following modules:

### picoros
Provides abstractions for ROS: 

 - Node
 - Subscriber 
 - Publisher
 - Service server 

Works with zenoh-pico and depends on zenoh-rmw. Can be used independantly from other modules.

### picoserdes 
Poor man's type abstractions and CDR serialization/deserialization. 

Allows generation of serdes functions from user provided header files.

User should provide their own list of types for creating serialization and deserialization functions.
This type list header file name can be provided with `USER_TYPE_FILE` define when building the library.

See examples/example_types.h for format description and a sample of some standard ROS types.

### picoparams
Parameter server implementation build with **picoros** and **picoserdes** with
application interface allows connecting to custom paramters backend.

## Supported targets
**Pico-ROS** is intended to be deployed on resource constrained embedded devices wherever zenoh-pico will run. 
Eventhough it is easy to use on linux, it doesn't intend to compete with [rclc](https://github.com/ros2/rclc) or [rclcpp](https://github.com/ros2/rclcpp)
which have more features.

## Dependancies
### Build
 - **picoros**
    - [zenoh-pico](https://github.com/eclipse-zenoh/zenoh-pico)
 - **picoserdes**
    - [Micro-CDR](https://github.com/eProsima/Micro-CDR)

#### Functional
 - [rmw-zenoh](https://github.com/ros2/rmw_zenoh). Needed for correct ROS interaction.

## Building

```sh
git clone <repo>
cd <repo_path>
git submodule update --init --recursive
mkdir build
cd build
cmake..
#cmake.. -DUSER_TYPE_FILE="user_types.h" -DPICOROS_BUILD_EXAMPLES=OFF
make
```

## Running examples
Run ROS with `zenoh_rmw`([installation instructions](https://github.com/ros2/rmw_zenoh?tab=readme-ov-file#installation))

Inside `<repo>/build`:

```sh
# first terminal - run talker node
./talker -m client -a <address of ROS zenoh router>

# second terminal - run listener node
./listener -m client -a <address of ROS zenoh router>

# third terminal - run service server
./srv_add2ints -m client -a <address of ROS zenoh router>

# fourth terminal - run parameters server
./param_server -m client -a <address of ROS zenoh router>

# fifth terminal on ROS host
# listen to topic
ros2 topic echo /picoros/chatter
# call service
ros2 service call /picoros/add2 example_interfaces/srv/AddTwoInts '{a: 1, b: 2}'
# get params
ros2 param list /picoros
```

