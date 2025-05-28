#include <stdio.h>
#include <stdint.h>
#include "pico-ros.h"
#include "ucdr/microcdr.h"

// Peer mode values (comment/uncomment as needed)
#define MODE        "client"
#define LOCATOR     "tcp/192.168.1.16:7447"
// #define MODE "peer"
// #define LOCATOR "udp/224.0.0.225:7447#iface=en0"

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
        .name = "picoros/add2",
        .type = "example_interfaces::srv::dds_::AddTwoInts",
        .rihs_hash = "e118de6bf5eeb66a2491b5bda11202e7b68f198d6f67922cf30364858239c81a",
    },
    .user_callback = add2_srv_cb,
};


// Example node
picoros_node_t node = {
    .name = "picoros",
    .mode = MODE,
    .locator = LOCATOR,
    . guid = {0x03},
};

// Service callback
picoros_service_reply_t add2_srv_cb(uint8_t* rx_data, size_t rx_size, void* user_data){
    int64_t a,b;
    ucdrBuffer reader = {};
    ucdr_init_buffer(&reader, rx_data + sizeof(cdr_header_t) , rx_size - sizeof(cdr_header_t));
    ucdr_deserialize_int64_t(&reader, &a);
    ucdr_deserialize_int64_t(&reader, &b);

    int64_t sum = a + b;

    printf("Service add2ints called a:%ld, b:%ld, Sending reply sum:%ld\n", a,b,sum);

    ucdr_serialize_int64_t(&srv_writer, sum);

    picoros_service_reply_t reply = {
        .length = ucdr_buffer_length(&srv_writer) + sizeof(cdr_header_t),
        .data = (uint8_t*)&srv_buf,
    };

    return reply;
}

// arg parsing utility
extern int picoros_parse_args(int argc, char **argv, picoros_node_t* node);

int main(int argc, char **argv){

    int ret = picoros_parse_args(argc, argv , &node);

    if(ret != 0){
        return ret;
    }
    printf("Starting Pico-ROS node %s\n"
           "mode:'%s' locator:'%s' domain:%d\n",
           node.name, node.mode, node.locator, node.domain_id);

    ucdr_init_buffer(&srv_writer, srv_buf.data, sizeof(srv_buf.data));

    while (picoros_init(&node) == PICOROS_NOT_READY){
        printf("Waiting RMW init...\n");
        z_sleep_s(1);
    }
    picoros_service_declare(&add2_srv);

    while(true){
        z_sleep_s(1);
    }
    return 0;
}
