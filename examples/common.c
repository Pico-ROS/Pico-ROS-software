#include "pico-ros.h"
#include <stdio.h>
#include <unistd.h>
#include "ucdr/microcdr.h"

size_t ucdr_deserialize_string_no_copy(ucdrBuffer* ub, char** pstring){
    uint32_t len = 0;
    ucdr_deserialize_endian_uint32_t(ub, ub->endianness, &len);
    *pstring = (char*)ub->iterator;
    ub->iterator += len;
    ub->offset += len;
    ub->last_data_size = 1;
    return len;
}

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
                // fprintf(stderr,
                //     "-m 'mode' ['client', 'peer'] \n"
                //     "-a 'address' to connect or listen on (ex: 'tcp/192.168.1.16:7447', 'udp/224.0.0.225:7447#iface=en0')\n"
                // );
                return -1;
                break;

        }
    }
    return 0;
}