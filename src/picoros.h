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

/* Exported constants --------------------------------------------------------*/
/** @brief Maximum size for key expressions used in topic names @ingroup picoros */
#define KEYEXPR_SIZE 400u
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
    const char* name;                     /**< Topic name */
    const char* type;                     /**< Message type */
    const char* rihs_hash;                /**< RIHS hash */
} rmw_topic_t;

/** @} */

 /**
 * @defgroup service_server Service server
 * @ingroup picoros
 * @{
 */
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
 * @param request_data Pointer to received request data (CDR encoded)
 * @param reqest_size Size of received request
 * @param user_data User data passed during service registration
 * @return Service reply structure containing response data
 */
typedef picoros_service_reply_t (*picoros_service_cb_t)(
            uint8_t*  request_data,
            size_t    reqest_size,
            void*     user_data
            );

/**
 * @brief Service server structure for Pico-ROS
 */
typedef struct {
    z_owned_queryable_t zqable;         /**< Zenoh queryable instance */
    rmw_topic_t         topic;          /**< Topic information */
    rmw_attachment_t    attachment;     /**< RMW attachment data */
    void*              user_data;       /**< User data for callback */
    picoros_service_cb_t user_callback; /**< User callback for service handling */
} picoros_service_t;

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
 * @param rx_data Pointer to received data buffer (CDR encoded)
 * @param data_len Size of received data in bytes
 */
typedef void (*picoros_sub_cb_t)(
            uint8_t* rx_data,
            size_t   data_len
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
picoros_res_t picoros_service_declare(picoros_node_t* node, picoros_service_t* srv);

#ifdef __cplusplus
}
#endif

#endif /* PICO_ROS_H_ */
