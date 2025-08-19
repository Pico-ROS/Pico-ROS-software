/*******************************************************************************
 * @file    jointState_publisher.cpp
 * @brief   Example cpp jointState_publisher node for picoros
 * @date    2025-Avg-19
 *
 * @details This example demonstrates a simple ROS publisher node that
 *          publishes jointState messages on the "picoros/joint" topic.
 *
 * @copyright Copyright (c) 2025 Ubiquity Robotics
 *******************************************************************************/

#include <cstdio>
#include <cstdint>
#include "picoros.h"
#include "picoserdes.h"
#include "zenoh-pico/system/common/platform.h"
#include "zenoh-pico/system/platform/unix.h"

// Use command line arguments to change default values
constexpr const char* MODE = "client";
constexpr const char* LOCATOR = "tcp/192.168.1.16:7447";

// Common utils
extern "C" int picoros_parse_args(int argc, char **argv, picoros_interface_t* ifx);

// Example Publisher
picoros_publisher_t publisher = {
    .topic = {
        .name = "picoros/joint",
        .type = ROSTYPE_NAME(ros_JointState),
        .rihs_hash = ROSTYPE_HASH(ros_JointState),
    },
};

// Example node
picoros_node_t node = {
    .name = "talker",
};

// Buffer for publication, used from this thread
uint8_t pub_buf[1024];

void publish_log() {
    double positions[] = {-1, 0, 1};
    double velocities[] = {0.5, 0, -0.1};
    double efforst[] = {0.5, 0, 0.2};
    const char* names[] = {"joint1", "joint2", "joint3"};
    z_clock_t now = z_clock_now();
    ros_JointState joint = {
        .header = {
            .stamp = {
                .sec = (int32_t)now.tv_sec,
                .nanosec = (uint32_t)now.tv_nsec,
            },
        },
        .name = {.data = (char**)names, .n_elements = 3},
        .position = {.data = positions, .n_elements = 3},
        .velocity = {.data = velocities, .n_elements = 3},
        .effort = {.data = efforst, .n_elements = 3},
    };
    printf("Publishing JointState...\n");
    size_t len = ps_serialize(pub_buf, &joint, 1024);
    if (len > 0){
        picoros_publish(&publisher, pub_buf, len);
    }
    else{
        printf("Message serialization error.");
    }

}

int main(int argc, char **argv) {
    picoros_interface_t ifx = {
        .mode = const_cast<char*>(MODE),
        .locator = const_cast<char*>(LOCATOR),
    };
    int ret = picoros_parse_args(argc, argv, &ifx);

    if (ret != 0) {
        return ret;
    }

    std::printf("Starting pico-ros interface %s %s\n", ifx.mode, ifx.locator);
    while (picoros_interface_init(&ifx) == PICOROS_NOT_READY) {
        std::printf("Waiting RMW init...\n");
        z_sleep_s(1);
    }

    std::printf("Starting Pico-ROS node %s domain:%d\n", node.name, node.domain_id);
    picoros_node_init(&node);

    std::printf("Declaring publisher on %s\n", publisher.topic.name);
    picoros_publisher_declare(&node, &publisher);

    while (true) {
        publish_log();
        z_sleep_s(1);
    }
    return 0;
}
