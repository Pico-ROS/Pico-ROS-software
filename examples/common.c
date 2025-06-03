/**
  ******************************************************************************
  * @file    common.c
  * @date    2025-May-27
  * @brief   Common utilities for examples
  * Copyright (c) 2025 Ubiquity Robotics
  ******************************************************************************
  */

#include "picoros.h"
#include <stdio.h>
#include <unistd.h>

int picoros_parse_args(int argc, char **argv, picoros_interface_t* ifx) {
    int opt;
    while ((opt = getopt(argc, argv, "a:m:h")) != -1) {
        switch (opt) {
            case 'a':
                // CONFIG_CONNECT_KEY
                ifx->locator = optarg;
                break;
            case 'm':
                // Z_CONFIG_MODE_KEY
                ifx->mode = optarg;
                break;
            case 'h':
                fprintf(stderr,
                    "-m 'mode' ['client', 'peer'] \n"
                    "-a 'address' to connect or listen on (ex: 'tcp/192.168.1.16:7447', 'udp/224.0.0.225:7447#iface=en0')\n"
                );
            case '?':
                if (optopt == 'a' || optopt == 'm') {
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                }
                return 1;
                break;
            default:
                return -1;
                break;

        }
    }
    return 0;
}