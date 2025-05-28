/**
  ******************************************************************************
  * @file    pico-ros.h
  * @date    2025-May-27
  * @brief   Description
  ******************************************************************************
  */
#ifndef PICO_ROS_H_
#define PICO_ROS_H_

#include <stddef.h>
#include <sys/types.h>
#ifdef __cplusplus
 extern "C" {
#endif

/* Exported includes ---------------------------------------------------------*/
#include <stdint.h>
#include "zenoh-pico.h"
/* Exported constants --------------------------------------------------------*/
#define KEYEXPR_SIZE 400u
#define RMW_GID_SIZE 16u

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

// typedef uint32_t cdr_header_t;


/* Function called when new data arrives on subscribed topic. */
typedef void (*picoros_sub_cb_t)(
            uint8_t* rx_data,       // pointer to recieved data buffer. CDR encoded.
            size_t   data_len       // size of recieved data
            );


/* Service call helpers */
typedef void (*f_free)(void*);
typedef struct{
    uint8_t* data;
    size_t   length;
    f_free   free_callback;
}picoros_service_reply_t;
/*
 * Function called when new service call arrives for registered service.
 * Return size of tx_data, 0 if no reply
 */

typedef picoros_service_reply_t (*picoros_service_cb_t)(
            uint8_t*  request_data,  // pointer to recieved data buffer. CDR encoded.
            size_t    reqest_size,   // size of recieved data
            void*     user_data      // user data passed when registering the service
            );

// #define PICOROS_SERVICE(_name) size_t _name(uint8_t* rx_data, size_t rx_size, uint8_t** tx_data, f_free* pf_free, void* user_data)


typedef struct __attribute__((__packed__)){
    int64_t                 sequence_number;
    int64_t                 time;
    uint8_t                 rmw_gid_size;
    uint8_t                 rmw_gid[RMW_GID_SIZE];
}rmw_attachment_t;

typedef struct{
    char*                   name;
    char*                   type;
    char*                   rihs_hash;
}rmw_topic_t;

typedef struct{
    z_owned_publisher_t     zpub;
    rmw_attachment_t        attachment;
    rmw_topic_t             topic;
}picoros_publisher_t;

typedef struct{
    z_owned_subscriber_t    zsub;
    rmw_topic_t             topic;
    picoros_sub_cb_t        user_callback;
}picoros_subscriber_t;

typedef struct{
    z_owned_queryable_t     zqable;
    rmw_topic_t             topic;
     rmw_attachment_t       attachment;
     void*                  user_data;
     picoros_service_cb_t   user_callback;
}picoros_service_t;


typedef struct{
    char*                   name;
    char*                   mode;
    char*                   locator;
    uint32_t                domain_id;
    uint8_t                 guid[RMW_GID_SIZE];
}picoros_node_t;

typedef enum{
    PICOROS_OK    =  0,
    PICOROS_ERROR = -1,
    PICOROS_NOT_READY = -2,
}picoros_res_t;


/* Exported functions --------------------------------------------------------*/
picoros_res_t picoros_init(picoros_node_t* node);
void          picoros_shutdown();

picoros_res_t picoros_publisher_declare(picoros_publisher_t *pub);
picoros_res_t picoros_publish(picoros_publisher_t *pub, uint8_t *payload, size_t len);

picoros_res_t picoros_subscriber_declare(picoros_subscriber_t *sub);
picoros_res_t picoros_unsubscribe(picoros_subscriber_t *sub);

picoros_res_t picoros_service_declare(picoros_service_t* srv);


// z_result_t picoros_service_call();


// z_result_t zenoh_query(
//         char* key,
//         z_get_options_t* opts,
//         _z_closure_reply_callback_t reply_cb,
//         z_closure_drop_callback_t drop_cb,
//         uint8_t* data, size_t size);
//
// z_result_t zenoh_queryable_reply(picoros_service_t* q, z_loaned_query_t *query, z_owned_bytes_t* payload, int64_t sequence);
//

int picoros_parse_args(int argc, char **argv, picoros_node_t* node);

#ifdef __cplusplus
}
#endif

#endif /* PICO_ROS_H_ */
