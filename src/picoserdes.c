/**
  ******************************************************************************
  * @file    picoserdes.c
  * @date    2025-May-31
  * @brief   Pico CDR serdes
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
    TYPE(bool)                  \
    TYPE(int8_t)                \
    TYPE(uint8_t)               \
    TYPE(int16_t)               \
    TYPE(uint16_t)              \
    TYPE(int32_t)               \
    TYPE(uint32_t)              \
    TYPE(int64_t)               \
    TYPE(uint64_t)              \
    TYPE(float)                 \
    TYPE(double)                \
    TYPE(rstring)               \

/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

// type name constants list
#define TYPE_NAME(TYPE, NAME, HASH, ...) char TYPE##_name[] = NAME;
MSG_LIST(TYPE_NAME, TYPE_NAME, TYPE_NAME, PS_UNUSED, PS_UNUSED)
SRV_LIST(TYPE_NAME, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED)
#undef TYPE_HASH
// type hash constants list
#define TYPE_HASH(TYPE, NAME, HASH, ...) char TYPE##_hash[] = HASH;
MSG_LIST(TYPE_HASH, TYPE_HASH, TYPE_HASH, PS_UNUSED, PS_UNUSED)
SRV_LIST(TYPE_HASH, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED)
#undef TYPE_HASH

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

// Base types serialization / deserialization wrappers
#define PS_SER_BASE(TYPE)                                                              \
static inline bool ps_ser_##TYPE(ucdrBuffer* writer, TYPE* msg) {                      \
    return ucdr_serialize_##TYPE(writer, *msg);                                        \
}
#define PS_SER_BASE_SEQ(TYPE)                                                          \
static inline bool ps_ser_seq_##TYPE(ucdrBuffer* writer, TYPE* msg, uint32_t number) { \
    return ucdr_serialize_array_##TYPE(writer, msg, number);                           \
}
#define PS_DES_BASE(TYPE)                                                              \
static inline bool ps_des_##TYPE(ucdrBuffer* reader, TYPE* msg) {                      \
    return ucdr_deserialize_##TYPE(reader, msg);                                       \
}
#define PS_DES_BASE_SEQ(TYPE)                                                          \
static inline bool ps_des_seq_##TYPE(ucdrBuffer* reader, TYPE* msg, uint32_t max_number) {    \
    uint32_t len = 0;                                                                  \
    return ucdr_deserialize_sequence_##TYPE(reader, msg, max_number, &len);            \
}
BASE_TYPES_LIST(PS_SER_BASE)
BASE_TYPES_LIST(PS_SER_BASE_SEQ)
BASE_TYPES_LIST(PS_DES_BASE)
BASE_TYPES_LIST(PS_DES_BASE_SEQ)

/* Public functions ----------------------------------------------------------*/
/* ----- ucdr helper functions -----------------------------------------------*/
// Deserialize string without copy
bool ucdr_deserialize_rstring(ucdrBuffer* ub, char** pstring){
    uint32_t len = 0;
    bool ret = ucdr_deserialize_endian_uint32_t(ub, ub->endianness, &len);
    if (ret){
        *pstring = (char*)ub->iterator;
        ub->iterator += len;
        ub->offset += len;
        ub->last_data_size = 1;
    }
    return ret;
}
// Wrapper for having consistent names
bool ucdr_serialize_rstring(ucdrBuffer* writer, char* pstring){
    if (pstring == NULL){
        return ucdr_serialize_uint32_t(writer, 0);
    }
    return ucdr_serialize_string(writer, pstring);
}
// Serialize array of string pointers
bool ucdr_serialize_array_rstring(ucdrBuffer* ub, char** strings, uint32_t number){
    ucdr_serialize_endian_uint32_t(ub, ub->endianness, number);
    for (int i = 0; i < number; i++){
       if (ucdr_serialize_sequence_char(ub, strings[i], (uint32_t)strlen(strings[i]) + 1) == false) {
            return false;
       }
    }
    return true;
}
// Deserialize strings sequence to array of string pointers
bool ucdr_deserialize_sequence_rstring(ucdrBuffer* ub, char** strings, uint32_t max_number, uint32_t* number){
    ucdr_deserialize_endian_uint32_t(ub, ub->endianness, number);
    if (*number <= max_number){
        for (int i = 0; i < *number; i++){
            if (ucdr_deserialize_rstring(ub, &strings[i]) == false){
                return false;
            }
        }
        return true;
    }
    return false;
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

// User message and service serdes implementation
#define EXP_TOKEN(...) __VA_ARGS__
#define PS_SER_TYPE(TYPE, FIELD)                                                                \
    if( ps_ser_##TYPE(writer, &msg->FIELD) != true){ return false; }
#define PS_SER_ARRAY(TYPE, FIELD, NUMBER)                                                       \
    if( ps_ser_seq_##TYPE(writer, msg->FIELD, NUMBER) != true){ return false; }
#define PS_DES_TYPE(TYPE, FIELD)                                                                \
    if( ps_des_##TYPE(reader, &msg->FIELD) != true){ return false; }
#define PS_DES_ARRAY(TYPE, FIELD, NUMBER)  \
    if( ps_des_seq_##TYPE(reader, msg->FIELD, NUMBER) != true){ return false; }
#define PS_SER_MSG_BIMPL(TYPE, NAME, HASH, TYPE2 ...)                                           \
    bool ps_ser_##TYPE(ucdrBuffer* writer, TYPE* msg) { return ps_ser_##TYPE2(writer, msg); }
#define PS_SER_MSG_CIMPL(TYPE, NAME, HASH, ...)                                                 \
    bool ps_ser_##TYPE(ucdrBuffer* writer, TYPE* msg) { __VA_ARGS__ return true; }
#define PS_DES_MSG_BIMPL(TYPE, NAME, HASH, TYPE2 ...)                                           \
    bool ps_des_##TYPE(ucdrBuffer* reader, TYPE* msg) { return ps_des_##TYPE2(reader, msg); }
#define PS_DES_MSG_CIMPL(TYPE, NAME, HASH, ...)                                                 \
    bool ps_des_##TYPE(ucdrBuffer* reader, TYPE* msg) { __VA_ARGS__ return true; }
#define PS_SER_SRV(TYPE, NAME, HASH, REQ, REP)                                                  \
    bool ps_ser_##TYPE##_request(ucdrBuffer* writer, request_##TYPE* msg) { REQ return true; }  \
    bool ps_ser_##TYPE##_reply(ucdrBuffer* writer, reply_##TYPE* msg) { REP return true; }
#define PS_DES_SRV(TYPE, NAME, HASH, REQ, REP)                                                  \
    bool ps_des_##TYPE##_request(ucdrBuffer* reader, request_##TYPE* msg){ REQ return true; }   \
    bool ps_des_##TYPE##_reply(ucdrBuffer* reader, reply_##TYPE* msg) { REP return true; }
MSG_LIST(PS_SER_MSG_BIMPL, PS_SER_MSG_CIMPL, PS_SER_MSG_BIMPL, PS_SER_TYPE, PS_SER_ARRAY)
MSG_LIST(PS_DES_MSG_BIMPL, PS_DES_MSG_CIMPL, PS_DES_MSG_BIMPL, PS_DES_TYPE, PS_DES_ARRAY)
SRV_LIST(PS_SER_SRV, EXP_TOKEN, EXP_TOKEN, PS_SER_TYPE, PS_SER_ARRAY)
SRV_LIST(PS_DES_SRV, EXP_TOKEN, EXP_TOKEN, PS_DES_TYPE, PS_DES_ARRAY)
