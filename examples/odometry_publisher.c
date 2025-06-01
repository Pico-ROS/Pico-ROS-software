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
picoros_publisher_t pub_odo = {
    .topic = {
        .name = "odom",
        .type = ROSTYPE_NAME(ros_Odometry),
        .rihs_hash = ROSTYPE_HASH(ros_Odometry),
    },
};

// Example node
picoros_node_t node = {
    .name = "talker",
};

// Buffer for publication, used from this thread
uint8_t pub_buf[1024];

void publish_odometry(){
    z_clock_t clk = z_clock_now();
    ros_Odometry odom = {
        .header = {
            .frame_id = "odom",
            .time.nsec = clk.tv_nsec,
            .time.sec = clk.tv_sec,
        },
        .child_frame_id = "base-link",
    };
    printf("Publishing odometery...\n");
    size_t len = ps_serialize(pub_buf, &odom, 1024);
    if (len > 0){
        picoros_publish(&pub_odo, pub_buf, len);
    }
    else{
        printf("Odometry message serialization error.");
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

    printf("Declaring publisher on %s\n", pub_odo.topic.name);
    picoros_publisher_declare(&node, &pub_odo);

    while(true){
        publish_odometry();
        z_sleep_s(1);
    }
    return 0;
}
