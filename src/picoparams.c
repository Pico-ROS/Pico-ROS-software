/*******************************************************************************
 * @file    picoparams.c
 * @brief   Pico-ROS Parameter Server Implementation
 * @date    2025-Jun-01
 * 
 * @details This file implements the ROS 2 parameter server functionality,
 *          providing parameter storage, retrieval, and manipulation capabilities.
 * 
 * @copyright Copyright (c) 2025 Ubiquity Robotics
 *******************************************************************************/

/* Private includes ----------------------------------------------------------*/
#include "picoparams.h"
/* Private typedef -----------------------------------------------------------*/

typedef struct {
    picoparams_interface_t interface;    /**< Parameter interface */
    picoros_service_t get_srv;           /**< Get parameter service */
    picoros_service_t list_srv;          /**< List parameters service */
    picoros_service_t set_srv;           /**< Set parameter service */
    picoros_service_t describe_srv;      /**< Describe parameter service */
    picoros_service_t get_types_srv;     /**< Get parameter types service */
    picoros_service_t set_atomic_srv;    /**< Set atomic parameter service */
    ucdr_writer_t* current_writer;       /**< Current writer context */
} picoparams_server_t;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
picoros_service_reply_t params_server_handler(uint8_t* request_data, size_t request_size, void* user_data);

/* Private variables ---------------------------------------------------------*/
/* Parameter topic names and hashes */
picoparams_server_t pserver = {
    .get_srv = {
        .topic = {
            .name = "get_parameters",
            .type = "rcl_interfaces::srv::dds_::GetParameters",
            .rihs_hash = "bf9803d5c74cf989a5de3e0c2e99444599a627c7ff75f97b8c05b01003675cbc",
        },
        .user_callback = params_server_handler,
        .user_data = &pserver.get_srv,
    },
    .list_srv = {
        .topic = {
            .name = "list_parameters",
            .type = "rcl_interfaces::srv::dds_::ListParameters",
            .rihs_hash = "3e6062bfbb27bfb8730d4cef2558221f51a11646d78e7bb30a1e83afac3aad9d",
        },
        .user_callback = params_server_handler,
        .user_data = &pserver.list_srv,
    },
    .set_srv = {
        .topic = {
            .name = "set_parameters",
            .type = "rcl_interfaces::srv::dds_::SetParameters",
            .rihs_hash = "56eed9a67e169f9cb6c1f987bc88f868c14a8fc9f743a263bc734c154015d7e0",
        },
        .user_callback = params_server_handler,
        .user_data = &pserver.set_srv,
    },
    .describe_srv = {
        .topic = {
            .name = "describe_parameters",
            .type = "rcl_interfaces::srv::dds_::DescribeParameters",
            .rihs_hash = "845b484d71eb0673dae682f2e3ba3c4851a65a3dcfb97bddd82c5b57e91e4cff",
        },
        .user_callback = params_server_handler,
        .user_data = &pserver.describe_srv,
    },
    .set_atomic_srv = {
        .topic = {
            .name = "set_parameters_atomically",
            .type = "rcl_interfaces::srv::dds_::SetParametersAtomically",
            .rihs_hash = "0e192ef259c07fc3c07a13191d27002222e65e00ccec653ca05e856f79285fcd",
        },
        .user_callback = params_server_handler,
        .user_data = &pserver.set_atomic_srv,
    },
    .get_types_srv = {
        .topic = {
            .name = "get_parameter_types",
            .type = "rcl_interfaces::srv::dds_::GetParameterTypes",
            .rihs_hash = "da199c878688b3e530bdfe3ca8f74cb9fa0c303101e980a9e8f260e25e1c80ca",
        },
        .user_callback = params_server_handler,
        .user_data = &pserver.get_types_srv,
    },
};

/* Public functions ----------------------------------------------------------*/

