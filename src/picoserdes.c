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
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
// basic types list
#define BASE_TYPES_LIST(TYPE)   \
    TYPE(double)                \
    TYPE(int64_t)               \
    TYPE(int32_t)               \
    TYPE(rstring)               \

// helpers
size_t ucdr_deserialize_rstring(ucdrBuffer* ub, char** pstring){
    uint32_t len = 0;
    ucdr_deserialize_endian_uint32_t(ub, ub->endianness, &len);
    *pstring = (char*)ub->iterator;
    ub->iterator += len;
    ub->offset += len;
    ub->last_data_size = 1;
    return len;
}
void ucdr_serialize_rstring(ucdrBuffer* writer, char* pstring){
    ucdr_serialize_string(writer, pstring);
}
void ucdr_serialize_array_rstring(ucdrBuffer* ub, char** strings, uint32_t number){
    ucdr_serialize_endian_uint32_t(ub, ub->endianness, number);
    for (int i = 0; i < number; i++){
       if (ucdr_serialize_sequence_char(ub, strings[i], (uint32_t)strlen(strings[i]) + 1) == false) {
            return;
       }
    }
}

void ucdr_deserialize_sequence_rstring(ucdrBuffer* ub, char** strings, uint32_t max_number, uint32_t* number){
    ucdr_deserialize_endian_uint32_t(ub, ub->endianness, number);
    if (*number <= max_number){
        for (int i = 0; i < *number; i++){
            ucdr_deserialize_rstring(ub, &strings[i]);
        }
    }
}

// Basic types serialization / deserialization
#define PS_SER_BASE(TYPE)                         \
void ps_ser_##TYPE(ucdrBuffer* writer, TYPE* msg) {    \
    ucdr_serialize_##TYPE(writer, *msg);                 \
}
#define PS_SER_BASE_SEQ(TYPE)                         \
void ps_ser_seq_##TYPE(ucdrBuffer* writer, TYPE* msg, uint32_t number) {    \
    ucdr_serialize_array_##TYPE(writer, msg, number);                 \
}
#define PS_DES_BASE(TYPE)                         \
void ps_des_##TYPE(ucdrBuffer* reader, TYPE* msg) {    \
    ucdr_deserialize_##TYPE(reader, msg);                 \
}
#define PS_DES_BASE_SEQ(TYPE)                         \
void ps_des_seq_##TYPE(ucdrBuffer* reader, TYPE* msg, uint32_t max_number) {    \
    uint32_t len = 0;                                                            \
    ucdr_deserialize_sequence_##TYPE(reader, msg, max_number, &len);              \
}

BASE_TYPES_LIST(PS_SER_BASE)
BASE_TYPES_LIST(PS_SER_BASE_SEQ)
BASE_TYPES_LIST(PS_DES_BASE)
BASE_TYPES_LIST(PS_DES_BASE_SEQ)
//#undef PS_SER_BASE
//#undef PS_SER_BASE_SEQ
//#undef PS_DES_BASE
//#undef PS_DES_BASE_SEQ


// ROS types serdes implementation
#define PS_SER_TYPE(TYPE, FIELD) ps_ser_##TYPE(writer, &msg->FIELD);
#define PS_SER_ARRAY(TYPE, FIELD, NUMBER) ps_ser_seq_##TYPE(writer, msg->FIELD, NUMBER);
#define PS_DES_TYPE(TYPE, FIELD)            ps_des_##TYPE(reader, &msg->FIELD);
#define PS_DES_ARRAY(TYPE, FIELD, NUMBER)   ps_des_seq_##TYPE(reader, msg->FIELD, NUMBER);

#define PS_SER_FUNC_BIMPL(TYPE, NAME, HASH, TYPE2 ...)  \
void ps_ser_##TYPE(ucdrBuffer* writer, TYPE* msg) {   \
    ps_ser_##TYPE2(writer, msg);                 \
}
#define PS_SER_FUNC_CIMPL(TYPE, NAME, HASH, ...)  \
void ps_ser_##TYPE(ucdrBuffer* writer, TYPE* msg) {   \
    __VA_ARGS__                                         \
}
#define PS_DES_FUNC_BIMPL(TYPE, NAME, HASH, TYPE2 ...)  \
void ps_des_##TYPE(ucdrBuffer* reader, TYPE* msg) {   \
    ps_des_##TYPE2(reader, msg);                 \
}
#define PS_DES_FUNC_CIMPL(TYPE, NAME, HASH, ...)  \
void ps_des_##TYPE(ucdrBuffer* reader, TYPE* msg) {   \
    __VA_ARGS__                                       \
}

ROSTYPE_LIST(PS_SER_FUNC_BIMPL, PS_SER_FUNC_CIMPL, PS_SER_FUNC_BIMPL, PS_SER_TYPE, PS_SER_ARRAY)
ROSTYPE_LIST(PS_DES_FUNC_BIMPL, PS_DES_FUNC_CIMPL, PS_DES_FUNC_BIMPL, PS_DES_TYPE, PS_DES_ARRAY)

void test(){
    ros_Odometery odo = {};
    ucdrBuffer buf;
    ps_serialize(&buf, &odo);
    ps_deserialize(&buf, &odo);
}

/* Public functions ----------------------------------------------------------*/
