/**
  ******************************************************************************
  * @file    picoros.h
  * @date    2025-May-27
  * @brief   Pico-ROS
  ******************************************************************************
  */
#ifndef PICO_ROS_H_
#define PICO_ROS_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Exported includes ---------------------------------------------------------*/
#include <stdint.h>
#include "zenoh-pico.h"
/* Exported constants --------------------------------------------------------*/
#define KEYEXPR_SIZE 400u
#define RMW_GID_SIZE 16u
#define USE_NODE_GUID 0
/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* Subscribe callback */
/* Function called when new data arrives on subscribed topic. */
typedef void (*picoros_sub_cb_t)(
            uint8_t* rx_data,       // pointer to recieved data buffer. CDR encoded.
            size_t   data_len       // size of recieved data
            );


/* Service call callback */
typedef void (*f_free)(void*);
typedef struct{
    uint8_t* data;                  // pointer to service reply data
    size_t   length;                // length of service reply
    f_free   free_callback;         // function to call when freeing reply data (can be NULL)
}picoros_service_reply_t;
/* Function called when new service call arrives for registered service.*/
typedef picoros_service_reply_t (*picoros_service_cb_t)(
            uint8_t*  request_data,  // pointer to recieved data buffer. CDR encoded.
            size_t    reqest_size,   // size of recieved data
            void*     user_data      // user data passed when registering the service
            );


/* RWM attchment expected by rmw_zenoh */
typedef struct __attribute__((__packed__)){
    int64_t                 sequence_number;
    int64_t                 time;
    uint8_t                 rmw_gid_size;
    uint8_t                 rmw_gid[RMW_GID_SIZE];
}rmw_attachment_t;

/* RMW topic expected by rmw_zenoh */
typedef struct{
    char*                   name;
    char*                   type;
    char*                   rihs_hash;
}rmw_topic_t;


/* pico-ros types */
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
    uint32_t                domain_id;
    uint8_t                 guid[RMW_GID_SIZE];
}picoros_node_t;

typedef struct{
    char*                   mode;
    char*                   locator;
}picoros_interface_t;

typedef enum{
    PICOROS_OK    =  0,
    PICOROS_ERROR = -1,
    PICOROS_NOT_READY = -2,
}picoros_res_t;


/* Exported functions --------------------------------------------------------*/
picoros_res_t picoros_interface_init(picoros_interface_t* ifx);
void          picoros_interface_shutdown();

picoros_res_t picoros_node_init(picoros_node_t* node);

picoros_res_t picoros_publisher_declare(picoros_node_t* node, picoros_publisher_t *pub);
picoros_res_t picoros_publish(picoros_publisher_t *pub, uint8_t *payload, size_t len);

picoros_res_t picoros_subscriber_declare(picoros_node_t* node, picoros_subscriber_t *sub);
picoros_res_t picoros_unsubscribe(picoros_subscriber_t *sub);

picoros_res_t picoros_service_declare(picoros_node_t* node, picoros_service_t* srv);

// picoros_res_t picoros_service_call();

#ifdef __cplusplus
}
#endif

#endif /* PICO_ROS_H_ */
