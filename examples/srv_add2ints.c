#include <stdio.h>
#include <stdint.h>
#include "pico-ros.h"
#include "ucdr/microcdr.h"

// Use command line arguments to change default values
#define MODE        "client"
#define LOCATOR     "tcp/192.168.1.16:7447"

// Common utils
extern int picoros_parse_args(int argc, char **argv,  picoros_interface_t* ifx);

// Service callback
picoros_service_reply_t add2_srv_cb(uint8_t* request, size_t size, void* user_data);

// Buffer for service reply, used from zenoh threads
typedef uint32_t cdr_header_t;
struct {
    cdr_header_t header;
    uint8_t data[1024];
}srv_buf = {.header = 0x0100}; // Little-Endian format OMG-CDR specification.


ucdrBuffer srv_writer;


// Example service
picoros_service_t add2_srv = {
    .topic = {
        .name = "add2",
        .type = "example_interfaces::srv::dds_::AddTwoInts",
        .rihs_hash = "e118de6bf5eeb66a2491b5bda11202e7b68f198d6f67922cf30364858239c81a",
    },
    .user_callback = add2_srv_cb,
};


// Example node
picoros_node_t node = {
    .name = "picoros",
    .guid = {0x03},
};

// Service callback
picoros_service_reply_t add2_srv_cb(uint8_t* rx_data, size_t rx_size, void* user_data){
    int64_t a,b;
    ucdrBuffer reader = {};
    ucdr_init_buffer(&reader, rx_data + sizeof(cdr_header_t) , rx_size - sizeof(cdr_header_t));
    ucdr_deserialize_int64_t(&reader, &a);
    ucdr_deserialize_int64_t(&reader, &b);

    int64_t sum = a + b;

    printf("Service add2(a:%ld, b:%ld) called. Sending reply sum:%ld\n", a, b, sum);

    ucdr_serialize_int64_t(&srv_writer, sum);

    picoros_service_reply_t reply = {
        .length = ucdr_buffer_length(&srv_writer) + sizeof(cdr_header_t),
        .data = (uint8_t*)&srv_buf,
        .free_callback = NULL, // no need to free static buffer
    };

    return reply;
}

int main(int argc, char **argv){
    ucdr_init_buffer(&srv_writer, srv_buf.data, sizeof(srv_buf.data));
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
