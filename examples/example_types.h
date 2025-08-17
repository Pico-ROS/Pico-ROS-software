/* Generated ROS message type definitions */
#ifndef GENERATED_TYPES_H
#define GENERATED_TYPES_H

#define MSG_LIST(BTYPE, CTYPE, TTYPE, FIELD, ARRAY) \
    CTYPE(ros_Duration, \
        "builtin_interfaces::msg::dds_::Duration", \
        "e8d009f659816f758b75334ee1a9ca5b5c0b859843261f14c7f937349599d93b", \
        FIELD(int32_t, sec) \
        FIELD(uint32_t, nanosec) \
    ) \
    CTYPE(ros_Time, \
        "builtin_interfaces::msg::dds_::Time", \
        "b106235e25a4c5ed35098aa0a61a3ee9c9b18d197f398b0e4206cea9acf9c197", \
        FIELD(int32_t, sec) \
        FIELD(uint32_t, nanosec) \
    ) \
    CTYPE(ros_KeyValue, \
        "diagnostic_msgs::msg::dds_::KeyValue", \
        "d68081eaa540288c5440753baecef0c4e16e81a5f78ad68902ded5100413bb42", \
        FIELD(rstring, key) \
        FIELD(rstring, value) \
    ) \
    CTYPE(ros_Point, \
        "geometry_msgs::msg::dds_::Point", \
        "6963084842a9b04494d6b2941d11444708d892da2f4b09843b9c43f42a7f6881", \
        FIELD(double, x) \
        FIELD(double, y) \
        FIELD(double, z) \
    ) \
    CTYPE(ros_Point32, \
        "geometry_msgs::msg::dds_::Point32", \
        "2fc4db7cae16a4582c79a56b66173a8d48d52c7dc520ddc55a0d4bcf2a4bfdbc", \
        FIELD(float, x) \
        FIELD(float, y) \
        FIELD(float, z) \
    ) \
    CTYPE(ros_Quaternion, \
        "geometry_msgs::msg::dds_::Quaternion", \
        "8a765f66778c8ff7c8ab94afcc590a2ed5325a1d9a076ffff38fbce36f458684", \
        FIELD(double, x) \
        FIELD(double, y) \
        FIELD(double, z) \
        FIELD(double, w) \
    ) \
    CTYPE(ros_Pose, \
        "geometry_msgs::msg::dds_::Pose", \
        "d501954e9476cea2996984e812054b68026ae0bfae789d9a10b23daf35cc90fa", \
        FIELD(ros_Point, position) \
        FIELD(ros_Quaternion, orientation) \
    ) \
    CTYPE(ros_PoseWithCovariance, \
        "geometry_msgs::msg::dds_::PoseWithCovariance", \
        "9a7c0fd234b7f45c6098745ecccd773ca1085670e64107135397aee31c02e1bb", \
        FIELD(ros_Pose, pose) \
        ARRAY(double, covariance, 36) \
    ) \
    CTYPE(ros_Vector3, \
        "geometry_msgs::msg::dds_::Vector3", \
        "cc12fe83e4c02719f1ce8070bfd14aecd40f75a96696a67a2a1f37f7dbb0765d", \
        FIELD(double, x) \
        FIELD(double, y) \
        FIELD(double, z) \
    ) \
    CTYPE(ros_Accel, \
        "geometry_msgs::msg::dds_::Accel", \
        "dc448243ded9b1fcbcca24aba0c22f013dae06c354ba2d849571c0a2a3f57ca0", \
        FIELD(ros_Vector3, linear) \
        FIELD(ros_Vector3, angular) \
    ) \
    CTYPE(ros_AccelWithCovariance, \
        "geometry_msgs::msg::dds_::AccelWithCovariance", \
        "230d51bd53bc36f260574e73b42941cefe44684753480b6fc330c032c5db5997", \
        FIELD(ros_Accel, accel) \
        ARRAY(double, covariance, 36) \
    ) \
    CTYPE(ros_Inertia, \
        "geometry_msgs::msg::dds_::Inertia", \
        "2ddd5dab5c347825ba2e56c895ddccfd0b8efe53ae931bf67f905529930b4bd7", \
        FIELD(double, m) \
        FIELD(ros_Vector3, com) \
        FIELD(double, ixx) \
        FIELD(double, ixy) \
        FIELD(double, ixz) \
        FIELD(double, iyy) \
        FIELD(double, iyz) \
        FIELD(double, izz) \
    ) \
    CTYPE(ros_Transform, \
        "geometry_msgs::msg::dds_::Transform", \
        "beb83fbe698636351461f6f35d1abb20010c43d55374d81bd041f1ba2581fddc", \
        FIELD(ros_Vector3, translation) \
        FIELD(ros_Quaternion, rotation) \
    ) \
    CTYPE(ros_Twist, \
        "geometry_msgs::msg::dds_::Twist", \
        "9c45bf16fe0983d80e3cfe750d6835843d265a9a6c46bd2e609fcddde6fb8d2a", \
        FIELD(ros_Vector3, linear) \
        FIELD(ros_Vector3, angular) \
    ) \
    CTYPE(ros_TwistWithCovariance, \
        "geometry_msgs::msg::dds_::TwistWithCovariance", \
        "49f574f033f095d8b6cd1beaca5ca7925e296e84af1716d16c89d38b059c8c18", \
        FIELD(ros_Twist, twist) \
        ARRAY(double, covariance, 36) \
    ) \
    CTYPE(ros_Wrench, \
        "geometry_msgs::msg::dds_::Wrench", \
        "018e8519d57c16adbe97c9fe1460ef21fec7e31bc541de3d653a35895677ce52", \
        FIELD(ros_Vector3, force) \
        FIELD(ros_Vector3, torque) \
    ) \
    CTYPE(ros_State, \
        "lifecycle_msgs::msg::dds_::State", \
        "dd2d02b82f3ebc858e53c431b1e6e91f3ffc71436fc81d0715214ac6ee2107a0", \
        FIELD(uint8_t, id) \
        FIELD(rstring, label) \
    ) \
    CTYPE(ros_Transition, \
        "lifecycle_msgs::msg::dds_::Transition", \
        "c65d7b31ea134cba4f54fc867b817979be799f7452035cd35fac9b7421fb3424", \
        FIELD(uint8_t, id) \
        FIELD(rstring, label) \
    ) \
    CTYPE(ros_TransitionDescription, \
        "lifecycle_msgs::msg::dds_::TransitionDescription", \
        "c5f1cd4bb1ad2ba0e3329d4ac7015c52a674a72c1faf7974c37a33f4f6048b28", \
        FIELD(ros_Transition, transition) \
        FIELD(ros_State, start_state) \
        FIELD(ros_State, goal_state) \
    ) \
    CTYPE(ros_TransitionEvent, \
        "lifecycle_msgs::msg::dds_::TransitionEvent", \
        "d5f8873a2f0146498f812d7885c7327ce27e463d36811d8792f35ee38c0d6c38", \
        FIELD(uint64_t, timestamp) \
        FIELD(ros_Transition, transition) \
        FIELD(ros_State, start_state) \
        FIELD(ros_State, goal_state) \
    ) \
    CTYPE(ros_MapMetaData, \
        "nav_msgs::msg::dds_::MapMetaData", \
        "2772d4b2000ef2b35dbaeb80fd3946c1369f817fb4f75677d916d27c17d763c8", \
        FIELD(ros_Time, map_load_time) \
        FIELD(float, resolution) \
        FIELD(uint32_t, width) \
        FIELD(uint32_t, height) \
        FIELD(ros_Pose, origin) \
    ) \
    CTYPE(ros_FloatingPointRange, \
        "rcl_interfaces::msg::dds_::FloatingPointRange", \
        "e6af23a23c177fee5f3075c8b1e435162a9b63c863d78c06017460b49684262d", \
        FIELD(double, from_value) \
        FIELD(double, to_value) \
        FIELD(double, step) \
    ) \
    CTYPE(ros_IntegerRange, \
        "rcl_interfaces::msg::dds_::IntegerRange", \
        "f7b7fdc0f65f07702e099218e13288c3963bcb9345bde78b560e6cd19800fc5a", \
        FIELD(int64_t, from_value) \
        FIELD(int64_t, to_value) \
        FIELD(uint64_t, step) \
    ) \
    CTYPE(ros_Log, \
        "rcl_interfaces::msg::dds_::Log", \
        "e28ce254ca8abc06abf92773b74602cdbf116ed34fbaf294fb9f81da9f318eac", \
        FIELD(ros_Time, stamp) \
        FIELD(uint8_t, level) \
        FIELD(rstring, name) \
        FIELD(rstring, msg) \
        FIELD(rstring, file) \
        FIELD(rstring, function) \
        FIELD(uint32_t, line) \
    ) \
    CTYPE(ros_LoggerLevel, \
        "rcl_interfaces::msg::dds_::LoggerLevel", \
        "95785cc42f048ab4f395af65035aeaf2181d8e1c7a44edb8ad4558445fdb43c0", \
        FIELD(rstring, name) \
        FIELD(uint32_t, level) \
    ) \
    BTYPE(ros_ParameterType, \
        "rcl_interfaces::msg::dds_::ParameterType", \
        "df29ed057a834862187be24dd187d981790ff3ea6502f4cd27b432cbc42c6d46", \
        uint8_t \
    ) \
    CTYPE(ros_SetLoggerLevelsResult, \
        "rcl_interfaces::msg::dds_::SetLoggerLevelsResult", \
        "9316e5e679a5b72d2dd7fd80c539bae9e106fa0890a06dc5da3a8177a3ff6909", \
        FIELD(bool, successful) \
        FIELD(rstring, reason) \
    ) \
    CTYPE(ros_SetParametersResult, \
        "rcl_interfaces::msg::dds_::SetParametersResult", \
        "cfcc0fb0371ee5159b403960ef4300f8f9d2f1fd6117c8666b7f9654d528a9b1", \
        FIELD(bool, successful) \
        FIELD(rstring, reason) \
    ) \
    TTYPE(ros_Clock, \
        "rosgraph_msgs::msg::dds_::Clock", \
        "692f7a66e93a3c83e71765d033b60349ba68023a8c689a79e48078bcb5c58564", \
        ros_Time \
    ) \
    CTYPE(ros_JoyFeedback, \
        "sensor_msgs::msg::dds_::JoyFeedback", \
        "231dd362f71d6fc08272770d07120ad5fe5874ce2dbac70109b28986834290cd", \
        FIELD(uint8_t, type) \
        FIELD(uint8_t, id) \
        FIELD(float, intensity) \
    ) \
    CTYPE(ros_NavSatStatus, \
        "sensor_msgs::msg::dds_::NavSatStatus", \
        "d1ed3befa628e09571bd273b888ba1c1fd187c9a5e0006b385d7e5e9095a3204", \
        FIELD(int8_t, status) \
        FIELD(uint16_t, service) \
    ) \
    CTYPE(ros_PointField, \
        "sensor_msgs::msg::dds_::PointField", \
        "5c6a4750728c2bcfbbf7037225b20b02d4429634732146b742dee1726637ef01", \
        FIELD(rstring, name) \
        FIELD(uint32_t, offset) \
        FIELD(uint8_t, datatype) \
        FIELD(uint32_t, count) \
    ) \
    CTYPE(ros_RegionOfInterest, \
        "sensor_msgs::msg::dds_::RegionOfInterest", \
        "ad16bcba5f9131dcdba6fbded19f726f5440e3c513b4fb586dd3027eeed8abb1", \
        FIELD(uint32_t, x_offset) \
        FIELD(uint32_t, y_offset) \
        FIELD(uint32_t, height) \
        FIELD(uint32_t, width) \
        FIELD(bool, do_rectify) \
    ) \
    CTYPE(ros_ServiceEventInfo, \
        "service_msgs::msg::dds_::ServiceEventInfo", \
        "41bcbbe07a75c9b52bc96bfd5c24d7f0fc0a08c0cb7921b3373c5732345a6f45", \
        FIELD(uint8_t, event_type) \
        FIELD(ros_Time, stamp) \
        ARRAY(uint8_t, client_gid, 16) \
        FIELD(int64_t, sequence_number) \
    ) \
    CTYPE(ros_MeshTriangle, \
        "shape_msgs::msg::dds_::MeshTriangle", \
        "618e5c073eeb729e433ef6226e72c01d995c459fb7d76348c9700409a5020bd0", \
        ARRAY(uint32_t, vertex_indices, 3) \
    ) \
    CTYPE(ros_Plane, \
        "shape_msgs::msg::dds_::Plane", \
        "dfbfe8314689c850615d4a727af017e9aa86c10e369a606c8c851ef8f16c58c8", \
        ARRAY(double, coef, 4) \
    ) \
    CTYPE(ros_StatisticDataPoint, \
        "statistics_msgs::msg::dds_::StatisticDataPoint", \
        "b7e61a407346ea912effb6954023d2d2c9adf8f7bc10f6cae576d350c445f6a5", \
        FIELD(uint8_t, data_type) \
        FIELD(double, data) \
    ) \
    BTYPE(ros_StatisticDataType, \
        "statistics_msgs::msg::dds_::StatisticDataType", \
        "840d83bd5dc660e0c6e996f9e1ab80bec3c5a91976360a40d6cc579a37a8c959", \
        uint8_t \
    ) \
    BTYPE(ros_Bool, \
        "std_msgs::msg::dds_::Bool", \
        "feb91e995ff9ebd09c0cb3d2aed18b11077585839fb5db80193b62d74528f6c9", \
        bool \
    ) \
    TTYPE(ros_Byte, \
        "std_msgs::msg::dds_::Byte", \
        "41e1a3345f73fe93ede006da826a6ee274af23dd4653976ff249b0f44e3e798f", \
        uint8_t \
    ) \
    BTYPE(ros_Char, \
        "std_msgs::msg::dds_::Char", \
        "3ad2d04dd29ba19d04b16659afa3ccaedd691914b02a64e82e252f2fa6a586a9", \
        uint8_t \
    ) \
    CTYPE(ros_ColorRGBA, \
        "std_msgs::msg::dds_::ColorRGBA", \
        "77a7a5b9ae477306097665106e0413ba74440245b1f3d0c6d6405fe5c7813fe8", \
        FIELD(float, r) \
        FIELD(float, g) \
        FIELD(float, b) \
        FIELD(float, a) \
    ) \
    BTYPE(ros_Empty, \
        "std_msgs::msg::dds_::Empty", \
        "20b625256f32d5dbc0d04fee44f43c41e51c70d3502f84b4a08e7a9c26a96312", \
        uint8_t \
    ) \
    BTYPE(ros_Float32, \
        "std_msgs::msg::dds_::Float32", \
        "7170d3d8f841f7be3172ce5f4f59f3a4d7f63b0447e8b33327601ad64d83d6e2", \
        float \
    ) \
    BTYPE(ros_Float64, \
        "std_msgs::msg::dds_::Float64", \
        "705ba9c3d1a09df43737eb67095534de36fd426c0587779bda2bc51fe790182a", \
        double \
    ) \
    CTYPE(ros_Header, \
        "std_msgs::msg::dds_::Header", \
        "f49fb3ae2cf070f793645ff749683ac6b06203e41c891e17701b1cb597ce6a01", \
        FIELD(ros_Time, stamp) \
        FIELD(rstring, frame_id) \
    ) \
    CTYPE(ros_AccelStamped, \
        "geometry_msgs::msg::dds_::AccelStamped", \
        "ef1df9eabae0a708cc049a061ebcddc4e2a5f745730100ba680e086a9698b165", \
        FIELD(ros_Header, header) \
        FIELD(ros_Accel, accel) \
    ) \
    CTYPE(ros_AccelWithCovarianceStamped, \
        "geometry_msgs::msg::dds_::AccelWithCovarianceStamped", \
        "61c9ad8928e71dd95ce791b2f02809ee2a0bbcc42cd0e4047fd00a822a08e444", \
        FIELD(ros_Header, header) \
        FIELD(ros_AccelWithCovariance, accel) \
    ) \
    CTYPE(ros_InertiaStamped, \
        "geometry_msgs::msg::dds_::InertiaStamped", \
        "766be45976252babf7f9d8ac4ae7c912a7ceccf71035622529f27518b695aa09", \
        FIELD(ros_Header, header) \
        FIELD(ros_Inertia, inertia) \
    ) \
    CTYPE(ros_PointStamped, \
        "geometry_msgs::msg::dds_::PointStamped", \
        "4c0296af86e01e562e9e0405d138a01537247580076c58ea38d7923ac1045897", \
        FIELD(ros_Header, header) \
        FIELD(ros_Point, point) \
    ) \
    CTYPE(ros_PoseStamped, \
        "geometry_msgs::msg::dds_::PoseStamped", \
        "10f3786d7d40fd2b54367835614bff85d4ad3b5dab62bf8bca0cc232d73b4cd8", \
        FIELD(ros_Header, header) \
        FIELD(ros_Pose, pose) \
    ) \
    CTYPE(ros_PoseWithCovarianceStamped, \
        "geometry_msgs::msg::dds_::PoseWithCovarianceStamped", \
        "26432f9803e43727d3c8f668d1fdb3c630f548af631e2f4e31382371bfea3b6e", \
        FIELD(ros_Header, header) \
        FIELD(ros_PoseWithCovariance, pose) \
    ) \
    CTYPE(ros_QuaternionStamped, \
        "geometry_msgs::msg::dds_::QuaternionStamped", \
        "381add86c6c3160644d228ca342182c7fd6c7fab11c7a85ad817a9cc22dbac6e", \
        FIELD(ros_Header, header) \
        FIELD(ros_Quaternion, quaternion) \
    ) \
    CTYPE(ros_TransformStamped, \
        "geometry_msgs::msg::dds_::TransformStamped", \
        "0a241f87d04668d94099cbb5ba11691d5ad32c2f29682e4eb5653424bd275206", \
        FIELD(ros_Header, header) \
        FIELD(rstring, child_frame_id) \
        FIELD(ros_Transform, transform) \
    ) \
    CTYPE(ros_TwistStamped, \
        "geometry_msgs::msg::dds_::TwistStamped", \
        "5f0fcd4f81d5d06ad9b4c4c63e3ea51b82d6ae4d0558f1d475229b1121db6f64", \
        FIELD(ros_Header, header) \
        FIELD(ros_Twist, twist) \
    ) \
    CTYPE(ros_TwistWithCovarianceStamped, \
        "geometry_msgs::msg::dds_::TwistWithCovarianceStamped", \
        "77b67434531e6529b7a0091357b186b6ebdb17fd9ffd3e0c7ce9d3fb11a44563", \
        FIELD(ros_Header, header) \
        FIELD(ros_TwistWithCovariance, twist) \
    ) \
    CTYPE(ros_Vector3Stamped, \
        "geometry_msgs::msg::dds_::Vector3Stamped", \
        "d4829622288cbb443886e7ea94ea5671a3b1be6bab4ad04224432a65f7d7887a", \
        FIELD(ros_Header, header) \
        FIELD(ros_Vector3, vector) \
    ) \
    CTYPE(ros_VelocityStamped, \
        "geometry_msgs::msg::dds_::VelocityStamped", \
        "55e7196186c8dbe4375278d7f1ac050dd8c9bacade1cf3eef8460fa667bd2457", \
        FIELD(ros_Header, header) \
        FIELD(rstring, body_frame_id) \
        FIELD(rstring, reference_frame_id) \
        FIELD(ros_Twist, velocity) \
    ) \
    CTYPE(ros_WrenchStamped, \
        "geometry_msgs::msg::dds_::WrenchStamped", \
        "8dc3deaf06b2ab281f9f9a742a8961c328ca7cec16e3fd6586d3a5c83fa78f77", \
        FIELD(ros_Header, header) \
        FIELD(ros_Wrench, wrench) \
    ) \
    CTYPE(ros_Odometry, \
        "nav_msgs::msg::dds_::Odometry", \
        "3cc97dc7fb7502f8714462c526d369e35b603cfc34d946e3f2eda2766dfec6e0", \
        FIELD(ros_Header, header) \
        FIELD(rstring, child_frame_id) \
        FIELD(ros_PoseWithCovariance, pose) \
        FIELD(ros_TwistWithCovariance, twist) \
    ) \
    CTYPE(ros_FluidPressure, \
        "sensor_msgs::msg::dds_::FluidPressure", \
        "22dfb2b145a0bd5a31a1ac3882a1b32148b51d9b2f3bab250290d66f3595bc32", \
        FIELD(ros_Header, header) \
        FIELD(double, fluid_pressure) \
        FIELD(double, variance) \
    ) \
    CTYPE(ros_Illuminance, \
        "sensor_msgs::msg::dds_::Illuminance", \
        "b954b25f452fcf81a91c9c2a7e3b3fd85c4c873d452aecb3cfd8fd1da732a22d", \
        FIELD(ros_Header, header) \
        FIELD(double, illuminance) \
        FIELD(double, variance) \
    ) \
    CTYPE(ros_Imu, \
        "sensor_msgs::msg::dds_::Imu", \
        "7d9a00ff131080897a5ec7e26e315954b8eae3353c3f995c55faf71574000b5b", \
        FIELD(ros_Header, header) \
        FIELD(ros_Quaternion, orientation) \
        ARRAY(double, orientation_covariance, 9) \
        FIELD(ros_Vector3, angular_velocity) \
        ARRAY(double, angular_velocity_covariance, 9) \
        FIELD(ros_Vector3, linear_acceleration) \
        ARRAY(double, linear_acceleration_covariance, 9) \
    ) \
    CTYPE(ros_MagneticField, \
        "sensor_msgs::msg::dds_::MagneticField", \
        "e80f32f56a20486c9923008fc1a1db07bbb273cbbf6a5b3bfa00835ee00e4dff", \
        FIELD(ros_Header, header) \
        FIELD(ros_Vector3, magnetic_field) \
        ARRAY(double, magnetic_field_covariance, 9) \
    ) \
    CTYPE(ros_NavSatFix, \
        "sensor_msgs::msg::dds_::NavSatFix", \
        "62223ab3fe210a15976021da7afddc9e200dc9ec75231c1b6a557fc598a65404", \
        FIELD(ros_Header, header) \
        FIELD(ros_NavSatStatus, status) \
        FIELD(double, latitude) \
        FIELD(double, longitude) \
        FIELD(double, altitude) \
        ARRAY(double, position_covariance, 9) \
        FIELD(uint8_t, position_covariance_type) \
    ) \
    CTYPE(ros_Range, \
        "sensor_msgs::msg::dds_::Range", \
        "b42b62562e93cbfe9d42b82fe5994dfa3d63d7d5c90a317981703f7388adff3a", \
        FIELD(ros_Header, header) \
        FIELD(uint8_t, radiation_type) \
        FIELD(float, field_of_view) \
        FIELD(float, min_range) \
        FIELD(float, max_range) \
        FIELD(float, range) \
        FIELD(float, variance) \
    ) \
    CTYPE(ros_RelativeHumidity, \
        "sensor_msgs::msg::dds_::RelativeHumidity", \
        "8687c99b4fb393cb2e545e407b5ea7fd0b5d8960bcd849a0f86c544740138839", \
        FIELD(ros_Header, header) \
        FIELD(double, relative_humidity) \
        FIELD(double, variance) \
    ) \
    CTYPE(ros_Temperature, \
        "sensor_msgs::msg::dds_::Temperature", \
        "72514a14126ab9f8a9abec974c78e5610a367b59db5da355ff1fb982d5bad4b8", \
        FIELD(ros_Header, header) \
        FIELD(double, temperature) \
        FIELD(double, variance) \
    ) \
    CTYPE(ros_TimeReference, \
        "sensor_msgs::msg::dds_::TimeReference", \
        "dd66e84cf40bbb5d5a40472e6ecf2675a031334d4c426abdb2ad41801a8efc99", \
        FIELD(ros_Header, header) \
        FIELD(ros_Time, time_ref) \
        FIELD(rstring, source) \
    ) \
    BTYPE(ros_Int16, \
        "std_msgs::msg::dds_::Int16", \
        "1dcc3464e47c288a55f943a389d337cdb06804de3f5cd7a266b0de718eee17e5", \
        int16_t \
    ) \
    BTYPE(ros_Int32, \
        "std_msgs::msg::dds_::Int32", \
        "b6578ded3c58c626cfe8d1a6fb6e04f706f97e9f03d2727c9ff4e74b1cef0deb", \
        int32_t \
    ) \
    BTYPE(ros_Int64, \
        "std_msgs::msg::dds_::Int64", \
        "8cd1048c2f186b6bd9a92472dc1ce51723c0833a221e2b7aecfff111774f4b49", \
        int64_t \
    ) \
    BTYPE(ros_Int8, \
        "std_msgs::msg::dds_::Int8", \
        "26525065a403d972cb672f0777e333f0c799ad444ae5fcd79e43d1e73bd0f440", \
        int8_t \
    ) \
    CTYPE(ros_MultiArrayDimension, \
        "std_msgs::msg::dds_::MultiArrayDimension", \
        "5e773a60a4c7fc8a54985f307c7837aa2994252a126c301957a24e31282c9cbe", \
        FIELD(rstring, label) \
        FIELD(uint32_t, size) \
        FIELD(uint32_t, stride) \
    ) \
    BTYPE(ros_String, \
        "std_msgs::msg::dds_::String", \
        "df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18", \
        rstring \
    ) \
    BTYPE(ros_UInt16, \
        "std_msgs::msg::dds_::UInt16", \
        "08a406e4b022bc22e907f985d6a9e9dd1d4fbecae573549cf49350113e7757b1", \
        uint16_t \
    ) \
    BTYPE(ros_UInt32, \
        "std_msgs::msg::dds_::UInt32", \
        "a5c874829b752bc5fa190024b0ad76f578cc278271e855c7d02a818b3516fb4a", \
        uint32_t \
    ) \
    BTYPE(ros_UInt64, \
        "std_msgs::msg::dds_::UInt64", \
        "fbdc52018fc13755dce18024d1a671c856aa8b4aaf63adfb095b608f98e8c943", \
        uint64_t \
    ) \
    BTYPE(ros_UInt8, \
        "std_msgs::msg::dds_::UInt8", \
        "6138bd83d8c3569cb80a667db03cfc1629f529fee79d944c39c34e352e72f010", \
        uint8_t \
    ) \
    CTYPE(ros_Builtins, \
        "test_msgs::msg::dds_::Builtins", \
        "9e61888e7521dda35c21ac6b6cabbcaff8dae88b6d67b25b9078fdb9abf56303", \
        FIELD(ros_Duration, duration_value) \
        FIELD(ros_Time, time_value) \
    ) \
    CTYPE(ros_KeyedLong, \
        "test_msgs::msg::dds_::KeyedLong", \
        "660defa556c3c4b1991498e62bca58bf71ac6e13bcf9a476aa998a8b66f650f0", \
        FIELD(int32_t, key) \
        FIELD(int32_t, value) \
    ) \
    CTYPE(ros_KeyedString, \
        "test_msgs::msg::dds_::KeyedString", \
        "20c0320c41013bc51612123c18abadac81944a31db59f1caf817829e2f186043", \
        FIELD(rstring, key) \
        FIELD(rstring, value) \
    ) \
    CTYPE(ros_NonKeyedWithNestedKey, \
        "test_msgs::msg::dds_::NonKeyedWithNestedKey", \
        "36baf7e933c8647ccf6a96e27549df0db4c6d5bc0200819dab4c61e6b6d927d2", \
        FIELD(ros_KeyedString, nested_data) \
        FIELD(int32_t, some_int) \
    ) \
    CTYPE(ros_ComplexNestedKey, \
        "test_msgs::msg::dds_::ComplexNestedKey", \
        "67de37f251f9431ed5f2630507939ff5538ecfaaa4169849be18870c3fbe2de8", \
        FIELD(uint32_t, uint32_key) \
        FIELD(ros_NonKeyedWithNestedKey, nested_keys) \
        FIELD(double, float64_value) \
    ) \
    CTYPE(ros_TypeSource, \
        "type_description_interfaces::msg::dds_::TypeSource", \
        "faeaec7596c04ecf5b6e99ad225e4c7cbb997ad5435f793526fb3984d011aae5", \
        FIELD(rstring, type_name) \
        FIELD(rstring, encoding) \
        FIELD(rstring, raw_file_contents) \
    ) \
    CTYPE(ros_UUID, \
        "unique_identifier_msgs::msg::dds_::UUID", \
        "1b8e8aca958cbea28fe6ef60bf6c19b683c97a9ef60bb34752067d0f2f7ab437", \
        ARRAY(uint8_t, uuid, 16) \
    ) \
    CTYPE(ros_GoalInfo, \
        "action_msgs::msg::dds_::GoalInfo", \
        "6398fe763154554353930716b225947f93b672f0fb2e49fdd01bb7a7e37933e9", \
        FIELD(ros_UUID, goal_id) \
        FIELD(ros_Time, stamp) \
    ) \
    CTYPE(ros_GoalStatus, \
        "action_msgs::msg::dds_::GoalStatus", \
        "32f4cfd717735d17657e1178f24431c1ce996c878c515230f6c5b3476819dbb9", \
        FIELD(ros_GoalInfo, goal_info) \
        FIELD(int8_t, status) \
    ) \
    CTYPE(ros_InteractiveMarkerFeedback, \
        "visualization_msgs::msg::dds_::InteractiveMarkerFeedback", \
        "6cc48741df9f05d19ba7d9ea3101e9fcd1309c9d6bda3c55668ba607492f725e", \
        FIELD(ros_Header, header) \
        FIELD(rstring, client_id) \
        FIELD(rstring, marker_name) \
        FIELD(rstring, control_name) \
        FIELD(uint8_t, event_type) \
        FIELD(ros_Pose, pose) \
        FIELD(uint32_t, menu_entry_id) \
        FIELD(ros_Point, mouse_point) \
        FIELD(bool, mouse_point_valid) \
    ) \
    CTYPE(ros_InteractiveMarkerPose, \
        "visualization_msgs::msg::dds_::InteractiveMarkerPose", \
        "c60e9a4407d5f709a63e0fe9caea324aee08fe717cd090209ebe35012ce7cb66", \
        FIELD(ros_Header, header) \
        FIELD(ros_Pose, pose) \
        FIELD(rstring, name) \
    ) \
    CTYPE(ros_MenuEntry, \
        "visualization_msgs::msg::dds_::MenuEntry", \
        "22170c387c70fd4236232ec902de8604e72ff027342c7c0f28ad9f68c64c51d6", \
        FIELD(uint32_t, id) \
        FIELD(uint32_t, parent_id) \
        FIELD(rstring, title) \
        FIELD(rstring, command) \
        FIELD(uint8_t, command_type) \
    ) \
    CTYPE(ros_UVCoordinate, \
        "visualization_msgs::msg::dds_::UVCoordinate", \
        "f27f7ed21fe360c6066944f856b801a0c0d1e94e815b6886444b42d90b196a26", \
        FIELD(float, u) \
        FIELD(float, v) \
    )