picoros_res_t picoparams_init(picoros_node_t* node, picoparams_interface_t ifx){
    #define SET_NOT_NULL_OR_RETURN(var, set) {if (set != 0){ var=set; } else{ return PICOROS_ERROR; }}

    if (node == NULL){ return PICOROS_ERROR; }
    SET_NOT_NULL_OR_RETURN(pserver.interface.f_ref, ifx.f_ref);
    SET_NOT_NULL_OR_RETURN(pserver.interface.f_get, ifx.f_get);
    SET_NOT_NULL_OR_RETURN(pserver.interface.f_set, ifx.f_set);
    SET_NOT_NULL_OR_RETURN(pserver.interface.f_describe, ifx.f_describe);
    SET_NOT_NULL_OR_RETURN(pserver.interface.f_type, ifx.f_type);
    SET_NOT_NULL_OR_RETURN(pserver.interface.f_prefixes, ifx.f_prefixes);
    SET_NOT_NULL_OR_RETURN(pserver.interface.f_list, ifx.f_list);
    SET_NOT_NULL_OR_RETURN(pserver.interface.reply_buf, ifx.reply_buf);
    SET_NOT_NULL_OR_RETURN(pserver.interface.reply_buf_size, ifx.reply_buf_size);

    // declare needed services
    if(picoros_service_declare(node, &pserver.get_srv) != PICOROS_OK){ return PICOROS_ERROR; }
    if(picoros_service_declare(node, &pserver.list_srv) != PICOROS_OK){ return PICOROS_ERROR; }
    if(picoros_service_declare(node, &pserver.set_srv) != PICOROS_OK){ return PICOROS_ERROR; }
    if(picoros_service_declare(node, &pserver.describe_srv) != PICOROS_OK){ return PICOROS_ERROR; }
    if(picoros_service_declare(node, &pserver.set_atomic_srv) != PICOROS_OK){ return PICOROS_ERROR; }
    if(picoros_service_declare(node, &pserver.get_types_srv) != PICOROS_OK){ return PICOROS_ERROR; }

    return PICOROS_OK;

}


/* Private functions ---------------------------------------------------------*/
static size_t serialize_ros_ParameterValue(ucdrBuffer* writer, ros_ParameterValue* val){

#define serialize_value(TYPE, VAL, func)                        \
    if (val->type == TYPE){                                     \
        if (val->write_data_n){/* user providede function */    \
            val->write_data_n(writer, val->user_data, 0);       \
        }                                                       \
        else{ func(writer, VAL); }                              \
    }                                                           \
    else{                                                       \
        func(writer, 0);                                        \
    }

#define serialize_array(TYPE, VAL, func)                        \
    if (val->type == TYPE){                                     \
        if (val->write_data_n){/* user providede function */    \
            ucdr_serialize_uint32_t(writer, val->length);       \
            for(int i = 0; i < val->length; i++){               \
                val->write_data_n(writer, val->user_data, i);   \
            }                                                   \
        }                                                       \
        else{ func(writer, VAL, val->length); }                 \
    }                                                           \
    else{                                                       \
        ucdr_serialize_uint32_t(writer, 0);                     \
    }

    // serialization  -------------------------------------------------------
    ucdr_serialize_uint8_t(writer, val->type);

    serialize_value(PARAMETER_BOOL, val->val_bool, ucdr_serialize_bool);
    serialize_value(PARAMETER_INTEGER, val->val_int, ucdr_serialize_int64_t);
    serialize_value(PARAMETER_DOUBLE, val->val_double, ucdr_serialize_double);
    serialize_value(PARAMETER_STRING, val->val_string, ucdr_serialize_rstring);

    serialize_array(PARAMETER_BYTE_ARRAY, val->val_bytearray, ucdr_serialize_sequence_uint8_t);
    serialize_array(PARAMETER_BOOL_ARRAY, val->val_boolarray, ucdr_serialize_sequence_bool);
    serialize_array(PARAMETER_INTEGER_ARRAY, val->val_intarray, ucdr_serialize_sequence_int64_t);
    serialize_array(PARAMETER_DOUBLE_ARRAY, val->val_doublearray, ucdr_serialize_sequence_double);
    ucdr_serialize_uint32_t(writer, 0); // string sequence

    return ucdr_buffer_length(writer);
#undef serialize_value
#undef serialize_array

}

