/*******************************************************************************
 * @file    srv_server_add2ints.c
 * @brief   Example service server node for picoros
 * @date    2025-May-27
 * 
 * @details This example demonstrates a ROS service server implementation that
 *          provides an "add two integers" service functionality.
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
extern int picoros_parse_args(int argc, char **argv,  picoros_interface_t* ifx);

// Service callback
picoros_service_reply_t add2_srv_cb(uint8_t* request, size_t size, void* user_data);

// Static buffer for service reply serialization, used from zenoh threads
uint8_t srv_buf[1024];

// Example service
picoros_srv_server_t add2_srv = {
    .topic = {
        .name = "add2",
        .type = ROSTYPE_NAME(srv_AddTwoInts),
        .rihs_hash = ROSTYPE_HASH(srv_AddTwoInts),
    },
    .user_callback = add2_srv_cb,
};

// Example node
picoros_node_t node = {
    .name = "picoros",
};

// Service callback
picoros_service_reply_t add2_srv_cb(uint8_t* rx_data, size_t rx_size, void* user_data){
    // request, response structs
    request_srv_AddTwoInts request = {};
    reply_srv_AddTwoInts response = {};
    // deserialize request
    ps_deserialize(rx_data, &request, rx_size);
    // apply service
    response.sum = request.a + request.b;
    printf("Service add2(a:%ld, b:%ld) called. Sending reply sum:%ld\n", request.a, request.b, response.sum);
    // serialize reply
    size_t len = ps_serialize(srv_buf, &response , 1024);
    // send reply
    picoros_service_reply_t reply = {
        .length = len,
        .data = srv_buf,
        .free_callback = NULL, // no need to free static buffer
    };
    return reply;
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

    printf("Declaring service on %s\n", add2_srv.topic.name);
    picoros_service_declare(&node, &add2_srv);

    while(true){
        z_sleep_s(1);
    }
    return 0;
}
