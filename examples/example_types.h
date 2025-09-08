/* Generated ROS message type definitions */
#ifndef GENERATED_TYPES_H
#define GENERATED_TYPES_H

#define MSG_LIST(BTYPE, CTYPE, TTYPE, FIELD, ARRAY, SEQUENCE) \
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
    CTYPE(ros_DiagnosticStatus, \
        "diagnostic_msgs::msg::dds_::DiagnosticStatus", \
        "b0e3e692ea2d54a8af2f4ef1930e81556a2db55216b771f8a7d2724ed47bf0e4", \
        FIELD(uint8_t, level) \
        FIELD(rstring, name) \
        FIELD(rstring, message) \
        FIELD(rstring, hardware_id) \
        SEQUENCE(ros_KeyValue, values) \
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
    CTYPE(ros_Polygon, \
        "geometry_msgs::msg::dds_::Polygon", \
        "3782f9f0bf044964d692d6c017d705e37611afb1f0bf6a9dee248a7dda0f784a", \
        SEQUENCE(ros_Point32, points) \
    ) \
    CTYPE(ros_PolygonInstance, \
        "geometry_msgs::msg::dds_::PolygonInstance", \
        "fa1cb3dc774329865258afef74f65b0553d487510c6d0f93ba38cc32d62ac0e5", \
        FIELD(ros_Polygon, polygon) \
        FIELD(int64_t, id) \
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
    CTYPE(ros_ListParametersResult, \
        "rcl_interfaces::msg::dds_::ListParametersResult", \
        "237ae3428413dcbcfb452b510c42355f3a2b021dc091afa3e18526d57022f1cd", \
        SEQUENCE(rstring, names) \
        SEQUENCE(rstring, prefixes) \
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
    CTYPE(ros_ParameterValue, \
        "rcl_interfaces::msg::dds_::ParameterValue", \
        "115fc089a387e23c7ecd3525c9189c379109119d6ab82e8dfbde0fdf6a7f9b68", \
        FIELD(uint8_t, type) \
        FIELD(bool, bool_value) \
        FIELD(int64_t, integer_value) \
        FIELD(double, double_value) \
        FIELD(rstring, string_value) \
        SEQUENCE(uint8_t, byte_array_value) \
        SEQUENCE(bool, bool_array_value) \
        SEQUENCE(int64_t, integer_array_value) \
        SEQUENCE(double, double_array_value) \
        SEQUENCE(rstring, string_array_value) \
    ) \
    CTYPE(ros_Parameter, \
        "rcl_interfaces::msg::dds_::Parameter", \
        "ddfe6442cffc462317adb5c92536a7b6dd55858c5c3e1e328165a6b73c2831af", \
        FIELD(rstring, name) \
        FIELD(ros_ParameterValue, value) \
    ) \
    CTYPE(ros_ParameterEvent, \
        "rcl_interfaces::msg::dds_::ParameterEvent", \
        "043e627780fcad87a22d225bc2a037361dba713fca6a6b9f4b869a5aa0393204", \
        FIELD(ros_Time, stamp) \
        FIELD(rstring, node) \
        SEQUENCE(ros_Parameter, new_parameters) \
        SEQUENCE(ros_Parameter, changed_parameters) \
        SEQUENCE(ros_Parameter, deleted_parameters) \
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
    CTYPE(ros_ChannelFloat32, \
        "sensor_msgs::msg::dds_::ChannelFloat32", \
        "92665437ddf39346f4ba39ee32e648390605b633cc077d40f4bd4d7b58af6cd4", \
        FIELD(rstring, name) \
        SEQUENCE(float, values) \
    ) \
    CTYPE(ros_JoyFeedback, \
        "sensor_msgs::msg::dds_::JoyFeedback", \
        "231dd362f71d6fc08272770d07120ad5fe5874ce2dbac70109b28986834290cd", \
        FIELD(uint8_t, type) \
        FIELD(uint8_t, id) \
        FIELD(float, intensity) \
    ) \
    CTYPE(ros_JoyFeedbackArray, \
        "sensor_msgs::msg::dds_::JoyFeedbackArray", \
        "3287c32e1b688cae04555e465443df3cca7dae76ee4ebf85c4658d585037bcaa", \
        SEQUENCE(ros_JoyFeedback, array) \
    ) \
    CTYPE(ros_LaserEcho, \
        "sensor_msgs::msg::dds_::LaserEcho", \
        "0fbc05a0db7d37fe52c0f0375356db55da0046f7ef5bd27ca6b34bd0582bc952", \
        SEQUENCE(float, echoes) \
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
    CTYPE(ros_Mesh, \
        "shape_msgs::msg::dds_::Mesh", \
        "f2150b82d8ee7e8bc3f396a2b158aefb4b9a5510a474be271ba1268aebb55289", \
        SEQUENCE(ros_MeshTriangle, triangles) \
        SEQUENCE(ros_Point, vertices) \
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
    CTYPE(ros_MetricsMessage, \
        "statistics_msgs::msg::dds_::MetricsMessage", \
        "36a67fb499fc2a11b8b9ff9ac735ed76e8d5a1f7775d6d5b97b625188ec68e20", \
        FIELD(rstring, measurement_source_name) \
        FIELD(rstring, metrics_source) \
        FIELD(rstring, unit) \
        FIELD(ros_Time, window_start) \
        FIELD(ros_Time, window_stop) \
        SEQUENCE(ros_StatisticDataPoint, statistics) \
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
    CTYPE(ros_DiagnosticArray, \
        "diagnostic_msgs::msg::dds_::DiagnosticArray", \
        "5a8a36efb05fb25070fa0fb3810290c0e6cd4862b54a8fb975a1ee8dc55a333e", \
        FIELD(ros_Header, header) \
        SEQUENCE(ros_DiagnosticStatus, status) \
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
    CTYPE(ros_PolygonInstanceStamped, \
        "geometry_msgs::msg::dds_::PolygonInstanceStamped", \
        "802f37ea4398d7ce547936aab1fd278923716a13c63373887cd896957434ce2f", \
        FIELD(ros_Header, header) \
        FIELD(ros_PolygonInstance, polygon) \
    ) \
    CTYPE(ros_PolygonStamped, \
        "geometry_msgs::msg::dds_::PolygonStamped", \
        "b7cf07932f1523d4b4088075945c1a0141f7cd21da87cc940fc61652e9138b46", \
        FIELD(ros_Header, header) \
        FIELD(ros_Polygon, polygon) \
    ) \
    CTYPE(ros_PoseArray, \
        "geometry_msgs::msg::dds_::PoseArray", \
        "af0cc36d190e104d546d168d6b39df04fa4b4ccecf59cb4c9ed328d3d5004aa0", \
        FIELD(ros_Header, header) \
        SEQUENCE(ros_Pose, poses) \
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
    CTYPE(ros_Goals, \
        "nav_msgs::msg::dds_::Goals", \
        "02305a51633b5c04d8979b878a7577cafd422f8a07465c878b17a920af3759e9", \
        FIELD(ros_Header, header) \
        SEQUENCE(ros_PoseStamped, goals) \
    ) \
    CTYPE(ros_GridCells, \
        "nav_msgs::msg::dds_::GridCells", \
        "bb99c2f5d0a04750745a81ec6a8147aa373cce5bd17c8cd6507f2413354a6933", \
        FIELD(ros_Header, header) \
        FIELD(float, cell_width) \
        FIELD(float, cell_height) \
        SEQUENCE(ros_Point, cells) \
    ) \
    CTYPE(ros_OccupancyGrid, \
        "nav_msgs::msg::dds_::OccupancyGrid", \
        "8d348150c12913a31ee0ec170fbf25089e4745d17035792a1ba94d6f0bc0cfc7", \
        FIELD(ros_Header, header) \
        FIELD(ros_MapMetaData, info) \
        SEQUENCE(int8_t, data) \
    ) \
    CTYPE(ros_Odometry, \
        "nav_msgs::msg::dds_::Odometry", \
        "3cc97dc7fb7502f8714462c526d369e35b603cfc34d946e3f2eda2766dfec6e0", \
        FIELD(ros_Header, header) \
        FIELD(rstring, child_frame_id) \
        FIELD(ros_PoseWithCovariance, pose) \
        FIELD(ros_TwistWithCovariance, twist) \
    ) \
    CTYPE(ros_Path, \
        "nav_msgs::msg::dds_::Path", \
        "1957a5bb3cee5da65c4e52e52b65a93df227efce4c20f8458b36e73066ca334b", \
        FIELD(ros_Header, header) \
        SEQUENCE(ros_PoseStamped, poses) \
    ) \
    CTYPE(ros_BatteryState, \
        "sensor_msgs::msg::dds_::BatteryState", \
        "4bee5dfce981c98faa6828b868307a0a73f992ed0789f374ee96c8f840e69741", \
        FIELD(ros_Header, header) \
        FIELD(float, voltage) \
        FIELD(float, temperature) \
        FIELD(float, current) \
        FIELD(float, charge) \
        FIELD(float, capacity) \
        FIELD(float, design_capacity) \
        FIELD(float, percentage) \
        FIELD(uint8_t, power_supply_status) \
        FIELD(uint8_t, power_supply_health) \
        FIELD(uint8_t, power_supply_technology) \
        FIELD(bool, present) \
        SEQUENCE(float, cell_voltage) \
        SEQUENCE(float, cell_temperature) \
        FIELD(rstring, location) \
        FIELD(rstring, serial_number) \
    ) \
    CTYPE(ros_CameraInfo, \
        "sensor_msgs::msg::dds_::CameraInfo", \
        "b3dfd68ff46c9d56c80fd3bd4ed22c7a4ddce8c8348f2f59c299e73118e7e275", \
        FIELD(ros_Header, header) \
        FIELD(uint32_t, height) \
        FIELD(uint32_t, width) \
        FIELD(rstring, distortion_model) \
        SEQUENCE(double, d) \
        ARRAY(double, k, 9) \
        ARRAY(double, r, 9) \
        ARRAY(double, p, 12) \
        FIELD(uint32_t, binning_x) \
        FIELD(uint32_t, binning_y) \
        FIELD(ros_RegionOfInterest, roi) \
    ) \
    CTYPE(ros_CompressedImage, \
        "sensor_msgs::msg::dds_::CompressedImage", \
        "15640771531571185e2efc8a100baf923961a4d15d5569652e6cb6691e8e371a", \
        FIELD(ros_Header, header) \
        FIELD(rstring, format) \
        SEQUENCE(uint8_t, data) \
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
    CTYPE(ros_Image, \
        "sensor_msgs::msg::dds_::Image", \
        "d31d41a9a4c4bc8eae9be757b0beed306564f7526c88ea6a4588fb9582527d47", \
        FIELD(ros_Header, header) \
        FIELD(uint32_t, height) \
        FIELD(uint32_t, width) \
        FIELD(rstring, encoding) \
        FIELD(uint8_t, is_bigendian) \
        FIELD(uint32_t, step) \
        SEQUENCE(uint8_t, data) \
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
    CTYPE(ros_JointState, \
        "sensor_msgs::msg::dds_::JointState", \
        "a13ee3a330e346c9d87b5aa18d24e11690752bd33a0350f11c5882bc9179260e", \
        FIELD(ros_Header, header) \
        SEQUENCE(rstring, name) \
        SEQUENCE(double, position) \
        SEQUENCE(double, velocity) \
        SEQUENCE(double, effort) \
    ) \
    CTYPE(ros_Joy, \
        "sensor_msgs::msg::dds_::Joy", \
        "0d356c79cad3401e35ffeb75a96a96e08be3ef896b8b83841d73e890989372c5", \
        FIELD(ros_Header, header) \
        SEQUENCE(float, axes) \
        SEQUENCE(int32_t, buttons) \
    ) \
    CTYPE(ros_LaserScan, \
        "sensor_msgs::msg::dds_::LaserScan", \
        "64c191398013af96509d518dac71d5164f9382553fce5c1f8cca5be7924bd828", \
        FIELD(ros_Header, header) \
        FIELD(float, angle_min) \
        FIELD(float, angle_max) \
        FIELD(float, angle_increment) \
        FIELD(float, time_increment) \
        FIELD(float, scan_time) \
        FIELD(float, range_min) \
        FIELD(float, range_max) \
        SEQUENCE(float, ranges) \
        SEQUENCE(float, intensities) \
    ) \
    CTYPE(ros_MagneticField, \
        "sensor_msgs::msg::dds_::MagneticField", \
        "e80f32f56a20486c9923008fc1a1db07bbb273cbbf6a5b3bfa00835ee00e4dff", \
        FIELD(ros_Header, header) \
        FIELD(ros_Vector3, magnetic_field) \
        ARRAY(double, magnetic_field_covariance, 9) \
    ) \
    CTYPE(ros_MultiDOFJointState, \
        "sensor_msgs::msg::dds_::MultiDOFJointState", \
        "4d4ded702cfba7ff3ec783835c1a1425f75e53939a430ff355d1fee4b3bbc40b", \
        FIELD(ros_Header, header) \
        SEQUENCE(rstring, joint_names) \
        SEQUENCE(ros_Transform, transforms) \
        SEQUENCE(ros_Twist, twist) \
        SEQUENCE(ros_Wrench, wrench) \
    ) \
    CTYPE(ros_MultiEchoLaserScan, \
        "sensor_msgs::msg::dds_::MultiEchoLaserScan", \
        "ba5eac341cd5bbb2701527aa4568e8baec172b69cadb9a1945d6f149d087ee48", \
        FIELD(ros_Header, header) \
        FIELD(float, angle_min) \
        FIELD(float, angle_max) \
        FIELD(float, angle_increment) \
        FIELD(float, time_increment) \
        FIELD(float, scan_time) \
        FIELD(float, range_min) \
        FIELD(float, range_max) \
        SEQUENCE(ros_LaserEcho, ranges) \
        SEQUENCE(ros_LaserEcho, intensities) \
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
    CTYPE(ros_PointCloud, \
        "sensor_msgs::msg::dds_::PointCloud", \
        "614593df71d3c2b9bd4604a71b750fd218f0d65c045ea988b713719455a65b3b", \
        FIELD(ros_Header, header) \
        SEQUENCE(ros_Point32, points) \
        SEQUENCE(ros_ChannelFloat32, channels) \
    ) \
    CTYPE(ros_PointCloud2, \
        "sensor_msgs::msg::dds_::PointCloud2", \
        "9198cabf7da3796ae6fe19c4cb3bdd3525492988c70522628af5daa124bae2b5", \
        FIELD(ros_Header, header) \
        FIELD(uint32_t, height) \
        FIELD(uint32_t, width) \
        SEQUENCE(ros_PointField, fields) \
        FIELD(bool, is_bigendian) \
        FIELD(uint32_t, point_step) \
        FIELD(uint32_t, row_step) \
        SEQUENCE(uint8_t, data) \
        FIELD(bool, is_dense) \
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
    CTYPE(ros_MultiArrayLayout, \
        "std_msgs::msg::dds_::MultiArrayLayout", \
        "4c66e6f78e740ac103a94cf63259f968e48c617e7699e829b63c21a5cb50dac6", \
        SEQUENCE(ros_MultiArrayDimension, dim) \
        FIELD(uint32_t, data_offset) \
    ) \
    CTYPE(ros_ByteMultiArray, \
        "std_msgs::msg::dds_::ByteMultiArray", \
        "972fec7f50ab3c1d06783c228e79e8a9a509021708c511c059926261ada901d4", \
        FIELD(ros_MultiArrayLayout, layout) \
        SEQUENCE(uint8_t, data) \
    ) \
    CTYPE(ros_Float32MultiArray, \
        "std_msgs::msg::dds_::Float32MultiArray", \
        "0599f6f85b4bfca379873a0b4375a0aca022156bd2d7021275d116ed1fa8bfe0", \
        FIELD(ros_MultiArrayLayout, layout) \
        SEQUENCE(float, data) \
    ) \
    CTYPE(ros_Float64MultiArray, \
        "std_msgs::msg::dds_::Float64MultiArray", \
        "1025ddc6b9552d191f89ef1a8d2f60f3d373e28b283d8891ddcc974e8c55397f", \
        FIELD(ros_MultiArrayLayout, layout) \
        SEQUENCE(double, data) \
    ) \
    CTYPE(ros_Int16MultiArray, \
        "std_msgs::msg::dds_::Int16MultiArray", \
        "b58810e8e5b90fb19a5062469eb8409f5ab11a446d60de7157a1457e52a076ce", \
        FIELD(ros_MultiArrayLayout, layout) \
        SEQUENCE(int16_t, data) \
    ) \
    CTYPE(ros_Int32MultiArray, \
        "std_msgs::msg::dds_::Int32MultiArray", \
        "84a7346323525d1b4dfca899df3820f245e54009dac5a6b69217d14fdefd1701", \
        FIELD(ros_MultiArrayLayout, layout) \
        SEQUENCE(int32_t, data) \
    ) \
    CTYPE(ros_Int64MultiArray, \
        "std_msgs::msg::dds_::Int64MultiArray", \
        "e60f9fe34d697f0939ad49d33158693c1277fbac0e2f04b7c2995dc21c89b422", \
        FIELD(ros_MultiArrayLayout, layout) \
        SEQUENCE(int64_t, data) \
    ) \
    CTYPE(ros_Int8MultiArray, \
        "std_msgs::msg::dds_::Int8MultiArray", \
        "f21998d4b492abd63330765d75d5831238d400740386f651f13a872a4d2188db", \
        FIELD(ros_MultiArrayLayout, layout) \
        SEQUENCE(int8_t, data) \
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
    CTYPE(ros_UInt16MultiArray, \
        "std_msgs::msg::dds_::UInt16MultiArray", \
        "94fe73428ec63baecc774f8fb82406123e9291cf728f1b7c91caf5335129492b", \
        FIELD(ros_MultiArrayLayout, layout) \
        SEQUENCE(uint16_t, data) \
    ) \
    BTYPE(ros_UInt32, \
        "std_msgs::msg::dds_::UInt32", \
        "a5c874829b752bc5fa190024b0ad76f578cc278271e855c7d02a818b3516fb4a", \
        uint32_t \
    ) \
    CTYPE(ros_UInt32MultiArray, \
        "std_msgs::msg::dds_::UInt32MultiArray", \
        "6c2577c7ad3cbdcc2164a41c12f1d5ad314ea320f3fb1ee47e78019fe16bb5b0", \
        FIELD(ros_MultiArrayLayout, layout) \
        SEQUENCE(uint32_t, data) \
    ) \
    BTYPE(ros_UInt64, \
        "std_msgs::msg::dds_::UInt64", \
        "fbdc52018fc13755dce18024d1a671c856aa8b4aaf63adfb095b608f98e8c943", \
        uint64_t \
    ) \
    CTYPE(ros_UInt64MultiArray, \
        "std_msgs::msg::dds_::UInt64MultiArray", \
        "fc1c685c2f76bdc6983da025cb25d2db5fb5157b059e300f6d957d86f981b366", \
        FIELD(ros_MultiArrayLayout, layout) \
        SEQUENCE(uint64_t, data) \
    ) \
    BTYPE(ros_UInt8, \
        "std_msgs::msg::dds_::UInt8", \
        "6138bd83d8c3569cb80a667db03cfc1629f529fee79d944c39c34e352e72f010", \
        uint8_t \
    ) \
    CTYPE(ros_UInt8MultiArray, \
        "std_msgs::msg::dds_::UInt8MultiArray", \
        "5687e861b8d307a5e48b7515467ae7a5fc2daf805bd0ce6d8e9e604bade9f385", \
        FIELD(ros_MultiArrayLayout, layout) \
        SEQUENCE(uint8_t, data) \
    ) \
    CTYPE(ros_DisparityImage, \
        "stereo_msgs::msg::dds_::DisparityImage", \
        "1ec1ff6b5bace919e4544a37f2d96ead9f81783701b7b7a4d97a09325ecf2711", \
        FIELD(ros_Header, header) \
        FIELD(ros_Image, image) \
        FIELD(float, f) \
        FIELD(float, t) \
        FIELD(ros_RegionOfInterest, valid_window) \
        FIELD(float, min_disparity) \
        FIELD(float, max_disparity) \
        FIELD(float, delta_d) \
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
    CTYPE(ros_JointTrajectoryPoint, \
        "trajectory_msgs::msg::dds_::JointTrajectoryPoint", \
        "de8907036d8bd45aac6f30cc9044a3d4a329c42cbf719aff7d95a584cfa532d7", \
        SEQUENCE(double, positions) \
        SEQUENCE(double, velocities) \
        SEQUENCE(double, accelerations) \
        SEQUENCE(double, effort) \
        FIELD(ros_Duration, time_from_start) \
    ) \
    CTYPE(ros_JointTrajectory, \
        "trajectory_msgs::msg::dds_::JointTrajectory", \
        "179b33eba59d676f6d967ac71fe35e7ca2f64b2f3928f4a018cec115e213796e", \
        FIELD(ros_Header, header) \
        SEQUENCE(rstring, joint_names) \
        SEQUENCE(ros_JointTrajectoryPoint, points) \
    ) \
    CTYPE(ros_MultiDOFJointTrajectoryPoint, \
        "trajectory_msgs::msg::dds_::MultiDOFJointTrajectoryPoint", \
        "6ada1085b5ee64eaa069b074968e69f0e27c8c5e6f5bb0586dd1c834ef0e32b8", \
        SEQUENCE(ros_Transform, transforms) \
        SEQUENCE(ros_Twist, velocities) \
        SEQUENCE(ros_Twist, accelerations) \
        FIELD(ros_Duration, time_from_start) \
    ) \
    CTYPE(ros_MultiDOFJointTrajectory, \
        "trajectory_msgs::msg::dds_::MultiDOFJointTrajectory", \
        "3a18fd095292a65cfde8833c72985a30af981f3ec44494655c6267262b443a4a", \
        FIELD(ros_Header, header) \
        SEQUENCE(rstring, joint_names) \
        SEQUENCE(ros_MultiDOFJointTrajectoryPoint, points) \
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
    CTYPE(ros_GoalStatusArray, \
        "action_msgs::msg::dds_::GoalStatusArray", \
        "6c1684b00f177d37438febe6e709fc4e2b0d4248dca4854946f9ed8b30cda83e", \
        SEQUENCE(ros_GoalStatus, status_list) \
    ) \
    CTYPE(ros_ImageMarker, \
        "visualization_msgs::msg::dds_::ImageMarker", \
        "603152491ef2331c200a5305230d31f6e8704875944b388da0f547c415d11836", \
        FIELD(ros_Header, header) \
        FIELD(rstring, ns) \
        FIELD(int32_t, id) \
        FIELD(int32_t, type) \
        FIELD(int32_t, action) \
        FIELD(ros_Point, position) \
        FIELD(float, scale) \
        FIELD(ros_ColorRGBA, outline_color) \
        FIELD(uint8_t, filled) \
        FIELD(ros_ColorRGBA, fill_color) \
        FIELD(ros_Duration, lifetime) \
        SEQUENCE(ros_Point, points) \
        SEQUENCE(ros_ColorRGBA, outline_colors) \
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
    CTYPE(ros_MeshFile, \
        "visualization_msgs::msg::dds_::MeshFile", \
        "7710ece15a148fb7c9b546364cfb215bb06098087bd6394fe5b73a493508f8c4", \
        FIELD(rstring, filename) \
        SEQUENCE(uint8_t, data) \
    ) \
    CTYPE(ros_UVCoordinate, \
        "visualization_msgs::msg::dds_::UVCoordinate", \
        "f27f7ed21fe360c6066944f856b801a0c0d1e94e815b6886444b42d90b196a26", \
        FIELD(float, u) \
        FIELD(float, v) \
    ) \
    CTYPE(ros_Marker, \
        "visualization_msgs::msg::dds_::Marker", \
        "45b13ccf791f225962bf74e746f9644518855d783a6f42ba0cc14fde2b4f3ce0", \
        FIELD(ros_Header, header) \
        FIELD(rstring, ns) \
        FIELD(int32_t, id) \
        FIELD(int32_t, type) \
        FIELD(int32_t, action) \
        FIELD(ros_Pose, pose) \
        FIELD(ros_Vector3, scale) \
        FIELD(ros_ColorRGBA, color) \
        FIELD(ros_Duration, lifetime) \
        FIELD(bool, frame_locked) \
        SEQUENCE(ros_Point, points) \
        SEQUENCE(ros_ColorRGBA, colors) \
        FIELD(rstring, texture_resource) \
        FIELD(ros_CompressedImage, texture) \
        SEQUENCE(ros_UVCoordinate, uv_coordinates) \
        FIELD(rstring, text) \
        FIELD(rstring, mesh_resource) \
        FIELD(ros_MeshFile, mesh_file) \
        FIELD(bool, mesh_use_embedded_materials) \
    ) \
    CTYPE(ros_InteractiveMarkerControl, \
        "visualization_msgs::msg::dds_::InteractiveMarkerControl", \
        "60e2fa36344f5f4791b24a809542a18bffd555f563550d4b22b3bbfc31ec0ed5", \
        FIELD(rstring, name) \
        FIELD(ros_Quaternion, orientation) \
        FIELD(uint8_t, orientation_mode) \
        FIELD(uint8_t, interaction_mode) \
        FIELD(bool, always_visible) \
        SEQUENCE(ros_Marker, markers) \
        FIELD(bool, independent_marker_orientation) \
        FIELD(rstring, description) \
    ) \
    CTYPE(ros_InteractiveMarker, \
        "visualization_msgs::msg::dds_::InteractiveMarker", \
        "3d5b51448b51d73b0f395b94d259edd3a5d269ae9f7d9fd5cceb9ae4b72be346", \
        FIELD(ros_Header, header) \
        FIELD(ros_Pose, pose) \
        FIELD(rstring, name) \
        FIELD(rstring, description) \
        FIELD(float, scale) \
        SEQUENCE(ros_MenuEntry, menu_entries) \
        SEQUENCE(ros_InteractiveMarkerControl, controls) \
    ) \
    CTYPE(ros_InteractiveMarkerInit, \
        "visualization_msgs::msg::dds_::InteractiveMarkerInit", \
        "23fda1b3373b154d9d6408dd7b9f8129b2a2b76b905ee421e8c4109d8bf71f78", \
        FIELD(rstring, server_id) \
        FIELD(uint64_t, seq_num) \
        SEQUENCE(ros_InteractiveMarker, markers) \
    ) \
    CTYPE(ros_InteractiveMarkerUpdate, \
        "visualization_msgs::msg::dds_::InteractiveMarkerUpdate", \
        "0a8b000c4fd4d50876ac716a7de018911599a2f015795388e956a2ca8b0c54f0", \
        FIELD(rstring, server_id) \
        FIELD(uint64_t, seq_num) \
        FIELD(uint8_t, type) \
        SEQUENCE(ros_InteractiveMarker, markers) \
        SEQUENCE(ros_InteractiveMarkerPose, poses) \
        SEQUENCE(rstring, erases) \
    ) \
    CTYPE(ros_MarkerArray, \
        "visualization_msgs::msg::dds_::MarkerArray", \
        "86cb8800b6fb05b5eff1abd7a56f62a5641d3ae9a1c29e78e67e704f1d067dcf", \
        SEQUENCE(ros_Marker, markers) \
    )

#define SRV_LIST(SRV, REQUEST, REPLY, FIELD, ARRAY, SEQUENCE) \
    SRV(srv_CancelGoal, \
        "action_msgs::srv::dds_::CancelGoal", \
        "573d8b0a534451d7bc2ac8c5ffde8ac14b8593b7001175d0cd6516dcbeb8689a", \
        REQUEST( \
            FIELD(ros_GoalInfo, goal_info) \
        ), \
        REPLY( \
            FIELD(int8_t, return_code) \
            SEQUENCE(ros_GoalInfo, goals_canceling) \
        ) \
    ) \
    SRV(srv_AddTwoInts, \
        "example_interfaces::srv::dds_::AddTwoInts", \
        "e118de6bf5eeb66a2491b5bda11202e7b68f198d6f67922cf30364858239c81a", \
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
        "abe9e4bb6b41b40e6789712c00ec8871923e089af3f667a79992a428cff2da0a", \
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
        "eeff2cd6fa5ad9d27cdf4dec64818317839b62f212a91e6b5304b634b2062c5f", \
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
        "356fe34f0475a43acf54542013af4167b0e729f77ea22ffb045c6ad8e20668e5", \
        REQUEST( \
            FIELD(ros_Transition, transition) \
        ), \
        REPLY( \
            FIELD(bool, success) \
        ) \
    ) \
    SRV(srv_GetAvailableStates, \
        "lifecycle_msgs::srv::dds_::GetAvailableStates", \
        "00a07d79d2207d71e81a8cbc1880e5d924cc16d4688ea8e8e06e443dc8f8aa1d", \
        REQUEST( \
            FIELD(uint8_t, empty) \
        ), \
        REPLY( \
            SEQUENCE(ros_State, available_states) \
        ) \
    ) \
    SRV(srv_GetAvailableTransitions, \
        "lifecycle_msgs::srv::dds_::GetAvailableTransitions", \
        "59b7ecefce0982a8a844b9f2c4f14764c1c4543cc55e72924e2aa4adad83e9bc", \
        REQUEST( \
            FIELD(uint8_t, empty) \
        ), \
        REPLY( \
            SEQUENCE(ros_TransitionDescription, available_transitions) \
        ) \
    ) \
    SRV(srv_GetState, \
        "lifecycle_msgs::srv::dds_::GetState", \
        "800a0a5aae599782b02932de0caf563f6dc4e7e94b794eadde075ba2cbef9795", \
        REQUEST( \
            FIELD(uint8_t, empty) \
        ), \
        REPLY( \
            FIELD(ros_State, current_state) \
        ) \
    ) \
    SRV(srv_GetLoggerLevels, \
        "rcl_interfaces::srv::dds_::GetLoggerLevels", \
        "03bf1bebd0d6514c7ed0ba7c5e08dc9f2f39c759fe99e1e30ea4157d7674f72d", \
        REQUEST( \
            SEQUENCE(rstring, names) \
        ), \
        REPLY( \
            SEQUENCE(ros_LoggerLevel, levels) \
        ) \
    ) \
    SRV(srv_GetParameterTypes, \
        "rcl_interfaces::srv::dds_::GetParameterTypes", \
        "da199c878688b3e530bdfe3ca8f74cb9fa0c303101e980a9e8f260e25e1c80ca", \
        REQUEST( \
            SEQUENCE(rstring, names) \
        ), \
        REPLY( \
            SEQUENCE(uint8_t, types) \
        ) \
    ) \
    SRV(srv_GetParameters, \
        "rcl_interfaces::srv::dds_::GetParameters", \
        "bf9803d5c74cf989a5de3e0c2e99444599a627c7ff75f97b8c05b01003675cbc", \
        REQUEST( \
            SEQUENCE(rstring, names) \
        ), \
        REPLY( \
            SEQUENCE(ros_ParameterValue, values) \
        ) \
    ) \
    SRV(srv_ListParameters, \
        "rcl_interfaces::srv::dds_::ListParameters", \
        "3e6062bfbb27bfb8730d4cef2558221f51a11646d78e7bb30a1e83afac3aad9d", \
        REQUEST( \
            SEQUENCE(rstring, prefixes) \
            FIELD(uint64_t, depth) \
        ), \
        REPLY( \
            FIELD(ros_ListParametersResult, result) \
        ) \
    ) \
    SRV(srv_SetLoggerLevels, \
        "rcl_interfaces::srv::dds_::SetLoggerLevels", \
        "3ff86cb4e91fbf9abae15c234ecc874448de6ece8e193401c077cf116e4f6d78", \
        REQUEST( \
            SEQUENCE(ros_LoggerLevel, levels) \
        ), \
        REPLY( \
            SEQUENCE(ros_SetLoggerLevelsResult, results) \
        ) \
    ) \
    SRV(srv_SetParameters, \
        "rcl_interfaces::srv::dds_::SetParameters", \
        "56eed9a67e169f9cb6c1f987bc88f868c14a8fc9f743a263bc734c154015d7e0", \
        REQUEST( \
            SEQUENCE(ros_Parameter, parameters) \
        ), \
        REPLY( \
            SEQUENCE(ros_SetParametersResult, results) \
        ) \
    ) \
    SRV(srv_SetParametersAtomically, \
        "rcl_interfaces::srv::dds_::SetParametersAtomically", \
        "0e192ef259c07fc3c07a13191d27002222e65e00ccec653ca05e856f79285fcd", \
        REQUEST( \
            SEQUENCE(ros_Parameter, parameters) \
        ), \
        REPLY( \
            FIELD(ros_SetParametersResult, result) \
        ) \
    ) \
    SRV(srv_ListNodes, \
        "composition_interfaces::srv::dds_::ListNodes", \
        "13f30d73a74c844ee4bacf78f3680f08777cad137f10fbd6a648544cc7b59d2a", \
        REQUEST( \
            FIELD(uint8_t, empty) \
        ), \
        REPLY( \
            SEQUENCE(rstring, full_node_names) \
            SEQUENCE(uint64_t, unique_ids) \
        ) \
    ) \
    SRV(srv_LoadNode, \
        "composition_interfaces::srv::dds_::LoadNode", \
        "2e521ef6066fba6d7fc3a3b175d486709f01b4d738d9a59188858546fc8e5acd", \
        REQUEST( \
            FIELD(rstring, package_name) \
            FIELD(rstring, plugin_name) \
            FIELD(rstring, node_name) \
            FIELD(rstring, node_namespace) \
            FIELD(uint8_t, log_level) \
            SEQUENCE(rstring, remap_rules) \
            SEQUENCE(ros_Parameter, parameters) \
            SEQUENCE(ros_Parameter, extra_arguments) \
        ), \
        REPLY( \
            FIELD(bool, success) \
            FIELD(rstring, error_message) \
            FIELD(rstring, full_node_name) \
            FIELD(uint64_t, unique_id) \
        ) \
    ) \
    SRV(srv_UnloadNode, \
        "composition_interfaces::srv::dds_::UnloadNode", \
        "dce562f83adc735ddcfe9dfb927b89293d4dd0fe2b8fc91e772fb83e970a43b0", \
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
        "6b91084c9f8bffd9b0c1f6883e0e8e491a831f4fc6c6aec77a4efaf3f1ea90b5", \
        REQUEST( \
            FIELD(rstring, load_namespace) \
        ), \
        REPLY( \
            FIELD(bool, success) \
            FIELD(rstring, message) \
        ) \
    ) \
    SRV(srv_SelfTest, \
        "diagnostic_msgs::srv::dds_::SelfTest", \
        "df1e4e54a814a16ccc77471e9239618d46818e8d0e3b45ad6f8c34c6e2abfef6", \
        REQUEST( \
            FIELD(uint8_t, empty) \
        ), \
        REPLY( \
            FIELD(rstring, id) \
            FIELD(uint8_t, passed) \
            SEQUENCE(ros_DiagnosticStatus, status) \
        ) \
    ) \
    SRV(srv_GetMap, \
        "nav_msgs::srv::dds_::GetMap", \
        "c8ae77c9995b3554b5ba80e4d4d443f970ac65143102a1d893ec24fc07b31147", \
        REQUEST( \
            FIELD(uint8_t, empty) \
        ), \
        REPLY( \
            FIELD(ros_OccupancyGrid, map) \
        ) \
    ) \
    SRV(srv_GetPlan, \
        "nav_msgs::srv::dds_::GetPlan", \
        "234f7aff100f5edb8150366601687b027bcdc253db47decb88fff846193fe5e8", \
        REQUEST( \
            FIELD(ros_PoseStamped, start) \
            FIELD(ros_PoseStamped, goal) \
            FIELD(float, tolerance) \
        ), \
        REPLY( \
            FIELD(ros_Path, plan) \
        ) \
    ) \
    SRV(srv_LoadMap, \
        "nav_msgs::srv::dds_::LoadMap", \
        "1a192ac56c40fed2767dac26f0b371785372276bd465c902676d2dca135aae5a", \
        REQUEST( \
            FIELD(rstring, map_url) \
        ), \
        REPLY( \
            FIELD(ros_OccupancyGrid, map) \
            FIELD(uint8_t, result) \
        ) \
    ) \
    SRV(srv_SetMap, \
        "nav_msgs::srv::dds_::SetMap", \
        "5e11a5b2ca53d8ae85b666a019f16c9904ebc787828f1f566c4e048a1ddedfb4", \
        REQUEST( \
            FIELD(ros_OccupancyGrid, map) \
            FIELD(ros_PoseWithCovarianceStamped, initial_pose) \
        ), \
        REPLY( \
            FIELD(bool, success) \
        ) \
    ) \
    SRV(srv_SetCameraInfo, \
        "sensor_msgs::srv::dds_::SetCameraInfo", \
        "a10cca5d33dc637c8d49db50ab288701a3592bb9cd854f2f16a0659613b68984", \
        REQUEST( \
            FIELD(ros_CameraInfo, camera_info) \
        ), \
        REPLY( \
            FIELD(bool, success) \
            FIELD(rstring, status_message) \
        ) \
    ) \
    SRV(srv_Empty, \
        "std_srvs::srv::dds_::Empty", \
        "5888399dedec5ccc85ea6451949fd2c9f97bfdf963f9a588821639fcd31b5d19", \
        REQUEST( \
            FIELD(uint8_t, empty) \
        ), \
        REPLY( \
            FIELD(uint8_t, empty) \
        ) \
    ) \
    SRV(srv_GetInteractiveMarkers, \
        "visualization_msgs::srv::dds_::GetInteractiveMarkers", \
        "bfdfaa861f4a8422b4ec3bd02c6681e4a45a83bdc2913d8facdf923d8ad1376a", \
        REQUEST( \
            FIELD(uint8_t, empty) \
        ), \
        REPLY( \
            FIELD(uint64_t, sequence_number) \
            SEQUENCE(ros_InteractiveMarker, markers) \
        ) \
    )

#endif /* GENERATED_TYPES_H */
