/*******************************************************************************
 * @file    talker.cpp
 * @brief   Example cpp talker node for picoros
 * @date    2025-Avg-8
 * 
 * @details This example demonstrates a simple ROS publisher node that
 *          publishes string messages on the "picoros/chatter" topic.
 * 
 * @copyright Copyright (c) 2025 Ubiquity Robotics
 *******************************************************************************/

#include <cstdio>
#include <cstdint>
#include <string>
#include "picoros.h"
#include "picoserdes.h"

// Use command line arguments to change default values
constexpr const char* MODE = "client";
constexpr const char* LOCATOR = "tcp/192.168.1.16:7447";

// Common utils
extern "C" int picoros_parse_args(int argc, char **argv, picoros_interface_t* ifx);

// Example Publisher
picoros_publisher_t pub_log = {
    .topic = {
        .name = "picoros/chatter",
        .type = ROSTYPE_NAME(ros_String),
        .rihs_hash = ROSTYPE_HASH(ros_String),
    },
};

// Example node
picoros_node_t node = {
    .name = "talker",
};

// Buffer for publication, used from this thread
uint8_t pub_buf[1024];

void publish_log() {
    std::printf("Publishing log...\n");
    char* msg = "Hello from Pico-ROS!";
    size_t len = ps_serialize(pub_buf, &msg, 1020);
    picoros_publish(&pub_log, pub_buf, len);
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

    std::printf("Declaring publisher on %s\n", pub_log.topic.name);
    picoros_publisher_declare(&node, &pub_log);

    while (true) {
        publish_log();
        z_sleep_s(1);
    }
    return 0;
}