static void deserialize_ros_ParameterValue(ucdrBuffer* reader, ros_ParameterValue* val){
    bool    d_bool;
    int64_t d_int;
    double  d_double;
    uint32_t size;
    val->length = 1;
    ucdr_deserialize_uint8_t(reader, &val->type);

    if (val->type == PARAMETER_NOT_SET){
        return;
    }

    ucdr_deserialize_bool(reader, &d_bool);
    ucdr_deserialize_int64_t(reader, &d_int);
    ucdr_deserialize_double(reader, &d_double);
    if (val->type == PARAMETER_BOOL){
        val->val_bool = d_bool;
        return;
    }
    if (val->type == PARAMETER_INTEGER){
        val->val_int = d_int;
        return;
    }
    if (val->type == PARAMETER_DOUBLE){
        val->val_double = d_double;
        return;
    }

    ucdr_deserialize_rstring(reader, &val->val_string);

    if (val->type == PARAMETER_BYTE_ARRAY){
        ucdr_deserialize_uint32_t(reader, &val->length);
        // default format is LE so we can just take pointers
        val->val_bytearray = reader->iterator;
    }
    else{
        ucdr_deserialize_uint32_t(reader, &size);
    }
    if (val->type == PARAMETER_BOOL_ARRAY){
        ucdr_deserialize_uint32_t(reader, &val->length);
        val->val_boolarray = (bool*)reader->iterator;
    }
    else{
        ucdr_deserialize_uint32_t(reader, &size);
    }
    if (val->type == PARAMETER_INTEGER_ARRAY){
        ucdr_deserialize_uint32_t(reader, &val->length);
        val->val_intarray = (int64_t*)reader->iterator;
    }
    else{
        ucdr_deserialize_uint32_t(reader, &size);
    }
    if (val->type == PARAMETER_DOUBLE_ARRAY){
        ucdr_deserialize_uint32_t(reader, &val->length);
        val->val_doublearray = (double*)reader->iterator;
    }
    else{
        ucdr_deserialize_uint32_t(reader, &size);
    }
//    if (val->type == PARAMETER_STRING_ARRAY){
//    }
//    else{
//        ucdr_deserialize_uint32_t(reader, &size);
//    }
}

static size_t serialize_rcl_ParameterDescriptor(ucdrBuffer* writer, ros_ParameterDescriptor* pd){
    ucdr_serialize_rstring(writer, pd->name);
    ucdr_serialize_uint8_t(writer, pd->type);
    ucdr_serialize_rstring(writer, pd->description);
    ucdr_serialize_rstring(writer, pd->additional_constraints);
    ucdr_serialize_bool(writer, pd->read_only);
    ucdr_serialize_bool(writer, pd->dynamic_typing);
    if (pd->type == PARAMETER_DOUBLE){
        ucdr_serialize_uint32_t(writer, 1);
        ucdr_serialize_double(writer, pd->float_range.min);
        ucdr_serialize_double(writer, pd->float_range.max);
        ucdr_serialize_double(writer, pd->float_range.step);
    }
    else{
        ucdr_serialize_uint32_t(writer, 0);
    }
    if (pd->type == PARAMETER_INTEGER){
        ucdr_serialize_uint32_t(writer, 1);
        pd->int_range.min = pd->int_range.min;
        pd->int_range.max = pd->int_range.max;

        ucdr_serialize_int64_t(writer, pd->int_range.min);
        ucdr_serialize_int64_t(writer, pd->int_range.max);
        ucdr_serialize_int64_t(writer, pd->int_range.step);
    }
    else{
        ucdr_serialize_uint32_t(writer, 0);
    }

    return ucdr_buffer_length(writer);
}

void string_stream_writer(char* string){
    if (string == NULL) {return;}
    ucdr_serialize_rstring(pserver.current_writer->p_buffer, string);
}

size_t list_params(picoparams_server_t* server, ucdrBuffer* reader, ucdrBuffer* writer){
    char* prefixes[PP_MAX_REQUEST_STRINGS];
    uint64_t depth = 0;
    uint32_t n_prefixes = 0;
    ucdr_deserialize_sequence_rstring(reader, prefixes, PP_MAX_REQUEST_STRINGS, &n_prefixes);
    ucdr_deserialize_uint64_t(reader, &depth);

    if (n_prefixes == 0){
        n_prefixes = 1; // list all params
        prefixes[0] = NULL;
    }
    // init string list writer
    uint32_t total_params = 0;
    ucdr_writer_t wr_params = ucdr_seq_start(writer);
    pserver.current_writer = &wr_params;
    for (int i = 0; i < n_prefixes; i++){
        // for all given prefixes
        total_params += pserver.interface.f_list(prefixes[i], string_stream_writer);
    }
    ucdr_seq_set_size(&wr_params, total_params);
    ucdr_seq_end(&wr_params);

    uint32_t total_prefixes = 0;
    ucdr_writer_t wr_prefix = ucdr_seq_start(writer);
    pserver.current_writer = &wr_prefix;
    for (int i = 0; i < n_prefixes; i++){
        // for all given prefixes
        total_prefixes += pserver.interface.f_prefixes(prefixes[i], string_stream_writer);
    }
    ucdr_seq_set_size(&wr_prefix, total_prefixes);
    ucdr_seq_end(&wr_prefix);

    return (ucdr_buffer_length(writer));
}