#define SRV_LIST(SRV, REQUEST, REPLY, FIELD, ARRAY) \
    SRV(srv_AddTwoInts, \
        "example_interfaces::srv::dds_::AddTwoInts", \
        "000c5fd92d6b2e1a05949348f584d6d652adea1e92d691792011ac2273508302", \
        REQUEST( \
            FIELD(int64_t, a) \
            FIELD(int64_t, b) \
        ), \
        REPLY( \
            FIELD(int64_t, sum) \
        ) \
    ) \
    SRV(srv_SetBool, \
        "std_srvs::srv::dds_::SetBool", \
        "c62fbb99d94e1b25e8ef9e109f9581956bb1b3361a45a4e5810c36a90d29932e", \
        REQUEST( \
            FIELD(bool, data) \
        ), \
        REPLY( \
            FIELD(bool, success) \
            FIELD(rstring, message) \
        ) \
    ) \
    SRV(srv_Trigger, \
        "std_srvs::srv::dds_::Trigger", \
        "d010825374ce8918e72bfd826c82603e60f45419e932ea976f807b74a863a199", \
        REQUEST( \
            FIELD(uint8_t, empty) \
        ), \
        REPLY( \
            FIELD(bool, success) \
            FIELD(rstring, message) \
        ) \
    ) \
    SRV(srv_ChangeState, \
        "lifecycle_msgs::srv::dds_::ChangeState", \
        "d74c6f524e14cade7b67e7297190360ed749264af300524fc1456f27cc2a553a", \
        REQUEST( \
            FIELD(ros_Transition, transition) \
        ), \
        REPLY( \
            FIELD(bool, success) \
        ) \
    ) \
    SRV(srv_GetState, \
        "lifecycle_msgs::srv::dds_::GetState", \
        "77a9c844e7975f15bc2221e8bbebafc4d118afb78c7f7071eef91f83f940f2f6", \
        REQUEST( \
            FIELD(uint8_t, empty) \
        ), \
        REPLY( \
            FIELD(ros_State, current_state) \
        ) \
    ) \
    SRV(srv_UnloadNode, \
        "composition_interfaces::srv::dds_::UnloadNode", \
        "95d5cfc357956740190c7685ac868d9c7c98dcb8c1416aae8eaa351422074e12", \
        REQUEST( \
            FIELD(uint64_t, unique_id) \
        ), \
        REPLY( \
            FIELD(bool, success) \
            FIELD(rstring, error_message) \
        ) \
    ) \
    SRV(srv_AddDiagnostics, \
        "diagnostic_msgs::srv::dds_::AddDiagnostics", \
        "e0b6572a07c2b3ca2c4f2acf742297661f6ceb50f5baa4f9305b6094cf950146", \
        REQUEST( \
            FIELD(rstring, load_namespace) \
        ), \
        REPLY( \
            FIELD(bool, success) \
            FIELD(rstring, message) \
        ) \
    ) \
    SRV(srv_Empty, \
        "std_srvs::srv::dds_::Empty", \
        "458eaf200a3f63d9cc53d19436edebc0c140ef4c93ca75d5bd4ce19fd7759e35", \
        REQUEST( \
            FIELD(uint8_t, empty) \
        ), \
        REPLY( \
            FIELD(uint8_t, empty) \
        ) \
    )

#endif /* GENERATED_TYPES_H */
