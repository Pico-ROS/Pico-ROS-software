/**
  ******************************************************************************
  * @file    listener.c
  * @date    2025-May-27
  * @brief   Example listener node for picoros
  * Copyright (c) 2025 Ubiquity Robotics
  ******************************************************************************
  */

#include <stdio.h>
#include <stdint.h>
#include "picoros.h"
#include "picoserdes.h"

// Use command line arguments to change default values
#define MODE        "client"
#define LOCATOR     "tcp/192.168.1.16:7447"

// Common utils
extern int picoros_parse_args(int argc, char **argv, picoros_interface_t* ifx);

// Subscriber callback
void log_callback(uint8_t*, size_t);

// Example Subscriber
picoros_subscriber_t sub_log = {
    .topic = {
        .name = "picoros/chatter",
        .type = ROSTYPE_NAME(ros_String),
        .rihs_hash = ROSTYPE_HASH(ros_String),
    },
    .user_callback = log_callback,
};

// Example node
picoros_node_t node = {
    .name = "listener",
};

void log_callback(uint8_t* rx_data, size_t data_len){
    char* msg = NULL;
    ps_deserialize(rx_data, &msg, data_len);
    printf("Subscriber recieved: %s\n", msg);
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

    printf("Declaring subscriber on %s\n", sub_log.topic.name);
    picoros_subscriber_declare(&node, &sub_log);

    while(true){
        z_sleep_s(1);
    }
    return 0;
}
