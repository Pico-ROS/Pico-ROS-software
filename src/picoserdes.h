/**
  ******************************************************************************
  * @file    picoserdes.h
  * @date    2025-May-31
  * @brief   Description
  ******************************************************************************
  */
#ifndef PICOSERDES_H
#define PICOSERDES_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Exported includes ---------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "ucdr/microcdr.h"

/* Exported macro ------------------------------------------------------------*/
/* Table of supported types
 * BTYPE = Basic type - only one member, implemented as typedef
 * TTYPE = Typedef type - alias for ros type, implemented as typedef
 *         (seperated to disable types in _Generic calls)
 * CTYPE = Compound type - more members, implemented as struct
 *      FIELD = Field of coumpound type FUNC(type, name)
 *      ARRAY = Array field of coumpound type FUNC(type, name, size)
 *
 * Each entry in table must have the following format
 * TYPE(                 \  // Can be BTYPE, CTYPE, TTYPE
 *      type_cname,      \  // Name of type used in code
 *      type_rmw_name,   \  // Full string name of type used in RMW
 *      type_rihs_hash,  \  // Type hash string used in RMS
 *      <fields ...>     \  // BTYPE - 1 item - aliased type
 * )                     \  // CTYPE - 1 or more items (struct members) each
 *                          //         wrapped in FIELD() or ARRAY() with no commas between them.
 * New lines need to be escaped with \
 *
 */

#define ROSTYPE_LIST(BTYPE, CTYPE, TTYPE, FIELD, ARRAY) \
    BTYPE(ros_Int32,                            \
        "std_msgs::msg::dds_::Int32",           \
        "xx",                                   \
        int32_t                                 \
    )                                           \
    BTYPE(ros_Int64,                            \
        "std_msgs::msg::dds_::Int64",           \
        "xx",                                   \
        int64_t                                 \
    )                                           \
    BTYPE(ros_Double,                           \
        "std_msgs::msg::dds_::Double",          \
        "xx",                                   \
        double                                  \
    )                                           \
    BTYPE(ros_String,                           \
        "std_msgs::msg::dds_::String",          \
        "df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18", \
        rstring                                 \
    )                                           \
    CTYPE(ros_Vector3,                          \
        "geometry_msgs::msg::dds_::Vector3",    \
        "cc12fe83e4c02719f1ce8070bfd14aecd40f75a96696a67a2a1f37f7dbb0765d", \
        FIELD(double, x)                        \
        FIELD(double, y)                        \
        FIELD(double, z)                        \
    )                                           \
    CTYPE(ros_Quaternion,                       \
        "geometry_msgs::msg::dds_::Quaternion", \
        "8a765f66778c8ff7c8ab94afcc590a2ed5325a1d9a076ffff38fbce36f458684", \
        FIELD(double, x)                        \
        FIELD(double, y)                        \
        FIELD(double, z)                        \
        FIELD(double, w)                        \
    )                                           \
    TTYPE(ros_Point,                            \
        "geometry_msgs::msg::dds_::Point",      \
        "xx",                                   \
         ros_Vector3                            \
    )                                           \
    CTYPE(ros_Time,                             \
        "std_msgs::msg::dds_::Time",            \
        "xx",                                   \
        FIELD(int32_t, sec)                     \
        FIELD(int32_t, nsec)                    \
    )                                           \
    CTYPE(ros_Header,                           \
        "std_msgs::msg::dds_::Header",          \
        "xx",                                   \
        FIELD(ros_Time, time)                   \
        FIELD(rstring, frame_id)                \
    )                                           \
    CTYPE(ros_Pose,                             \
        "geometry_msgs::msg::dds_::Pose",       \
        "d501954e9476cea2996984e812054b68026ae0bfae789d9a10b23daf35cc90fa", \
        FIELD(ros_Point, position)              \
        FIELD(ros_Quaternion, orientation)      \
    )                                           \
    CTYPE(ros_Twist,                            \
        "geometry_msgs::msg::dds_::Twist",      \
        "9c45bf16fe0983d80e3cfe750d6835843d265a9a6c46bd2e609fcddde6fb8d2a", \
        FIELD(ros_Vector3, linear)              \
        FIELD(ros_Vector3, angulat)             \
    )                                           \
    CTYPE(ros_PoseWithCovariance,               \
        "geometry_msgs::msg::dds_::PoseWithCovariance", \
        "xx",                                   \
        FIELD(ros_Pose, pose)                   \
        ARRAY(double, covariance, 36)           \
    )                                           \
    CTYPE(ros_TwistWithCovariance,              \
        "geometry_msgs::msg::dds_::TwistWithCovariance", \
        "xx",                                   \
        FIELD(ros_Twist, twist)                 \
        ARRAY(double, covariance, 36)           \
    )                                           \
    CTYPE(ros_Odometry,                         \
        "nav_msgs::msg::dds_::Odometry",        \
        "3cc97dc7fb7502f8714462c526d369e35b603cfc34d946e3f2eda2766dfec6e0", \
        FIELD(ros_Header, header)               \
        FIELD(rstring, child_frame_id)          \
        FIELD(ros_PoseWithCovariance, pose)     \
        FIELD(ros_TwistWithCovariance, twist)   \
    )                                           \


#define PS_UNUSED(...)

/* Exported types ------------------------------------------------------------*/
typedef char* rstring;

