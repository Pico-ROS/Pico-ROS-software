#include <stdio.h>
#include <stdint.h>
#include "pico-ros.h"
#include "ucdr/microcdr.h"

// Use command line arguments to change default values
#define MODE        "client"
#define LOCATOR     "tcp/192.168.1.16:7447"

// Common utils
extern int picoros_parse_args(int argc, char **argv, picoros_node_t* node);
extern size_t ucdr_deserialize_string_no_copy(ucdrBuffer* ub, char** pstring);

// Buffer for publication, used from this thread
typedef uint32_t cdr_header_t;
struct {
    cdr_header_t header;
    uint8_t data[1024];
}pub_buf = {.header = 0x0100}; // Little-Endian format OMG-CDR specification.

// Buffer writer
ucdrBuffer pub_writer;

// Example Publisher
picoros_publisher_t pub_log = {
    .topic = {
        .name = "picoros/chatter",
        .type = "std_msgs::msg::dds_::String",
        .rihs_hash = "df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18",
    },
};

// Example node
picoros_node_t node = {
    .name = "talker",
    .mode = MODE,
    .locator = LOCATOR,
    . guid = {0x01},
};

void publish_log(){
    printf("Publishing log...\n");
    ucdr_serialize_string(&pub_writer, "Hello from Pico-ROS!");
    size_t len = ucdr_buffer_length(&pub_writer);
    picoros_publish(&pub_log, (uint8_t*)&pub_buf, len + sizeof(cdr_header_t));
}

int main(int argc, char **argv){
    int ret = picoros_parse_args(argc, argv , &node);

    if(ret != 0){
        return ret;
    }
    printf("Starting Pico-ROS node %s\n"
           "mode:'%s' locator:'%s' domain:%d\n",
           node.name, node.mode, node.locator, node.domain_id);

    ucdr_init_buffer(&pub_writer, pub_buf.data, sizeof(pub_buf.data));
    while (picoros_init(&node) == PICOROS_NOT_READY){
        printf("Waiting RMW init...\n");
        z_sleep_s(1);
    }
    picoros_publisher_declare(&pub_log);

    while(true){
        publish_log();
        z_sleep_s(1);
    }
    return 0;
}
