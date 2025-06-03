/**
  ******************************************************************************
  * @file    params_server.c
  * @date    2025-May-27
  * @brief   Example parameters server node for picoparams
  * Copyright (c) 2025 Ubiquity Robotics
  ******************************************************************************
  */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "picoros.h"
#include "picoparams.h"

// Use command line arguments to change default values
#define MODE        "client"
#define LOCATOR     "tcp/192.168.1.16:7447"

// Common utils
extern int picoros_parse_args(int argc, char **argv,  picoros_interface_t* ifx);

// Static buffer for service reply serialization, used from zenoh threads
#define STATIC_BUF_SIZE 4096
uint8_t srv_buf[STATIC_BUF_SIZE];

// Example node
picoros_node_t node = {
    .name = "picoros",
};

// Example parameters
// Should probably be implemented as different structure in your application.
typedef struct{
    ros_ParameterDescriptor desc;
    ros_ParameterValue      val;
}parameter_t;

#define N_PARAMS 3
parameter_t params[N_PARAMS] = {
    {
        .desc = {
            .name = "example.param1",
            .description = "Super important parameter",
            .additional_constraints = "Must not be 0",
            .type = PARAMETER_INTEGER,
            .int_range.min = -50,
            .int_range.max =  50,
        },
        .val.type = PARAMETER_INTEGER,
        .val.val_int = 10,
        .val.length = 1,
    },{
        .desc = {
            .name = "example.param2",
            .description = "Don't touch!",
            .type = PARAMETER_DOUBLE,
            .float_range.min = -3.14,
            .float_range.max =  3.14,
        },
        .val.type = PARAMETER_DOUBLE,
        .val.val_double = 1.2525,
        .val.length = 1,

    },{
        .desc = {
            .name = "example.param3",
            .type = PARAMETER_BYTE_ARRAY,
        },
        .val.type = PARAMETER_BYTE_ARRAY,
        .val.val_bytearray = (uint8_t[]){1,2,3,4,5,6,7,8,9,10},
        .val.length = 10,
    },
};



picoparams_server_t params_server;

// Parameters server api functions declaration
void*                   api_param_ref(char* name);
ros_ParameterDescriptor api_param_describe(void* param);
ros_ParameterValue      api_param_get(void* param);
ros_ParameterType       api_param_type(void* param);
bool                    api_param_set(void* param, ros_ParameterValue* value, char** error_msg);
int                     api_param_list(char* prefix,  void (*write_next)(char* param_name) );
int                     api_prefix_list(char* prefix,  void (*write_next)(char* prefix_name) );




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

    picoparams_interface_t params_ifx = {
            .f_ref = api_param_ref,
            .f_get = api_param_get,
            .f_set = api_param_set,
            .f_describe = api_param_describe,
            .f_type = api_param_type,
            .f_list = api_param_list,
            .f_prefixes = api_prefix_list,
            .reply_buf = srv_buf,
            .reply_buf_size = STATIC_BUF_SIZE,
    };
    if (picoparams_init(&params_server, &node, params_ifx) != PICOROS_OK){
        printf("Parameters server init failed.\n");
        exit(-1);
    }
    printf("Parameter server started\n");

    while(true){
        z_sleep_s(1);
    }
    return 0;
}


void* api_param_ref(char* name){
    for (int i = 0; i < N_PARAMS; i++){
        if(strcmp(name, params[i].desc.name) == 0){
            return &params[i];
        }
    }
    printf("No param found: %s\n", name);
    return NULL;
}

ros_ParameterDescriptor api_param_describe(void* param){
    parameter_t* p = (parameter_t*)param;
    return p->desc;
}

ros_ParameterValue  api_param_get(void* param){
    parameter_t* p = (parameter_t*)param;
    return p->val;
}

ros_ParameterType api_param_type(void* param){
    parameter_t* p = (parameter_t*)param;
    return p->desc.type;
}

size_t get_param_value_size(parameter_t* p){
    size_t ret = sizeof(uint8_t);
    switch(p->val.type){
        case PARAMETER_INTEGER:
        case PARAMETER_INTEGER_ARRAY:
            ret = sizeof(int64_t);
            break;
        case PARAMETER_DOUBLE:
        case PARAMETER_DOUBLE_ARRAY:
            ret = sizeof(double);
            break;
        case PARAMETER_BOOL:
        case PARAMETER_BOOL_ARRAY:
            ret = sizeof(bool);
            break;
        default:
            break;
    }
    return ret * p->val.length;
}

bool api_param_set(void* param, ros_ParameterValue* value, char** error_msg){
    parameter_t* p = (parameter_t*)param;
    if (p->desc.type != value->type){
        *error_msg = "Error wrong value";
        return false;
    }
    if (p->val.length != value->length){
        *error_msg = "Error wrong length";
        return false;
    }
    switch(p->val.type){
        case PARAMETER_BOOL:
        case PARAMETER_INTEGER:
        case PARAMETER_DOUBLE:
            memcpy(&p->val.val_bool, &value->val_bool, get_param_value_size(p));
            break;
        case PARAMETER_BYTE_ARRAY:
        case PARAMETER_BOOL_ARRAY:
        case PARAMETER_INTEGER_ARRAY:
        case PARAMETER_DOUBLE_ARRAY:
            memcpy(p->val.val_bytearray, value->val_bytearray, get_param_value_size(p));
            break;
        default:
            *error_msg = "Set operation not supported";
            return false;
    }

    return true;
}

int api_param_list(char* prefix,  void (*write_next)(char* param_name) ){
    (void)prefix; // not used
    for (int i = 0; i < N_PARAMS; i++){
        write_next(params[i].desc.name);
    }
    return N_PARAMS;
}

int api_prefix_list(char* prefix,  void (*write_next)(char* prefix_name) ){
    return 0; // not used
}