// Declare all types
#define FIELD_EXPAND(TYPE, NAME) TYPE NAME;
#define ARRAY_EXPAND(TYPE, NAME, SIZE) TYPE NAME[SIZE];
#define BTYPE_DECLARE(TYPE, NAME, HASH, TYPE2, ...) typedef TYPE2 TYPE;
#define CTYPE_DECLARE(TYPE, NAME, HASH, ...) \
    typedef struct { \
        __VA_ARGS__ \
    }TYPE;
ROSTYPE_LIST(BTYPE_DECLARE, CTYPE_DECLARE, BTYPE_DECLARE, FIELD_EXPAND, ARRAY_EXPAND)
#undef FIELD_EXPAND
#undef BTYPE_DECLARE
#undef CTYPE_DECLARE
#undef ARRAY_EXPAND

#define ENUM_NAME(TYPE, ...) TYPE##_E,
typedef enum {
    ROSTYPE_LIST(ENUM_NAME, ENUM_NAME, ENUM_NAME, FIELD_EXPAND, ARRAY_EXPAND)
}ps_type_e;
#undef ENUM_NAME



typedef struct{
    uint32_t*   p_size;
    ucdrBuffer* p_buffer;
    size_t      len;
}ucdr_writer_t;

/* Exported constants --------------------------------------------------------*/

// string name constants list
#define TYPE_NAME(TYPE, NAME, HASH, ...) extern char TYPE##_name[];
ROSTYPE_LIST(TYPE_NAME, TYPE_NAME, TYPE_NAME, PS_UNUSED, PS_UNUSED)
#undef TYPE_NAME

// type hash constants list
#define TYPE_HASH(TYPE, NAME, HASH, ...) extern char TYPE##_hash[];
ROSTYPE_LIST(TYPE_HASH, TYPE_HASH, TYPE_HASH, PS_UNUSED, PS_UNUSED)
#undef TYPE_HASH

#define ROSTYPE_NAME(TYPE) &TYPE##_name[0]
#define ROSTYPE_HASH(TYPE) &TYPE##_hash[0]

/* Exported functions --------------------------------------------------------*/

// type serialization function declarations
#define PS_SER_FUNC_DEF(TYPE, ...) \
    void ps_ser_##TYPE(ucdrBuffer* writer, TYPE* msg);
ROSTYPE_LIST(PS_SER_FUNC_DEF, PS_SER_FUNC_DEF, PS_SER_FUNC_DEF, PS_UNUSED, PS_UNUSED)
#undef PS_SER_FUNC_DEF

// type derialization function declarations
#define PS_DES_FUNC_DEF(TYPE, ...) \
    void ps_des_##TYPE(ucdrBuffer* reader, TYPE* msg);
ROSTYPE_LIST(PS_DES_FUNC_DEF, PS_DES_FUNC_DEF, PS_DES_FUNC_DEF, PS_UNUSED, PS_UNUSED)
#undef PS_DES_FUNC_DEF

// Generic serdes function macros
// pBUF = uint8_t* pointer to start of raw cdr message buffer
// pMSG = *ros_xxx type pointer
// MAX = max size of buffer
#define PS_SEL_SER(TYPE, ...)  TYPE*: ps_ser_##TYPE,
#define PS_SEL_DES(TYPE, ...)  TYPE*: ps_des_##TYPE,
#define ps_serialize(pBUF, pMSG, MAX)                                                   \
    ({                                                                                  \
        ucdrBuffer writer = {};                                                         \
        ucdr_init_buffer(&writer, pBUF, MAX);                                           \
        ucdr_serialize_uint32_t(&writer,  0x0100); /*Little endian */                   \
        _Generic((pMSG),                                                                \
            ROSTYPE_LIST(PS_SEL_SER, PS_SEL_SER, PS_UNUSED, PS_UNUSED, PS_UNUSED)       \
            default: 0                                                                  \
        )(&writer, pMSG);                                                               \
        size_t _ret = ucdr_buffer_length(&writer);                                      \
        _ret;                                                                           \
    })
#define ps_deserialize(pBUF, pMSG, MAX)                                                 \
    {                                                                                   \
        ucdrBuffer reader = {};                                                         \
        ucdr_init_buffer(&reader, pBUF + sizeof(uint32_t), MAX - sizeof(uint32_t));     \
        _Generic((pMSG),                                                                \
            ROSTYPE_LIST(PS_SEL_DES, PS_SEL_DES, PS_UNUSED, PS_UNUSED, PS_UNUSED)       \
            default: 0                                                                  \
        )(&reader, pMSG);                                                               \
    }

size_t          ucdr_deserialize_rstring(ucdrBuffer* ub, char** pstring);
void            ucdr_serialize_rstring(ucdrBuffer* writer, char* pstring);
void            ucdr_serialize_array_rstring(ucdrBuffer* ub, char** strings, uint32_t number);
void            ucdr_deserialize_sequence_rstring(ucdrBuffer* ub, char** strings, uint32_t max_number, uint32_t* number);

ucdr_writer_t   ucdr_seq_start(ucdrBuffer* ub);
void            ucdr_seq_write_str(void* data, char* string);
void            ucdr_seq_set_size(ucdr_writer_t* writer, size_t len);
void            ucdr_seq_end(ucdr_writer_t* writer);

#ifdef __cplusplus
}
#endif

#endif /* PICOSERDES_H */
