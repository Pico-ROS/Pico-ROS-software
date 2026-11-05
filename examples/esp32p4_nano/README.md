# Pico-ROS on the Waveshare ESP32-P4-NANO (ESP-IDF, Ethernet)

Barebones Pico-ROS node using this repository's ESP-IDF component ([components/picoros](../../components/picoros)),
which builds `src/` and the zenoh-pico and Micro-CDR submodules in place. Nothing is vendored or patched.

* Ethernet: on-board IP101 PHY (MDC 31, MDIO 52, reset 51, addr 1), DHCP
* zenoh client to a ROS 2 `rmw_zenohd` router
* `/picoros/chatter` (std_msgs/String, 1 Hz) published by the board
* `/picoros/listener` (std_msgs/String) subscribed and logged by the board

## Build and flash (ESP-IDF v5.5.x)

```sh
cd examples/esp32p4_nano
idf.py set-target esp32p4
idf.py menuconfig        # "Pico-ROS example": router locator, node name, domain
idf.py -p <port> build flash monitor
```

The component generates `zenoh-pico/config.h` in the build directory with a configure-only CMake pass of the
zenoh-pico submodule (defaults: TCP client, no scouting/multicast, lease 60000 ms x2 to match `rmw_zenohd`).
The project's `CMakeLists.txt` only adds `components/` to `EXTRA_COMPONENT_DIRS` and turns on `PICOROS_DEBUG`;
see the component for the other options (your own type header, zenoh feature flags, log level).

## ROS 2 side (Lyrical + rmw_zenoh)

```sh
export RMW_IMPLEMENTATION=rmw_zenoh_cpp
ros2 run rmw_zenoh_cpp rmw_zenohd                     # router on tcp/[::]:7447
ros2 node list                                        # /picoros_p4
ros2 topic echo /picoros/chatter
ros2 topic pub /picoros/listener std_msgs/msg/String "{data: hi}"
```

## Known issue: session drops with the current zenoh-pico pin

Tested 2026-11-04 on an ESP32-P4-NANO (chip v1.0) against ROS 2 Lyrical / rmw_zenoh_cpp 0.10.6. Publishing,
subscribing and graph discovery all work, but the pinned zenoh-pico ignores the socket timeout in
`src/link/transport/tcp/tcp_esp32.c` (`_ZP_UNUSED(tout)`), so the single zenoh-pico executor task blocks in
`recv()` and cannot send keep-alives. On this board that shows up as:

* a 30 s stall inside `picoros_interface_init()` after `z_open()`;
* `rmw_zenohd` expiring the board's lease (`expired after 60000 milliseconds`) ~90 s after connecting and
  every ~60 s after each reconnect, with ~60 s offline each time while `picoros_publish()` still returns
  `PICOROS_OK`.

See Pico-ROS issue #23. The fix has been submitted to zenoh-pico as eclipse-zenoh/zenoh-pico#1361
(with #1362 and #1363 for two related executor issues); with those applied the same test ran 10 minutes
with no lease expiries and no startup stall.