static size_t get_params_value(picoparams_server_t* server, ucdrBuffer* reader, ucdrBuffer* writer){
    char* target_params[PP_MAX_REQUEST_STRINGS];
    uint32_t n_targets = 0;
    ucdr_deserialize_sequence_rstring(reader, target_params, PP_MAX_REQUEST_STRINGS, &n_targets);

    ucdr_serialize_uint32_t(writer, n_targets);
    for (int i = 0; i < n_targets; i++){
       void* param = pserver.interface.f_ref(target_params[i]);
       ros_ParameterValue val = {};
       if (param){
           val = pserver.interface.f_get(param);
       }
       serialize_ros_ParameterValue(writer, &val);
    }
    return (ucdr_buffer_length(writer));
}

static size_t set_params_value(picoparams_server_t* server, ucdrBuffer* reader, ucdrBuffer* writer){
    uint32_t n_targets = 0;
    char* path = NULL;
    ucdr_deserialize_uint32_t(reader, &n_targets);

    ucdr_serialize_uint32_t(writer, n_targets);
    for (int i = 0; i < n_targets; i++){
        ucdr_deserialize_rstring(reader, &path);
        void* param = pserver.interface.f_ref(path);
        if(param == NULL){
            ucdr_serialize_bool(writer, false);
            ucdr_serialize_string(writer, "Parameter not found");
            continue;
        }
        ros_ParameterValue val = {};
        deserialize_ros_ParameterValue(reader, &val);
        char* error_msg = NULL;
        bool status = pserver.interface.f_set(param, &val, &error_msg);
        ucdr_serialize_bool(writer, status);
        if (status){
            ucdr_serialize_uint32_t(writer, 0);
        }
        else{
            ucdr_serialize_string(writer, error_msg);
        }
    }
    return (ucdr_buffer_length(writer));
}

static size_t get_params_type(picoparams_server_t* server, ucdrBuffer* reader, ucdrBuffer* writer){
    char* target_params[PP_MAX_REQUEST_STRINGS];
    uint32_t n_targets = 0;
    ucdr_deserialize_sequence_rstring(reader, target_params, PP_MAX_REQUEST_STRINGS, &n_targets);

    ucdr_serialize_uint32_t(writer, n_targets);
    for (int i = 0; i < n_targets; i++){
        void* param = pserver.interface.f_ref(target_params[i]);
        if (param){
            ucdr_serialize_uint8_t(writer, pserver.interface.f_type(param));
        }
        else{
            ucdr_serialize_uint8_t(writer, 0);
        }
    }
    return (ucdr_buffer_length(writer));
}

static size_t describe_params(picoparams_server_t* server, ucdrBuffer* reader, ucdrBuffer* writer){
    char* target_params[PP_MAX_REQUEST_STRINGS];
    uint32_t n_targets = 0;
    ucdr_deserialize_sequence_rstring(reader, target_params, PP_MAX_REQUEST_STRINGS, &n_targets);

    ucdr_serialize_uint32_t(writer, n_targets);
    for (int i = 0; i < n_targets; i++){
        void* param = pserver.interface.f_ref(target_params[i]);
        ros_ParameterDescriptor pdesc = {};
        if (param){
            pdesc = pserver.interface.f_describe(param);
        }
        pdesc.name = target_params[i]; // get request param name
        serialize_rcl_ParameterDescriptor(writer, &pdesc);
    }

    return (ucdr_buffer_length(writer));
}



picoros_service_reply_t params_server_handler(uint8_t* request_data, size_t request_size, void* user_data) {
    picoros_service_reply_t reply = {};
    picoros_service_t* s = (picoros_service_t*)user_data;

    ucdrBuffer querry_writter = {};
    ucdrBuffer querry_reader = {};
    ucdr_init_buffer(&querry_reader, request_data + 4, request_size - 4);
    *((uint32_t*)pserver.interface.reply_buf) = 0x0100;
    ucdr_init_buffer(&querry_writter, pserver.interface.reply_buf + 4, pserver.interface.reply_buf_size - 4);
    size_t ret = 0;
    if (s == &pserver.list_srv){
        ret = list_params(&pserver, &querry_reader, &querry_writter);
    }
    else if(s == &pserver.describe_srv){
        ret = describe_params(&pserver, &querry_reader, &querry_writter);
    }
    else if(s == &pserver.get_srv){
        ret = get_params_value(&pserver, &querry_reader, &querry_writter);
    }
    else if(s == &pserver.set_srv){
        ret = set_params_value(&pserver, &querry_reader, &querry_writter);
    }
    else if(s == &pserver.get_types_srv){
        ret = get_params_type(&pserver, &querry_reader, &querry_writter);
    }
    else if(s == &pserver.set_atomic_srv){
        // Not implemented
    }

    reply.data = pserver.interface.reply_buf;
    reply.length = ret + 4;
    return reply;
}

