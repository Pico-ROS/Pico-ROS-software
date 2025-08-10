/*******************************************************************************
 * @file    example_types.h
 * @brief   Example ROS message type definitions for picoros
 * @date    2025-May-27
 * 
 * @details This header file provides some standard ROS message and service type definitions.
 *          It is used in the examples to demonstrate the usage of the picoros library.
 * 
 * @copyright Copyright (c) 2025 Ubiquity Robotics
 *******************************************************************************/
#ifndef EXAMPLE_TYPES_H
#define EXAMPLE_TYPES_H

/* MSG_LIST(BTYPE, CTYPE, TTYPE, FIELD, ARRAY)
 *
 * User provided list of message types for creating serdes functions
 * BTYPE = Basic type - only one member, implemented as typedef. FUNC(name, rmw_name, rmw_hash, type)
 * TTYPE = Typedef type - alias for ros type, implemented as typedef. FUNC(name, rmw_name, rmw_hash, type)
 *         (seperated to disable types in _Generic calls)
 * CTYPE = Compound type - more members, implemented as struct. FUNC(name, rmw_name, rmw_hash, <fields...>)
 *      FIELD = Field of coumpound type FUNC(type, name)
 *      ARRAY = Array field of coumpound type FUNC(type, name, size)
 *
 * Each entry in table must have the following format
 * TYPE(                 \  // Can be BTYPE, CTYPE, TTYPE
 *      name,            \  // Name of type used in code
 *      rmw_name,        \  // Full string name of type used in RMW
 *      rmw_hash,        \  // Type hash string used in RMS
 *      <fields ...>     \  // BTYPE - 1 item - aliased type
 * )                     \  // CTYPE - 1 or more items (struct members) each
 *                          //         wrapped in FIELD() or ARRAY() with no commas between them.
 * note: New lines need to be escaped with \
 */
#define MSG_LIST(BTYPE, CTYPE, TTYPE, FIELD, ARRAY) \
    BTYPE(ros_Int32,                            \
        "example_interfaces::msg::dds_::Int32", \
        "5cd04cd7f3adb9d6c6064c316047b24c76622eb89144f300b536d657fd55e652", \
        int32_t                                 \
    )                                           \
    BTYPE(ros_Int64,                            \
        "example_interfaces::msg::dds_::Int64", \
        "1b3b9a6502f560d079520c73c685a9550e5a1838d2cefd537fe0aba75a3639a0", \
        int64_t                                 \
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
        "6963084842a9b04494d6b2941d11444708d892da2f4b09843b9c43f42a7f6881", \
         ros_Vector3                            \
    )                                           \
    CTYPE(ros_Time,                             \
        "builtin_interfaces::msg::dds_::Time",  \
        "b106235e25a4c5ed35098aa0a61a3ee9c9b18d197f398b0e4206cea9acf9c197", \
        FIELD(int32_t, sec)                     \
        FIELD(int32_t, nsec)                    \
    )                                           \
    CTYPE(ros_Header,                           \
        "builtin_interfaces::msg::dds_::Header",\
        "f49fb3ae2cf070f793645ff749683ac6b06203e41c891e17701b1cb597ce6a01", \
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
        FIELD(ros_Vector3, angular)             \
    )                                           \
    CTYPE(ros_PoseWithCovariance,               \
        "geometry_msgs::msg::dds_::PoseWithCovariance", \
        "9a7c0fd234b7f45c6098745ecccd773ca1085670e64107135397aee31c02e1bb", \
        FIELD(ros_Pose, pose)                   \
        ARRAY(double, covariance, 36)           \
    )                                           \
    CTYPE(ros_TwistWithCovariance,              \
        "geometry_msgs::msg::dds_::TwistWithCovariance", \
        "49f574f033f095d8b6cd1beaca5ca7925e296e84af1716d16c89d38b059c8c18", \
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


/* SRV_LIST(SRV, REQUEST, REPLY, FIELD, ARRAY)
 *
 * User provided list of service types for creating serdes functions
 * SRV = Top level service name, hash and type. FUNC(srv_name, rmw_name, rmw_hash, <request>, <reply>)
 *      REQUEST / REPLY = Request/reply type, implemented as struct. FUNC(<fields...>)
 *          FIELD = Field of request/reply member. FUNC(type, name)
 *          ARRAY = Array field of request/reply member. FUNC(type, name, size)
 *
 * Each entry in table must have the following format
 * SRV(                  \
 *      srv_name,        \  // Name of service type
 *      rmw_name,        \  // Full string name of type used in RMW
 *      rmw_hash,        \  // Type hash string used in RMW
 *      REQUEST(         \
 *          <fields ...> \  // List of fields using FIELD/ARRAY macro with no commas between
 *      )                \
 *      REPLLY(          \
 *          <fields ...> \  // List of fields using FIELD/ARRAY macro with no commas between
 *      )                \
 * )                     \
 * note: New lines need to be escaped with \
 *       Name of generated request/reply types is request_<srv_name> and reply_<srv_name>
 */
#define SRV_LIST(SRV, REQUEST, REPLY, FIELD, ARRAY)     \
    SRV(srv_add2Ints,                                   \
        "example_interfaces::srv::dds_::AddTwoInts",    \
        "e118de6bf5eeb66a2491b5bda11202e7b68f198d6f67922cf30364858239c81a", \
        REQUEST(                                        \
            FIELD(ros_Int64, a)                         \
            FIELD(ros_Int64, b)                         \
        ),                                              \
        REPLY(                                          \
            FIELD(ros_Int64, sum)                       \
        )                                               \
    )

#endif /* EXAMPLE_TYPES_H */
