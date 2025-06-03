/*******************************************************************************
 * @file    talker.c
 * @brief   Example talker node for picoros
 * @date    2025-May-27
 * 
 * @details This example demonstrates a simple ROS publisher node that
 *          publishes string messages on the "picoros/chatter" topic.
 * 
 * @copyright Copyright (c) 2025 Ubiquity Robotics
 *******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "picoros.h"
#include "picoserdes.h"

// Use command line arguments to change default values
#define MODE        "client"
#define LOCATOR     "tcp/192.168.1.16:7447"

// Common utils
extern int picoros_parse_args(int argc, char **argv, picoros_interface_t* ifx);

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

void publish_log(){
    printf("Publishing log...\n");
    char* msg = "Hello from Pico-ROS!";
    size_t len = ps_serialize(pub_buf, &msg, 1020);
    picoros_publish(&pub_log, pub_buf, len);
}

int main(int argc, char **argv){
    picoros_interface_t ifx = {
        .mode = MODE,
        .locator = LOCATOR,
    };
    int ret = picoros_parse_args(argc, argv , &ifx);

    if(ret != 0){
        return ret;
    }

    printf("Starting pico-ros interface %s %s\n", ifx.mode, ifx.locator );
    while (picoros_interface_init(&ifx) == PICOROS_NOT_READY){
        printf("Waiting RMW init...\n");
        z_sleep_s(1);
    }

    printf("Starting Pico-ROS node %s domain:%d\n", node.name, node.domain_id);
    picoros_node_init(&node);

    printf("Declaring publisher on %s\n", pub_log.topic.name);
    picoros_publisher_declare(&node, &pub_log);

    while(true){
        publish_log();
        z_sleep_s(1);
    }
    return 0;
}
