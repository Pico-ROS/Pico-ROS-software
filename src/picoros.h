/*******************************************************************************
 * @file    picoros.h
 * @brief   Pico-ROS - A lightweight ROS client implementation for resource-constrained devices
 * @date    2025-May-27
 *
 * @details This header file provides the core functionality for Pico-ROS, including:
 *          - Node management
 *          - Publisher/Subscriber communication
 *          - Service server implementation
 *
 * @copyright Copyright (c) 2025 Ubiquity Robotics
 *******************************************************************************/

#ifndef PICO_ROS_H_
#define PICO_ROS_H_

#include "zenoh-pico.h"

#ifdef __cplusplus
 extern "C" {
#endif

 /**
 * @defgroup picoros picoros
 * @{
 */

/** @} */

/* Exported includes ---------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>

/* Exported constants --------------------------------------------------------*/
/** @brief Maximum size for key expressions used in rmw topic names @ingroup picoros */
#define KEYEXPR_SIZE 400u
/** @brief Maximum size for fully_qualified_name @ingroup picoros */
#define TOPIC_MAX_NAME 124u
/** @brief Size of RMW GID (Global Identifier) */
#define RMW_GID_SIZE 16u
/** @brief Flag to enable/disable node GUID usage @ingroup picoros*/
#define USE_NODE_GUID 0

/* Exported types ------------------------------------------------------------*/

/**
 * @defgroup rmw RMW support
 * @ingroup picoros
 * @{
 */

/**
 * @brief RMW attachment structure required by rmw_zenoh
 */
typedef struct __attribute__((__packed__)) {
    int64_t  sequence_number;       /**< Message sequence number */
    int64_t  time;                  /**< Timestamp */
    uint8_t  rmw_gid_size;          /**< Size of RMW GID */
    uint8_t  rmw_gid[RMW_GID_SIZE]; /**< RMW Global Identifier */
} rmw_attachment_t;

/**
 * @brief RMW topic structure required by rmw_zenoh
 */
typedef struct {
    const char* name;               /**< Topic name */
    const char* type;               /**< Message type */
    const char* rihs_hash;          /**< RIHS hash */
} rmw_topic_t;

/** @} */

/**
 * @defgroup service_server Service server
 * @ingroup picoros
 * @{
 */

/* Forward declaration */
struct picoros_srv_server_s;

/**
 * @brief Service reply data structure
 */
typedef void (*f_free)(void*);
typedef struct {
    uint8_t* data;                  /**< Pointer to service reply data */
    size_t   length;                /**< Length of service reply */
    f_free   free_callback;         /**< Function to call when freeing reply data (can be NULL) */
} picoros_service_reply_t;

/**
* @brief Callback function type for service request handling
* @return Service reply structure containing response data
*/
typedef picoros_service_reply_t (*picoros_srv_server_cb_t)(
    struct picoros_srv_server_s* server,         /**< Pointer to server instance */
    uint8_t*                     request_data,   /**< Pointer to received request data */
    size_t                       reqest_size     /**< Request data size */
);

/**
 * @brief Service server structure for Pico-ROS
 */
typedef struct picoros_srv_server_s{
    z_owned_queryable_t      zqable;         /**< Zenoh queryable instance */
    rmw_topic_t              topic;          /**< Topic information */
    rmw_attachment_t         attachment;     /**< RMW attachment data */
    void*                    user_data;      /**< User data, not used by picoros */
    picoros_srv_server_cb_t  user_callback;  /**< User callback for service handling */
} picoros_srv_server_t;

/** @} */


/**
 * @defgroup service_client Service client
 * @ingroup picoros
 * @{
 */

/* Forward declaration */
struct picoros_srv_client_s;

/**
 * @brief Callback function type for service reply handling
 * @return void
 */
typedef void (*picoros_srv_client_cb_t)(
    struct picoros_srv_client_s*    client,         /**< Pointer to client starting the request */
    uint8_t*                        reply_data,     /**< Pointer to received reply data (CDR encoded) */
    size_t                          reply_size,     /**< Size of received reply */
    bool                            error           /**< Received error reply */
);

/**
 * @brief Callback function type for service reply dropped handling
 * @param client picoros_srv_client_t that started the request
 * @return void
 */
typedef void (*picoros_srv_client_drop_cb_t)( struct picoros_srv_client_s* client);


/**
* @brief Service client structure for Pico-ROS
 */
typedef struct picoros_srv_client_s{
    char*                         node_name;             /**< Node name of service server */
    uint32_t                      node_domain_id;        /**< Domain ID of service server */
    rmw_topic_t                   topic;                 /**< Topic information */
    picoros_srv_client_cb_t       user_callback;         /**< User callback for service reply handling. Called if reply is received. */
    picoros_srv_client_drop_cb_t  drop_callback;         /**< User callback for service call drop handling. Called for every service call.*/
    bool                          _in_progress;          /**< Private flag used to limit to one ongoing request */
    z_get_options_t*              opts;                  /**< Request options, if NULL default options are used */
    void*                         user_data;             /**< User data, not used by picoros */
    z_view_keyexpr_t              ke;                    /**< Precomputed when creating the client */
    char*                         _key_buf;              /**< Private buffer for key expresion */
} picoros_srv_client_t;

/** @} */

/**
 * @brief Publisher structure for Pico-ROS @ingroup picoros
 */
typedef struct {
    z_owned_publisher_t zpub;       /**< Zenoh publisher instance */
    rmw_attachment_t   attachment;  /**< RMW attachment data */
    rmw_topic_t        topic;       /**< Topic information */
    z_publisher_options_t opts;     /**< Topic options, if NULL default options are used */
} picoros_publisher_t;

/** @} */


/**
 * @defgroup subscriber Subscriber
 * @ingroup picoros
 * @{
 */

/**
 * @brief Callback function type for subscriber data handling
 */
typedef void (*picoros_sub_cb_t)(
            uint8_t* rx_data,   /**< Pointer to received data buffer (CDR encoded) */
            size_t   data_len   /**< Size of received data in bytes */
            );

/**
 * @brief Subscriber structure for Pico-ROS
 */
typedef struct {
    z_owned_subscriber_t zsub;         /**< Zenoh subscriber instance */
    rmw_topic_t         topic;         /**< Topic information */
    picoros_sub_cb_t    user_callback; /**< User callback for data handling */
} picoros_subscriber_t;

/** @} */


/**
 * @defgroup node Node
 * @ingroup picoros
 * @{
 */
/**
 * @brief Node configuration structure
 */
typedef struct {
    const char* name;                  /**< Node name */
    uint32_t    domain_id;             /**< ROS domain ID */
    uint8_t     guid[RMW_GID_SIZE];    /**< Node GUID */
} picoros_node_t;

/** @} */

/**
 * @defgroup interface Network interface
 * @ingroup picoros
 * @{
 */

/**
 * @brief Network interface configuration
 */
typedef struct {
    char* mode;                     /**< Connection mode (peer/client) */
    char* locator;                  /**< Network locator string */
} picoros_interface_t;

/** @} */

/**
 * @brief Result codes for Pico-ROS operations @ingroup picoros
 */
typedef enum {
    PICOROS_OK = 0,                /**< Operation successful */
    PICOROS_ERROR = -1,            /**< Operation failed */
    PICOROS_NOT_READY = -2,        /**< System not ready */
} picoros_res_t;

/* Exported functions --------------------------------------------------------*/

/**
 * @brief Initialize the network interface
 * @param ifx Pointer to interface configuration
 * @return PICOROS_OK on success, error code otherwise
 * @ingroup interface
 */
picoros_res_t picoros_interface_init(picoros_interface_t* ifx);

/**
 * @brief Shutdown the network interface
 * @ingroup interface
 */
void picoros_interface_shutdown(void);

/**
 * @brief Initialize a ROS node
 * @param node Pointer to node configuration
 * @return PICOROS_OK on success, error code otherwise
 * @ingroup node
 */
picoros_res_t picoros_node_init(picoros_node_t* node);

/**
 * @brief Declare a publisher for a node
 * @param node Pointer to node instance
 * @param pub Pointer to publisher configuration
 * @return PICOROS_OK on success, error code otherwise
 * @ingroup publisher
 */
picoros_res_t picoros_publisher_declare(picoros_node_t* node, picoros_publisher_t *pub);

/**
 * @brief Publish data on a topic
 * @param pub Pointer to publisher instance
 * @param payload Pointer to data to publish
 * @param len Length of data in bytes
 * @return PICOROS_OK on success, error code otherwise
 * @ingroup publisher
 */
picoros_res_t picoros_publish(picoros_publisher_t *pub, uint8_t *payload, size_t len);

/**
 * @brief Declare a subscriber for a node
 * @param node Pointer to node instance
 * @param sub Pointer to subscriber configuration
 * @return PICOROS_OK on success, error code otherwise
 * @ingroup subscriber
 */
picoros_res_t picoros_subscriber_declare(picoros_node_t* node, picoros_subscriber_t *sub);

/**
 * @brief Unsubscribe from a topic
 * @param sub Pointer to subscriber instance
 * @return PICOROS_OK on success, error code otherwise
 * @ingroup subscriber
 */
picoros_res_t picoros_unsubscribe(picoros_subscriber_t *sub);

/**
 * @brief Declare a service server for a node
 * @param node Pointer to node instance
 * @param srv Pointer to service configuration
 * @return PICOROS_OK on success, error code otherwise
 * @ingroup service_server
 */
picoros_res_t picoros_service_declare(picoros_node_t* node, picoros_srv_server_t* srv);


/**
 * @brief Initialize service client with precomputed key expression.
 * @param client Pointer to client instance.
 * @return PICOROS_OK on success, error code otherwise
 * @ingroup service_client
 */
picoros_res_t picoros_service_client_init(picoros_srv_client_t * client);


/**
 * @brief Call service using service client.
 * @param client Pointer to client instance. Should be in scope until service call is ongoing.
 * @param payload Pointer to data payload
 * @param len Size of data
 * @return PICOROS_OK on success, PICOROS_NOT_READY when request already progress, error code otherwise
 * @ingroup service_client
 */
picoros_res_t picoros_service_call(picoros_srv_client_t* client, uint8_t* payload, size_t len);

/**
 * @brief Check if client has ongoing service call.
 * @param client Pointer to client instance.
 * @return true if request is in progress
 * @ingroup service_client
 */
bool picoros_service_call_in_progress(picoros_srv_client_t* client);

#ifdef __cplusplus
}
#endif

#endif /* PICO_ROS_H_ */
