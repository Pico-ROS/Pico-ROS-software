/*******************************************************************************
 * @file    odometry_publisher.c
 * @brief   Example odometry publisher node for picoros
 * @date    2025-May-27
 * 
 * @details This example demonstrates a ROS publisher node that publishes
 *          simulated odometry messages on the "odom" topic.
 * 
 * @copyright Copyright (c) 2025 Ubiquity Robotics
 *******************************************************************************/
  
#include <stdbool.h>
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
picoros_publisher_t pub_bs = {
    .topic = {
        .name = "battery_state",
        .type = ROSTYPE_NAME(ros_BatteryState),
        .rihs_hash = ROSTYPE_HASH(ros_BatteryState),
    },
};

// Example node
picoros_node_t node = {
    .name = "talker",
};

// Buffer for publication, used from this thread
uint8_t pub_buf[1024];

void publish_odometry(){
    ros_BatteryState bat = {
        .present = true,
        .location = "main",
        .serial_number = "ABCD1234",
        .design_capacity = 20000,
        .capacity = 18000,
        .percentage = 50,
        .charge = 9000,
        .current = 0.5f,
        .voltage = 25.1f,
        .cell_voltage = {.data = (float[]){12.5f, 12.6f}, .n_elements = 2},
        .cell_temperature = {.data = (float[]){31.2f, 32.5f}, .n_elements = 2},
    };
    printf("Publishing battery state...\n");
    size_t len = ps_serialize(pub_buf, &bat, 1024);
    if (len > 0){
        picoros_publish(&pub_bs, pub_buf, len);
    }
    else{
        printf("Message serialization error.");
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

    printf("Declaring publisher on %s\n", pub_bs.topic.name);
    picoros_publisher_declare(&node, &pub_bs);

    while(true){
        publish_odometry();
        z_sleep_s(1);
    }
    return 0;
}
