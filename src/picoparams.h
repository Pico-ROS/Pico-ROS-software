/*******************************************************************************
 * @file    picoparams.h
 * @brief   Pico-ROS Parameter Server Implementation
 * @date    2025-Jun-01
 * 
 * @details This module implements a ROS 2 compatible parameter server for embedded systems.
 * 
 * @copyright Copyright (c) 2025 Ubiquity Robotics
 *******************************************************************************/

#ifndef PICOPARAMS_H_
#define PICOPARAMS_H_

#ifdef __cplusplus
 extern "C" {
#endif


 /**
 * @defgroup picoparams picoparams
 * @{
 */
/** @} */

/* Exported includes ---------------------------------------------------------*/
#include "picoros.h"
#include "picoserdes.h"

/* Exported types ------------------------------------------------------------*/
/** @brief Maximum number of strings in a single parameter request
 * (prefixes for list, parameters for get/set)
 * @ingroup picoparams */
#define PP_MAX_REQUEST_STRINGS 50


 /**
 * @defgroup ros_parameter ROS Parameter types
 * @ingroup picoparams
 * @{
 */

/**
 * @brief ROS parameter types
 */
typedef enum {
    PARAMETER_NOT_SET,        /**< Parameter not set */
    PARAMETER_BOOL,           /**< Boolean parameter */
    PARAMETER_INTEGER,        /**< Integer parameter */
    PARAMETER_DOUBLE,         /**< Double parameter */
    PARAMETER_STRING,         /**< String parameter */
    PARAMETER_BYTE_ARRAY,     /**< Byte array parameter */
    PARAMETER_BOOL_ARRAY,     /**< Boolean array parameter */
    PARAMETER_INTEGER_ARRAY,  /**< Integer array parameter */
    PARAMETER_DOUBLE_ARRAY,   /**< Double array parameter */
    PARAMETER_STRING_ARRAY,   /**< String array parameter */
} pp_ParameterType;

/**
 * @brief Floating point range descriptor for parameters
 */
typedef struct {
    double min;   /**< Minimum allowed value */
    double max;   /**< Maximum allowed value */
    double step;  /**< Step size for value changes */
} pp_FloatingPointRange;

/**
 * @brief Integer range descriptor for parameters
 */
typedef struct {
    int64_t min;   /**< Minimum allowed value */
    int64_t max;   /**< Maximum allowed value */
    int64_t step;  /**< Step size for value changes */
} pp_IntegerRange;

/**
 * @brief Parameter value container
 */
typedef struct {
    uint8_t type;           /**< Parameter type (ros_ParameterType) */
    union {
        bool        val_bool;        /**< Boolean value */
        int64_t     val_int;         /**< Integer value */
        double      val_double;      /**< Double value */
        char*       val_string;      /**< String value */
        uint8_t*    val_bytearray;   /**< Byte array value */
        bool*       val_boolarray;   /**< Boolean array value */
        int64_t*    val_intarray;    /**< Integer array value */
        double*     val_doublearray; /**< Double array value */
    };
    uint32_t length;  /**< Length of data (>1 for arrays) */

    /**
     * @brief User function to serialize [n] part of data to buffer
     * @note If not set, data from value union is used for serialization
     */
    void (*write_data_n)(ucdrBuffer* writer, void* user_data, uint32_t n);
    void* user_data;  /**< User data for write_data_n callback */
} pp_ParameterValue;

/**
 * @brief ROS parameter with name and value
 */
typedef struct {
    char* name;              /**< Parameter name */
    pp_ParameterValue value; /**< Parameter value */
} pp_Parameter;

/**
 * @brief Parameter descriptor containing metadata
 */
typedef struct {
    char* name;                    /**< Parameter name */
    pp_ParameterType type:8;      /**< Parameter type */
    char* description;             /**< Parameter description */
    char* additional_constraints;  /**< Additional constraints as string */
    bool read_only;                /**< Whether parameter is read-only */
    bool dynamic_typing;           /**< Whether parameter type can change */
    union {
        pp_FloatingPointRange float_range; /**< Floating point range constraints */
        pp_IntegerRange int_range;         /**< Integer range constraints */
    };
} pp_ParameterDescriptor;

/** @} */


 /**
 * @defgroup picoparams_interface Parameter server interface
 * @ingroup picoparams_server
 * @{
 */

/**
 * @brief Function to get parameter reference from full path
 * @param name Full parameter path
 * @return Opaque parameter reference
 */
typedef void* (*f_param_ref)(char* name);

/**
 * @brief Function to get parameter descriptor
 * @param param Parameter reference
 * @return Parameter descriptor
 */
typedef pp_ParameterDescriptor (*f_param_describe)(void* param);

/**
 * @brief Function to get parameter value
 * @param param Parameter reference
 * @return Parameter value
 */
typedef pp_ParameterValue (*f_param_get)(void* param);

/**
 * @brief Function to get parameter type
 * @param param Parameter reference
 * @return Parameter type
 */
typedef pp_ParameterType (*f_param_type)(void* param);

/**
 * @brief Function to set parameter value
 * @param param Parameter reference
 * @param value New parameter value
 * @param error_msg Error message if set fails
 * @return true if set successful, false otherwise
 */
typedef bool (*f_param_set)(void* param, pp_ParameterValue* value, char** error_msg);

/**
 * @brief Function to list parameters at a prefix
 * @param prefix Parameter prefix to list
 * @param write_next Callback for each parameter found
 * @return Number of parameters found
 */
typedef int (*f_param_list)(char* prefix, void (*write_next)(char* param_name));

/**
 * @brief Function to list parameter prefixes
 * @param prefix Prefix to list
 * @param write_next Callback for each prefix found
 * @return Number of prefixes found
 */
typedef int (*f_prefix_list)(char* prefix, void (*write_next)(char* prefix_name));

/**
 * @brief Parameter server interface
 */
typedef struct {
    f_param_ref f_ref;              /**< Get parameter reference */
    f_param_describe f_describe;    /**< Get parameter descriptor */
    f_param_get f_get;              /**< Get parameter value */
    f_param_type f_type;            /**< Get parameter type */
    f_param_set f_set;              /**< Set parameter value */
    f_param_list f_list;            /**< List parameters */
    f_prefix_list f_prefixes;       /**< List parameter prefixes */
    uint8_t* reply_buf;             /**< Reply buffer */
    uint32_t reply_buf_size;        /**< Reply buffer size */
} picoparams_interface_t;

/** @} */

/**
 * @brief Initialize parameter server
 * @param server Parameter server instance
 * @param node ROS node
 * @param ifx Parameter interface
 * @return PICOROS_OK on success, error code otherwise
 */
picoros_res_t picoparams_init(picoros_node_t* node, picoparams_interface_t ifx);


/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* PICOPARAMS_H_ */
