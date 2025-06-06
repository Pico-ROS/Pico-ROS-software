/*******************************************************************************
 * @file    odometry_listener.c
 * @brief   Example odometry subscriber node for picoros
 * @date    2025-May-27
 * 
 * @details This example demonstrates a ROS subscriber node that listens to
 *          odometry messages on the "odom" topic and prints the pose data.
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

// Subscriber callback
void odometry_callback(uint8_t* rx_data, size_t data_len);

// Example Subscriber
picoros_subscriber_t sub_odo = {
    .topic = {
        .name = "odom",
        .type = ROSTYPE_NAME(ros_Odometry),
        .rihs_hash = ROSTYPE_HASH(ros_Odometry),
    },
    .user_callback = odometry_callback,
};

// Example node
picoros_node_t node = {
    .name = "listener",
};

void odometry_callback(uint8_t* rx_data, size_t data_len){
    ros_Odometry odo = {};
    if (ps_deserialize(rx_data, &odo, data_len)){
        printf("New odometry frame:%s @%ds position x:%f y:%f z:%f\n",
            odo.child_frame_id, odo.header.time.sec,
            odo.pose.pose.position.x, odo.pose.pose.position.y, odo.pose.pose.position.z);
    }
    else{
        printf("Odometry message deserialization error\n");
    }
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

    printf("Declaring subscriber on %s\n", sub_odo.topic.name);
    picoros_subscriber_declare(&node, &sub_odo);

    while(true){
        z_sleep_s(1);
    }
    return 0;
}
