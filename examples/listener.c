#include <stdio.h>
#include <stdint.h>
#include "pico-ros.h"
#include "ucdr/microcdr.h"

// Use command line arguments to change default values
#define MODE        "client"
#define LOCATOR     "tcp/192.168.1.16:7447"

// Common utils
extern int picoros_parse_args(int argc, char **argv, picoros_interface_t* ifx);
extern size_t ucdr_deserialize_string_no_copy(ucdrBuffer* ub, char** pstring);

typedef uint32_t cdr_header_t;

// Subscriber callback
void log_callback(uint8_t*, size_t);

// Example Subscriber
picoros_subscriber_t sub_log = {
    .topic = {
        .name = "picoros/chatter",
        .type = "std_msgs::msg::dds_::String",
        .rihs_hash = "df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18",
    },
    .user_callback = log_callback,
};

// Example node
picoros_node_t node = {
    .name = "listener",
    . guid = {0x02},
};

void log_callback(uint8_t* rx_data, size_t data_len){
    ucdrBuffer reader;
    ucdr_init_buffer(&reader, rx_data + sizeof(cdr_header_t), data_len - sizeof(cdr_header_t));
    char* msg = NULL;
    ucdr_deserialize_string_no_copy(&reader, &msg);
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
