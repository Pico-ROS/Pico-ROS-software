# Pico-ROS

## What is this
This is a small layer above [zenoh-pico](https://github.com/eclipse-zenoh/zenoh-pico) working with [rmw-zenoh](https://github.com/ros2/rmw_zenoh)
designed to provide easy interaction with ROS.

This library provides some abstractions for ROS:
 - Node
 - Subscriber
 - Publisher
 - Service server
 
It is intended to be deployed on resource constrained devices wherever `zenoh-pico` will run. 
Eventhough it is easy to use on linux, it doesn't intend to compete with [rclc](https://github.com/ros2/rclc) or [rclcpp](https://github.com/ros2/rclcpp)
which have more features and are RMW agnostic.

User needs to provide their own CDR serialization / deserialization layer. This layer only passes the raw cdr buffer from RMW.

## Dependancies
### Build
 - [zenoh-pico](https://github.com/eclipse-zenoh/zenoh-pico)

### Functional
 - [rmw-zenoh](https://github.com/ros2/rmw_zenoh). Needed for correct ROS interaction.

### Examples 
 - [Micro-CDR](https://github.com/eProsima/Micro-CDR). CDR encoding / decoding.

 
## Building
```sh
git clone <repo>
cd <repo_path>
git submodule update --init --recursive
mkdir build
cd build
cmake..
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
