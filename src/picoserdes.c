/**
  ******************************************************************************
  * @file    picoserdes.c
  * @date    2025-May-31
  * @brief   Description
  ******************************************************************************
  */

/* Private includes ----------------------------------------------------------*/
#include "picoserdes.h"
#include <string.h>
#include <stdio.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
// Base types for which we have external serialization functions
#define BASE_TYPES_LIST(TYPE)   \
    TYPE(int8_t)                \
    TYPE(uint8_t)               \
    TYPE(int16_t)               \
    TYPE(uint16_t)              \
    TYPE(int32_t)               \
    TYPE(uint32_t)              \
    TYPE(int64_t)               \
    TYPE(double)                \
    TYPE(rstring)               \

/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

// type name constants list
#define TYPE_NAME(TYPE, NAME, HASH, ...) char TYPE##_name[] = NAME;
MSG_LIST(TYPE_NAME, TYPE_NAME, TYPE_NAME, PS_UNUSED, PS_UNUSED)
#undef TYPE_HASH
// type hash constants list
#define TYPE_HASH(TYPE, NAME, HASH, ...) char TYPE##_hash[] = HASH;
MSG_LIST(TYPE_HASH, TYPE_HASH, TYPE_HASH, PS_UNUSED, PS_UNUSED)
#undef TYPE_HASH

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

// Base types serialization / deserialization wrappers
#define PS_SER_BASE(TYPE)                                                       \
void ps_ser_##TYPE(ucdrBuffer* writer, TYPE* msg) {                             \
    ucdr_serialize_##TYPE(writer, *msg);                                        \
}
#define PS_SER_BASE_SEQ(TYPE)                                                   \
void ps_ser_seq_##TYPE(ucdrBuffer* writer, TYPE* msg, uint32_t number) {        \
    ucdr_serialize_array_##TYPE(writer, msg, number);                           \
}
#define PS_DES_BASE(TYPE)                                                       \
void ps_des_##TYPE(ucdrBuffer* reader, TYPE* msg) {                             \
    ucdr_deserialize_##TYPE(reader, msg);                                       \
}
#define PS_DES_BASE_SEQ(TYPE)                                                   \
void ps_des_seq_##TYPE(ucdrBuffer* reader, TYPE* msg, uint32_t max_number) {    \
    uint32_t len = 0;                                                           \
    ucdr_deserialize_sequence_##TYPE(reader, msg, max_number, &len);            \
}
BASE_TYPES_LIST(PS_SER_BASE)
BASE_TYPES_LIST(PS_SER_BASE_SEQ)
BASE_TYPES_LIST(PS_DES_BASE)
BASE_TYPES_LIST(PS_DES_BASE_SEQ)

/* Public functions ----------------------------------------------------------*/
/* ----- ucdr helper functions -----------------------------------------------*/
// Deserialize string without copy
size_t ucdr_deserialize_rstring(ucdrBuffer* ub, char** pstring){
    uint32_t len = 0;
    ucdr_deserialize_endian_uint32_t(ub, ub->endianness, &len);
    *pstring = (char*)ub->iterator;
    ub->iterator += len;
    ub->offset += len;
    ub->last_data_size = 1;
    return len;
}
// Wrapper for having consistent names
void ucdr_serialize_rstring(ucdrBuffer* writer, char* pstring){
    ucdr_serialize_string(writer, pstring);
}
// Serialize array of string pointers
void ucdr_serialize_array_rstring(ucdrBuffer* ub, char** strings, uint32_t number){
    ucdr_serialize_endian_uint32_t(ub, ub->endianness, number);
    for (int i = 0; i < number; i++){
       if (ucdr_serialize_sequence_char(ub, strings[i], (uint32_t)strlen(strings[i]) + 1) == false) {
            return;
       }
    }
}
// Deserialize strings sequence to array of string pointers
void ucdr_deserialize_sequence_rstring(ucdrBuffer* ub, char** strings, uint32_t max_number, uint32_t* number){
    ucdr_deserialize_endian_uint32_t(ub, ub->endianness, number);
    if (*number <= max_number){
        for (int i = 0; i < *number; i++){
            ucdr_deserialize_rstring(ub, &strings[i]);
        }
    }
}

// Start cdr sequence and return writer object
ucdr_writer_t ucdr_seq_start(ucdrBuffer* ub){
    // Write sequence size and save pointer for later access
    ucdr_serialize_endian_uint32_t(ub, ub->endianness, 0);
    ucdr_writer_t writer = {
        .p_buffer = ub,
        .p_size = (uint32_t*)(ub->iterator - sizeof(uint32_t)),
    };
    return writer;
}

// Set size of cdr sequence
void ucdr_seq_set_size(ucdr_writer_t* writer, size_t len){
    writer->len = len;
}

// Write string part to cdr sequence
void ucdr_seq_write_str(void* data, char* string){
    ucdr_writer_t* writer = (ucdr_writer_t*)data;
    size_t len = strlen(string); // no null
    ucdr_serialize_endian_array_char(writer->p_buffer, writer->p_buffer->endianness, string, len);
    writer->len += len;
}

// Close cdr sequence and write current size
void ucdr_seq_end(ucdr_writer_t* writer){
    *writer->p_size = writer->len;
}


// ROS types serdes implementation
#define PS_SER_TYPE(TYPE, FIELD)            ps_ser_##TYPE(writer, &msg->FIELD);
#define PS_SER_ARRAY(TYPE, FIELD, NUMBER)   ps_ser_seq_##TYPE(writer, msg->FIELD, NUMBER);
#define PS_DES_TYPE(TYPE, FIELD)            ps_des_##TYPE(reader, &msg->FIELD);
#define PS_DES_ARRAY(TYPE, FIELD, NUMBER)   ps_des_seq_##TYPE(reader, msg->FIELD, NUMBER);
#define PS_SER_FUNC_BIMPL(TYPE, NAME, HASH, TYPE2 ...)      \
void ps_ser_##TYPE(ucdrBuffer* writer, TYPE* msg) {         \
    ps_ser_##TYPE2(writer, msg);                            \
}
#define PS_SER_FUNC_CIMPL(TYPE, NAME, HASH, ...)            \
void ps_ser_##TYPE(ucdrBuffer* writer, TYPE* msg) {         \
    __VA_ARGS__                                             \
}
#define PS_DES_FUNC_BIMPL(TYPE, NAME, HASH, TYPE2 ...)      \
void ps_des_##TYPE(ucdrBuffer* reader, TYPE* msg) {         \
    ps_des_##TYPE2(reader, msg);                            \
}
#define PS_DES_FUNC_CIMPL(TYPE, NAME, HASH, ...)            \
void ps_des_##TYPE(ucdrBuffer* reader, TYPE* msg) {         \
    __VA_ARGS__                                             \
}
MSG_LIST(PS_SER_FUNC_BIMPL, PS_SER_FUNC_CIMPL, PS_SER_FUNC_BIMPL, PS_SER_TYPE, PS_SER_ARRAY)
MSG_LIST(PS_DES_FUNC_BIMPL, PS_DES_FUNC_CIMPL, PS_DES_FUNC_BIMPL, PS_DES_TYPE, PS_DES_ARRAY)


