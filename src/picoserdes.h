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

typedef char* rstring;
// Supported types list
// BTYPE = Base type
// CTYPE = Compound type
// TTYPE = Typedef type
// FIELD = Field of coumpound type FUNC(type, name)
// ARRAY = Array field of coumpound type FUNC(type, name, size)
#define ROSTYPE_LIST(BTYPE, CTYPE, TTYPE, FIELD, ARRAY)\
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
    )                           \
    BTYPE(ros_String,            \
        "std_msgs::msg::dds_::String", \
        "df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18", \
        rstring           \
    )                           \
    CTYPE(ros_Vector3,           \
        "geometry_msgs::msg::dds_::Vector3",\
        "cc12fe83e4c02719f1ce8070bfd14aecd40f75a96696a67a2a1f37f7dbb0765d", \
        FIELD(double, x)            \
        FIELD(double, y)            \
        FIELD(double, z)            \
    )                           \
    CTYPE(ros_Quaternion,        \
        "geometry_msgs::msg::dds_::Quaternion",\
        "8a765f66778c8ff7c8ab94afcc590a2ed5325a1d9a076ffff38fbce36f458684", \
        FIELD(double, x)            \
        FIELD(double, y)            \
        FIELD(double, z)            \
        FIELD(double, w)            \
    )                           \
    TTYPE(ros_Point,             \
        "geometry_msgs::msg::dds_::Point",\
        "xx", \
         ros_Vector3            \
    )                           \
    CTYPE(ros_Time,              \
        "std_msgs::msg::dds_::Time", \
        "xx", \
        FIELD(int32_t, sec)          \
        FIELD(int32_t, nsec)         \
    )                           \
    CTYPE(ros_Header,            \
        "std_msgs::msg::dds_::Header", \
        "xx", \
        FIELD(ros_Time, time)        \
        FIELD(rstring, frame_id)       \
    )                           \
    CTYPE(ros_Pose,                      \
        "geometry_msgs::msg::dds_::Pose", \
        "d501954e9476cea2996984e812054b68026ae0bfae789d9a10b23daf35cc90fa", \
        FIELD(ros_Point, position)         \
        FIELD(ros_Quaternion, orientation)  \
    )                                   \
    CTYPE(ros_Twist,                     \
        "geometry_msgs::msg::dds_::Twist", \
        "9c45bf16fe0983d80e3cfe750d6835843d265a9a6c46bd2e609fcddde6fb8d2a", \
        FIELD(ros_Vector3, linear)         \
        FIELD(ros_Vector3, angulat)         \
    )                                   \
    CTYPE(ros_PoseWithCovariance,        \
        "geometry_msgs::msg::dds_::PoseWithCovariance", \
        "xx", \
        FIELD(ros_Pose, pose)               \
        ARRAY(double, covariance, 36)       \
    )                                   \
    CTYPE(ros_TwistWithCovariance,       \
        "geometry_msgs::msg::dds_::TwistWithCovariance", \
        "xx", \
        FIELD(ros_Twist, twist)             \
        ARRAY(double, covariance, 36)       \
    )                                   \
    CTYPE(ros_Odometery,                   \
        "nav_msgs::msg::dds_::Odometry", \
        "3cc97dc7fb7502f8714462c526d369e35b603cfc34d946e3f2eda2766dfec6e0", \
        FIELD(ros_PoseWithCovariance, pose)   \
        FIELD(ros_TwistWithCovariance, twist) \
    )                                     \

#define PS_UNUSED(...)

 /* Exported types ------------------------------------------------------------*/
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

typedef struct{
    uint32_t*   p_size;
    ucdrBuffer* p_buffer;
    size_t      len;
}ucdr_writer_t;

/* Exported constants --------------------------------------------------------*/

// string name constants list
#define TYPE_NAME(TYPE, NAME, HASH, ...) const char* TYPE##_name = NAME;
ROSTYPE_LIST(TYPE_NAME, TYPE_NAME, TYPE_NAME, PS_UNUSED, PS_UNUSED)
#undef TYPE_NAME

// type hash constants list
#define TYPE_HASH(TYPE, NAME, HASH, ...) const char* TYPE##_hash = HASH;
ROSTYPE_LIST(TYPE_HASH, TYPE_HASH, TYPE_HASH, PS_UNUSED, PS_UNUSED)
#undef TYPE_HASH

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

#define PS_SEL_SER(TYPE, ...)  TYPE*: ps_ser_##TYPE,
#define PS_SEL_DES(TYPE, ...)  TYPE*: ps_des_##TYPE,
#define ps_serialize(pWRITER, pDATA)                            \
              _Generic((pDATA),                                       \
              ROSTYPE_LIST(PS_SEL_SER, PS_SEL_SER, PS_UNUSED, PS_UNUSED, PS_UNUSED)   \
                  default: 0                                            \
              )(pWRITER, pDATA)

#define ps_deserialize(pWRITER, pDATA)                            \
              _Generic((pDATA),                                       \
              ROSTYPE_LIST(PS_SEL_DES, PS_SEL_DES, PS_UNUSED, PS_UNUSED, PS_UNUSED)   \
                  default: 0                                            \
              )(pWRITER, pDATA)

ucdr_writer_t ucdr_write_seq_start(ucdrBuffer* ub);
void          ucdr_write_str(void* data, char* string);
void          ucdr_write_set_seq_size(ucdr_writer_t* writer, size_t len);
void          ucdr_write_seq_end(ucdr_writer_t* writer);

#ifdef __cplusplus
}
#endif

#endif /* PICOSERDES_H */
