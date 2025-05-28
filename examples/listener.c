#include <stdio.h>
#include <stdint.h>
#include "pico-ros.h"
#include "ucdr/microcdr.h"

#define MODE        "client"
#define LOCATOR     "tcp/192.168.1.16:7447"

// Peer mode values (comment/uncomment as needed)
// #define MODE "peer"
// #define LOCATOR "udp/224.0.0.225:7447#iface=en0"

typedef uint32_t cdr_header_t;

void log_callback(uint8_t*, size_t);

picoros_subscriber_t sub_log = {
    .topic = {
        .name = "picoros/chatter",
        .type = "std_msgs::msg::dds_::String",
        .rihs_hash = "df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18",
    },
    .user_callback = log_callback,
};

picoros_node_t node = {
    .name = "listener",
    .mode = MODE,
    .locator = LOCATOR,
    . guid = {0x02},
};

size_t ucdr_deserialize_string_no_copy(ucdrBuffer* ub, char** pstring){
    uint32_t len = 0;
    ucdr_deserialize_endian_uint32_t(ub, ub->endianness, &len);
    *pstring = (char*)ub->iterator;
    ub->iterator += len;
    ub->offset += len;
    ub->last_data_size = 1;
    return len;
}

void log_callback(uint8_t* rx_data, size_t data_len){
    ucdrBuffer reader;
    ucdr_init_buffer(&reader, rx_data + sizeof(cdr_header_t), data_len - sizeof(cdr_header_t));
    char* msg = NULL;
    ucdr_deserialize_string_no_copy(&reader, &msg);
    printf("Subscriber recieved: %s\n", msg);
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

    while (picoros_init(&node) == PICOROS_NOT_READY){
        printf("Waiting RMW init...\n");
        z_sleep_s(1);
    }
    picoros_subscriber_declare(&sub_log);

    while(true){
        z_sleep_s(1);
    }
    return 0;
}
