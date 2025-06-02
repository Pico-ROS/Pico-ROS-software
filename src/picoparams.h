/**
  ******************************************************************************
  * @file    picoparams.h
  * @date    2025-Jun-01
  * @brief   Pico parameter server
  ******************************************************************************
  */
#ifndef PICOPARAMS_H_
#define PICOPARAMS_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Exported includes ---------------------------------------------------------*/
#include "picoros.h"
#include "picoserdes.h"
/* Exported types ------------------------------------------------------------*/
#define MAX_REQUEST_STRINGS 50
typedef enum{
    PARAMETER_NOT_SET,
    PARAMETER_BOOL,
    PARAMETER_INTEGER,
    PARAMETER_DOUBLE,
    PARAMETER_STRING,
    PARAMETER_BYTE_ARRAY,
    PARAMETER_BOOL_ARRAY,
    PARAMETER_INTEGER_ARRAY,
    PARAMETER_DOUBLE_ARRAY,
    PARAMETER_STRING_ARRAY,
}ros_ParameterType;

typedef struct{
    double min;
    double max;
    double step;
}ros_FloatingPointRange;

typedef struct{
    int64_t min;
    int64_t max;
    int64_t step;
}ros_IntegerRange;

typedef struct{
    uint8_t  type;           // ros_ParameterType
    union{
        bool        val_bool;
        int64_t     val_int;
        double      val_double;
        char*       val_string;
        uint8_t*    val_bytearray;
        bool*       val_boolarray;
        int64_t*    val_intarray;
        double*     val_doublearray;
    };
    uint32_t length;  // length of data > 1 for array

    // User function called to serialize [n] part of data to buffer.
    // If left unset data from value union is used for serialization.
    void     (*write_data_n)(ucdrBuffer* writer, void* user_data, uint32_t n);
    void*    user_data;

}ros_ParameterValue;

typedef struct{
    char*                  name;
    ros_ParameterValue     value;
}ros_Parameter;

typedef struct{
    char*             name;
    ros_ParameterType type:8;
    char*             description;
    char*             additional_constraints;
    bool              read_only;
    bool              dynamic_typing;
    union{
        ros_FloatingPointRange  float_range;
        ros_IntegerRange        int_range;
    };
}ros_ParameterDescriptor;

/* Function to return parameter reference from full path. */
typedef void* (*f_param_ref)(char* name);

/* Function to return ros_ParameterDescriptor from parameter reference. */
typedef ros_ParameterDescriptor (*f_param_describe)(void* param);

/* Function to return ros_ParameterValue from parameter reference. */
typedef ros_ParameterValue (*f_param_get)(void* param);

/* Function to return ros_ParameterType from parameter reference. */
typedef ros_ParameterType (*f_param_type)(void* param);

/* Function to set ros_ParameterValue to parmeter reference.
 * return true if set if successful, false if failed.
 * If false is returned error message can be set.
 */
typedef bool (*f_param_set)(void* param, ros_ParameterValue* value, char** error_msg);

/* Function to list all parameters at provided prefix.
 * write_next should be called with each new parameter full path.
 * Return total number of parameters written.
 */
typedef int (*f_param_list)(char* prefix,  void (*write_next)(char* param_name) );

/* Function to list all prefixes at provided prefix.
 * write_next should be called with each new prefix.
 * Return total number of orefixes written.
 */
typedef int (*f_prefix_list)(char* prefix,  void (*write_next)(char* prefix_name) );

typedef struct{
    f_param_ref            f_ref;
    f_param_describe       f_describe;
    f_param_get            f_get;
    f_param_type           f_type;
    f_param_set            f_set;
    f_param_list           f_list;
    f_prefix_list          f_prefixes;
    uint8_t*               reply_buf;
    uint32_t               reply_buf_size;
}picoparams_interface_t;

typedef struct{
    picoparams_interface_t interface;
    picoros_service_t      get_srv;
    picoros_service_t      list_srv;
    picoros_service_t      set_srv;
    picoros_service_t      describe_srv;
    picoros_service_t      get_types_srv;
    picoros_service_t      set_atomic_srv;
    ucdr_writer_t*         current_writer;
}picoparams_server_t;

picoros_res_t picoparams_init(picoparams_server_t* server, picoros_node_t* node, picoparams_interface_t ifx);

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* PICOPARAMS_H_ */
