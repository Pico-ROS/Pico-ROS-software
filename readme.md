# Pico-ROS

## What is this?
**Pico-ROS** is a small layer above [zenoh-pico](https://github.com/eclipse-zenoh/zenoh-pico) working with [rmw-zenoh](https://github.com/ros2/rmw_zenoh)
designed to provide easy interaction with ROS.

This repository provides two independant modules:

### picoros
Provides abstractions for ROS: 

 - Node
 - Subscriber 
 - Publisher
 - Service server 

 Works with zenoh-pico and depends on zenoh-rmw.

### picoserdes 
Poor man's type abstractions and CDR serialization/deserialization. 

## Supported targets
**Pico-ROS** is intended to be deployed on resource constrained devices wherever `zenoh-pico` will run. 
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

Inside `<repo>/build`
```sh
# first terminal
./talker -m client -a <address of ROS zenoh router>

# second terminal
./listener -m client -a <address of ROS zenoh router>

# third terminal
./srv_add2ints -m client -a <address of ROS zenoh router>

# fourth terminal
<ROS_HOST>:~$ ros2 service call /picoros/add2 example_interfaces/srv/AddTwoInts '{a: 1, b: 2}'
requester: making request: example_interfaces.srv.AddTwoInts_Request(a=1, b=2)

response:
example_interfaces.srv.AddTwoInts_Response(sum=3)

```

## Picoserdes typs
User should provide their own list of types for creating serialization and deserialization functions.
This type list header file name can be provided with `USER_TYPE_FILE` define when building the library.

See examples/example_types.h for format description and a sample of some standard ROS types.