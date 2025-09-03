/*******************************************************************************
 * @file    srv_client_add2ints.c
 * @brief   Example service client for picoros
 * @date    2025-Sept-2
 * 
 * @details This example demonstrates a ROS service client implementation that
 *          calls an "add two integers" service and prints the result.
 * 
 * @copyright Copyright (c) 2025 Ubiquity Robotics
 *******************************************************************************/
  
#include <stdio.h>
#include <stdint.h>
#include "picoros.h"
#include "picoserdes.h"
#include "zenoh-pico/system/common/platform.h"

// Use command line arguments to change default values
#define MODE        "client"
#define LOCATOR     "tcp/192.168.1.16:7447"

// Common utils
extern int picoros_parse_args(int argc, char **argv,  picoros_interface_t* ifx);

// Service reply callback
void add2_client_cb(uint8_t* reply_data, size_t reply_size,  bool error);

// Example service
picoros_srv_client_t add2_client = {
    .node_name = "add_two_ints",
    .topic = {
        .type = ROSTYPE_NAME(srv_AddTwoInts),
        .rihs_hash = ROSTYPE_HASH(srv_AddTwoInts),
    },
    .user_callback = add2_client_cb,
    // .opts = &(z_get_options_t){
    //     .timeout_ms = 2000,
    //     .consolidation.mode = Z_CONSOLIDATION_MODE_MONOTONIC,
    //     .congestion_control = Z_CONGESTION_CONTROL_BLOCK,
    // },
};

void add2_client_cb(uint8_t* reply_data, size_t reply_size,  bool error){
    if (error){
        printf("Service error reply recieved\n");
        return; 
    }
    reply_srv_AddTwoInts response = {};
    ps_deserialize(reply_data, &response, reply_size);
    printf("Got reply - sum: %ld\n", response.sum);
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
    
    int a = 0;
    int b = 100;
    while(true){
        uint8_t buf[100];
        request_srv_AddTwoInts request = {.a=a, b=b};
        size_t len = ps_serialize(buf, &request, 100);
        if (picoros_service_call(&add2_client, buf, len) == PICOROS_OK){
            printf("Sent service call...\n");
            a++;
        }
        z_sleep_s(1);
    }
    return 0;
}
