/*******************************************************************************
 * @file    picoserdes.h
 * @brief   Pico CDR Serialization/Deserialization Library
 * @date    2025-May-31
 *
 * @details This library provides CDR (Common Data Representation) serialization and
 *          deserialization functionality for ROS messages. It wraps the Micro-CDR library
 *          and extends it with generic serialization/deserialization macros that work with
 *          any user-defined type. The library allows users to define a list of message types
 *          to automatically generate the corresponding serialization and deserialization
 *          functions for those types at compile time. This eliminates the need to write custom
 *          serdes code for each message type while maintaining type safety through static typing.
 *
 * @copyright Copyright (c) 2025 Ubiquity Robotics
 *******************************************************************************/

#ifndef PICOSERDES_H
#define PICOSERDES_H

#ifdef __cplusplus
 extern "C" {
#endif

 /**
 * @defgroup picoserdes Picoserdes
 * @{
 */

/** @} */

/* Exported includes ---------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "ucdr/microcdr.h"
 /**
 * @defgroup user_types_list User types list
 * @ingroup picoserdes
 * @{
 */
#ifndef USER_TYPE_FILE
/**
 * @brief Message type list macro
 * @details
 * \verbatim
 * User provided list of message types for creating serdes functions
 * BTYPE = Basic type - only one member, implemented as typedef. FUNC(name, rmw_name, rmw_hash, type)
 * TTYPE = Typedef type - alias for ros type, implemented as typedef. FUNC(name, rmw_name, rmw_hash, type)
 *         (seperated to disable types in _Generic calls)
 * CTYPE = Compound type - more members, implemented as struct. FUNC(name, rmw_name, rmw_hash, <fields...>)
 *      FIELD = Field of coumpound type FUNC(type, name)
 *      ARRAY = Array field of coumpound type FUNC(type, name, size)
 *      SEQUENCE = Sequence field of compound type FUNC(type, name)
 *
 * Each entry in table must have the following format
 * TYPE(                 \  // Can be BTYPE, CTYPE, TTYPE
 *      name,            \  // Name of type used in code
 *      rmw_name,        \  // Full string name of type used in RMW
 *      rmw_hash,        \  // Type hash string used in RMS
 *      <fields ...>     \  // BTYPE - 1 item - aliased type
 * )                     \  // CTYPE - 1 or more items (struct members) each
 *                          //         wrapped in FIELD() , ARRAY() or SEQUENCE() with no commas between them.
 * note: New lines need to be escaped with \
 * \endverbatim
 */
    #define MSG_LIST(BTYPE, CTYPE, TTYPE, FIELD, ARRAY, SEQUENCE)
/**
 * @brief Service type list macro
 * @details
 * \verbatim
 * User provided list of service types for creating serdes functions
 * SRV = Top level service name, hash and type. FUNC(srv_name, rmw_name, rmw_hash, <request>, <reply>)
 *      REQUEST / REPLY = Request/reply type, implemented as struct. FUNC(<fields...>)
 *          FIELD = Field of request/reply member. FUNC(type, name)
 *          ARRAY = Array field of request/reply member. FUNC(type, name, size)
 *          SEQUENCE = Sequence field of request/reply member. FUNC(type, name). 
 *
 * Each entry in table must have the following format
 * SRV(                  \
 *      srv_name,        \  // Name of service type
 *      rmw_name,        \  // Full string name of type used in RMW
 *      rmw_hash,        \  // Type hash string used in RMW
 *      REQUEST(         \
 *          <fields ...> \  // List of fields using FIELD macro with no commas between
 *      )                \
 *      REPLLY(          \
 *          <fields ...> \  // List of fields using FIELD macro with no commas between
 *      )                \
 * )                     \
 * note: New lines need to be escaped with \
 *       Name of generated request/reply types is request_<srv_name> and reply_<srv_name>
 * \endverbatim
 */
    #define SRV_LIST(SRV, REQUEST, REPLY, FIELD, ARRAY, SEQUENCE)
#else
    // user needs to provide MSG_LIST and SRV_LIST macros
    #include USER_TYPE_FILE
#endif
/** @} */

/**
 * @brief List of supported base types
 * @details This macro defines all primitive types that can be serialized/deserialized.
 *          Each type is processed by the provided TYPE macro.
 *
 * @param TYPE Macro function to process each base type. Will be called as TYPE(type_name)
 *             where type_name is one of types in the list.
 */
#define BASE_TYPES_LIST(TYPE)   \
    TYPE(bool)                  \
    TYPE(char)                  \
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

/* Exported macro ------------------------------------------------------------*/
/** @brief Empty macro function */
#define PS_UNUSED(...)

/* Exported types ------------------------------------------------------------*/
/** @brief char* type alias to be used for string fields */
typedef char* rstring;

/**
 * @brief Type declaration macros
 * @{
 */
#define CAT(x, y) x##y
#define FIELD_EXPAND(TYPE, NAME) TYPE NAME;
#define ARRAY_EXPAND(TYPE, NAME, SIZE) TYPE NAME[SIZE];
#define SEQUENCE_EXPAND(TYPE, NAME, ...) TYPE##_sequence NAME;
#define SEQUENCE_DECLARE(TYPE, ...)             \
    typedef struct{                             \
        TYPE* data;                             \
        uint32_t n_elements;                    \
    }TYPE##_sequence;
#define BTYPE_DECLARE(TYPE, NAME, HASH, TYPE2, ...) \
    typedef TYPE2 TYPE; \
    SEQUENCE_DECLARE(TYPE)

#define CTYPE_DECLARE(TYPE, NAME, HASH, ...)    \
    typedef struct {                            \
        __VA_ARGS__                             \
    }TYPE;\
     SEQUENCE_DECLARE(TYPE)

#define SRV_DECLARE(TYPE, NAME, HASH, REQ, REP) CAT(REQ, TYPE); CAT(REP,TYPE);
#define REQUEST_DECLARE(...)                    \
    typedef struct {                            \
        __VA_ARGS__                             \
    }request_
#define REPLY_DECLARE(...)                      \
    typedef struct {                            \
        __VA_ARGS__                             \
    }reply_
/** @} */

/**
 * @defgroup custom_type_declarations Custom type declarations
 * @ingroup picoserdes
 * @{
 */
 BASE_TYPES_LIST(SEQUENCE_DECLARE)
 MSG_LIST(BTYPE_DECLARE, CTYPE_DECLARE, BTYPE_DECLARE, FIELD_EXPAND, ARRAY_EXPAND, SEQUENCE_EXPAND)
 SRV_LIST(SRV_DECLARE, REQUEST_DECLARE, REPLY_DECLARE, FIELD_EXPAND, ARRAY_EXPAND, SEQUENCE_EXPAND)
 
/** @} */
#undef CAT
#undef FIELD_EXPAND
#undef ARRAY_EXPAND
#undef BTYPE_DECLARE
#undef CTYPE_DECLARE
#undef SRV_DECLARE
#undef REQUEST_DECLARE
#undef REPLY_DECLARE


/**
 * @brief Writer context for CDR serialization
 */
typedef struct {
    uint32_t*   p_size;    /**< Pointer to size field */
    ucdrBuffer* p_buffer;  /**< Pointer to CDR buffer */
    size_t      len;       /**< Current length */
} ucdr_writer_t;

/* Exported constants --------------------------------------------------------*/
/**
 * @defgroup type_constats Type name and hash constants
 * @ingroup picoserdes
 * @{
 */

#define TYPE_NAME(TYPE, NAME, HASH, ...) extern char TYPE##_name[];
/**
 * @brief Type name constants
 * @{
 */
MSG_LIST(TYPE_NAME, TYPE_NAME, TYPE_NAME, PS_UNUSED, PS_UNUSED, PS_UNUSED)
SRV_LIST(TYPE_NAME, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED,PS_UNUSED)
/** @} */
#undef TYPE_NAME

#define TYPE_HASH(TYPE, NAME, HASH, ...) extern char TYPE##_hash[];
/**
 * @brief Type hash constants
 * @{
 */
MSG_LIST(TYPE_HASH, TYPE_HASH, TYPE_HASH, PS_UNUSED, PS_UNUSED, PS_UNUSED)
SRV_LIST(TYPE_HASH, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED)
/** @} */
#undef TYPE_HASH


/** @brief Get type name macro*/
#define ROSTYPE_NAME(TYPE) &TYPE##_name[0]

/** @brief Get type hash macro*/
#define ROSTYPE_HASH(TYPE) &TYPE##_hash[0]

/** @} */

/* Exported functions --------------------------------------------------------*/


/* Generate serialization function declarations */
#define PS_SER_FUNC_DEF(TYPE, ...)                                          \
    bool ps_ser_##TYPE(ucdrBuffer* writer, TYPE* msg);                      \
    bool ps_ser_sequence_##TYPE(ucdrBuffer* writer, TYPE##_sequence* msg);
#define PS_SER_SRV_FUNC_DEF(TYPE, ...)                                      \
    bool ps_ser_##TYPE##_request(ucdrBuffer* writer, request_##TYPE* msg);  \
    bool ps_ser_##TYPE##_reply(ucdrBuffer* writer, reply_##TYPE* msg);
/**
 * @defgroup serialization_functions Serialization Functions
 * @ingroup picoserdes
 * @{
 */
MSG_LIST(PS_SER_FUNC_DEF, PS_SER_FUNC_DEF, PS_SER_FUNC_DEF, PS_UNUSED, PS_UNUSED, PS_UNUSED)
SRV_LIST(PS_SER_SRV_FUNC_DEF, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED)
BASE_TYPES_LIST(PS_SER_FUNC_DEF)
/** @} */
#undef PS_SER_FUNC_DEF
#undef PS_SER_SRV_FUNC_DEF


/* Generate deserialization function declarations */
#define PS_DES_FUNC_DEF(TYPE, ...)                                          \
    bool ps_des_##TYPE(ucdrBuffer* reader, TYPE* msg);                      \
    bool ps_des_sequence_##TYPE(ucdrBuffer* reader, TYPE##_sequence* msg);
#define PS_DES_SRV_FUNC_DEF(TYPE, ...)                                      \
    bool ps_des_##TYPE##_request(ucdrBuffer* writer, request_##TYPE* msg);  \
    bool ps_des_##TYPE##_reply(ucdrBuffer* writer, reply_##TYPE* msg);
/**
 * @defgroup deserialization_functions Deserialization Functions
 * @ingroup picoserdes
 * @{
 */
MSG_LIST(PS_DES_FUNC_DEF, PS_DES_FUNC_DEF, PS_DES_FUNC_DEF, PS_UNUSED, PS_UNUSED, PS_UNUSED)
SRV_LIST(PS_DES_SRV_FUNC_DEF, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED)
BASE_TYPES_LIST(PS_DES_FUNC_DEF)
/** @} */
#undef PS_DES_FUNC_DEF
#undef PS_DES_SRV_FUNC_DEF


/**
 * @brief Generic serdes macros helpers
 * @{
 */
#define PS_SEL_SER(TYPE, ...)                           \
            TYPE*: ps_ser_##TYPE,                       \
            TYPE##_sequence*: ps_ser_sequence_##TYPE,
#define PS_SEL_DES(TYPE, ...)                           \
            TYPE*: ps_des_##TYPE,                       \
            TYPE##_sequence*: ps_des_sequence_##TYPE,
#define PS_SEL_SRV_SER(TYPE, ...)                       \
            request_##TYPE*: ps_ser_##TYPE##_request,   \
            reply_##TYPE*: ps_ser_##TYPE##_reply,
#define PS_SEL_SRV_DES(TYPE, ...)                       \
            request_##TYPE*: ps_des_##TYPE##_request,   \
            reply_##TYPE*: ps_des_##TYPE##_reply,

// Helpers needed for using _ps_serialize in macros given to xxx_LIST xmacros
// xxx_LIST macro expanison needs to be deffered to allow rescaning and expanding the second time
#define PS_EMPTY(...)
#define PS_DEFER(id) id PS_EMPTY()()
#define PS_OBSTRUCT(...) __VA_ARGS__ PS_DEFER(PS_EMPTY)()
#define PS_EXPAND(...) __VA_ARGS__
#define MSG_LIST_INDIRECT() MSG_LIST
#define SRV_LIST_INDIRECT() SRV_LIST
#define BASE_TYPES_LIST_INDIRECT() BASE_TYPES_LIST
#define MSG_LIST_EXPAND(...)        PS_EXPAND(MSG_LIST(__VA_ARGS__))
#define SRV_LIST_EXPAND(...)        PS_EXPAND(SRV_LIST(__VA_ARGS__))
#define BASE_TYPES_LIST_EXPAND(...) PS_EXPAND(BASE_TYPES_LIST(__VA_ARGS__))

/** @} */


/**
 * @defgroup generic_serdes_macros Generic serdes macros
 * @ingroup picoserdes
 * @{
 */

/**
 * @brief Generic serialization macro
 * @param pBUF Pointer to raw CDR message buffer
 * @param pMSG Pointer to ROS message
 * @param MAX Maximum buffer size
 * @return Size of serialized message
 */
#define ps_serialize(pBUF, pMSG, MAX) PS_EXPAND(_ps_serialize(pBUF, pMSG, MAX))
#define _ps_serialize(pBUF, pMSG, MAX)                                                              \
    ({                                                                                              \
        ucdrBuffer writer = {};                                                                     \
        *((uint32_t*)pBUF) =  0x0100; /*Little endian header*/                                      \
        ucdr_init_buffer(&writer, pBUF + sizeof(uint32_t), MAX - sizeof(uint32_t));                 \
        _Generic((pMSG),                                                                            \
            PS_DEFER(BASE_TYPES_LIST_INDIRECT)(PS_SEL_SER)                                          \
            PS_DEFER(MSG_LIST_INDIRECT)(PS_UNUSED, PS_SEL_SER, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED)     \
            PS_DEFER(SRV_LIST_INDIRECT)(PS_SEL_SRV_SER, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED) \
            default: 0                                                                              \
        )(&writer, pMSG);                                                                           \
        size_t _ret = ucdr_buffer_length(&writer) + sizeof(uint32_t);                               \
        _ret;                                                                                       \
    })
/**
 * @brief Generic deserialization macro
 * @param pBUF Pointer to raw CDR message buffer
 * @param pMSG Pointer to ROS message
 * @param MAX Maximum buffer size
 * @return true if deserialization successful
 */
#define ps_deserialize(pBUF, pMSG, MAX) PS_EXPAND(_ps_deserialize(pBUF, pMSG, MAX))
#define _ps_deserialize(pBUF, pMSG, MAX)                                                            \
    ({                                                                                              \
        ucdrBuffer reader = {};                                                                     \
        ucdr_init_buffer(&reader, pBUF + sizeof(uint32_t), MAX - sizeof(uint32_t));                 \
        bool _ok = _Generic((pMSG),                                                                 \
            PS_DEFER(BASE_TYPES_LIST_INDIRECT)(PS_SEL_DES)                                          \
            PS_DEFER(MSG_LIST_INDIRECT)(PS_UNUSED, PS_SEL_DES, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED)     \
            PS_DEFER(SRV_LIST_INDIRECT)(PS_SEL_SRV_DES, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED) \
            default: 0                                                                              \
        )(&reader, pMSG);                                                                           \
        _ok;                                                                                        \
    })

/** @} */

 /**
 * @defgroup ucdr_helpers UCDR helper functions
 * @ingroup picoserdes
 * @{
 */
/**
 * @brief Deserialize a ROS string
 * @param ub CDR buffer
 * @param pstring Pointer to string pointer
 * @return true if deserialization successful
 */
bool ucdr_deserialize_rstring(ucdrBuffer* ub, char** pstring);

/**
 * @brief Serialize a string
 * @param writer CDR buffer
 * @param pstring String to serialize
 * @return true if serialization successful
 */
bool ucdr_serialize_rstring(ucdrBuffer* writer, char* pstring);

/**
 * @brief Serialize an array of strings
 * @param ub CDR buffer
 * @param strings Array of strings
 * @param number Number of strings
 * @return true if serialization successful
 */
bool ucdr_serialize_array_rstring(ucdrBuffer* ub, char** strings, uint32_t number);

/**
* @brief Deserialize array of strings
* @param ub CDR buffer
* @param strings Array to store strings
* @param number Actual number of strings read
* @return true if deserialization successful
*/
bool ucdr_deserialize_array_rstring(ucdrBuffer* ub, char** strings, uint32_t number);

/**
* @brief Deserialize a sequence of strings
* @param ub CDR buffer
* @param strings Array to store strings
* @param max_number Maximum number of strings
* @param number Actual number of strings read
* @return true if deserialization successful
*/
bool ucdr_deserialize_sequence_rstring(ucdrBuffer* ub, char** strings, uint32_t max_number, uint32_t* number);

/**
* @brief Serialize sequence of strings
* @param ub CDR buffer
* @param strings Array to store strings
* @param number Number of strings to write
* @return true if serialization successful
*/
bool ucdr_serialize_sequence_rstring(ucdrBuffer* ub, char** strings, uint32_t number);

/**
 * @brief Start writing a sequence
 * @param ub CDR buffer
 * @return Writer context
 */
ucdr_writer_t ucdr_seq_start(ucdrBuffer* ub);

/**
 * @brief Write a string to a sequence
 * @param data Writer context
 * @param string String to write
 */
void ucdr_seq_write_str(void* data, char* string);

/**
 * @brief Set the size of a sequence
 * @param writer Writer context
 * @param len Sequence length
 */
void ucdr_seq_set_size(ucdr_writer_t* writer, size_t len);

/**
 * @brief End writing a sequence
 * @param writer Writer context
 */
void ucdr_seq_end(ucdr_writer_t* writer);
/** @} */

#ifdef __cplusplus
}

#undef ps_deserialize
#undef ps_serialize

/**
 * @defgroup generic_serdes_macros Generic serdes c++ overrides
 * @ingroup picoserdes
 * @{
 */

// Template overload generation macros
#define PS_CPP_SER_OVERLOAD(TYPE, ...)                                     \
    inline size_t ps_serialize(uint8_t* pBUF, TYPE* pMSG, size_t MAX) {    \
        ucdrBuffer writer = {};                                            \
        *((uint32_t*)pBUF) = 0x0100; /* Little endian header */            \
        ucdr_init_buffer(&writer, pBUF + sizeof(uint32_t),                 \
                        MAX - sizeof(uint32_t));                           \
        ps_ser_##TYPE(&writer, pMSG);                                      \
        return ucdr_buffer_length(&writer) + sizeof(uint32_t);             \
    }

#define PS_CPP_DES_OVERLOAD(TYPE, ...)                                     \
    inline bool ps_deserialize(uint8_t* pBUF, TYPE* pMSG, size_t MAX) {    \
        ucdrBuffer reader = {};                                            \
        ucdr_init_buffer(&reader, pBUF + sizeof(uint32_t),                 \
                        MAX - sizeof(uint32_t));                           \
        return ps_des_##TYPE(&reader, pMSG);                               \
    }


// Generate C++ overloads for all message types
BASE_TYPES_LIST(PS_CPP_SER_OVERLOAD)
BASE_TYPES_LIST(PS_CPP_DES_OVERLOAD)
MSG_LIST(PS_UNUSED, PS_CPP_SER_OVERLOAD, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED)
MSG_LIST(PS_UNUSED, PS_CPP_DES_OVERLOAD, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED)

// Generate C++ overloads for service request/reply types
#define PS_CPP_SRV_SER_OVERLOAD(TYPE, NAME, HASH, ...)                     \
    inline size_t ps_serialize(uint8_t* pBUF, request_##TYPE* pMSG, size_t MAX) { \
        ucdrBuffer writer = {};                                             \
        *((uint32_t*)pBUF) = 0x0100;                                        \
        ucdr_init_buffer(&writer, pBUF + sizeof(uint32_t),                  \
                        MAX - sizeof(uint32_t));                            \
        ps_ser_##TYPE##_request(&writer, pMSG);                             \
        return ucdr_buffer_length(&writer) + sizeof(uint32_t);              \
    }                                                                       \
    inline size_t ps_serialize(uint8_t* pBUF, reply_##TYPE* pMSG, size_t MAX) { \
        ucdrBuffer writer = {};                                             \
        *((uint32_t*)pBUF) = 0x0100;                                        \
        ucdr_init_buffer(&writer, pBUF + sizeof(uint32_t),                  \
                        MAX - sizeof(uint32_t));                            \
        ps_ser_##TYPE##_reply(&writer, pMSG);                               \
        return ucdr_buffer_length(&writer) + sizeof(uint32_t);              \
    }

#define PS_CPP_SRV_DES_OVERLOAD(TYPE, NAME, HASH, ...)                      \
    inline bool ps_deserialize(uint8_t* pBUF, request_##TYPE* pMSG, size_t MAX) { \
        ucdrBuffer reader = {};                                             \
        ucdr_init_buffer(&reader, pBUF + sizeof(uint32_t),                  \
                        MAX - sizeof(uint32_t));                            \
        return ps_des_##TYPE##_request(&reader, pMSG);                      \
    }                                                                       \
    inline bool ps_deserialize(uint8_t* pBUF, reply_##TYPE* pMSG, size_t MAX) { \
        ucdrBuffer reader = {};                                             \
        ucdr_init_buffer(&reader, pBUF + sizeof(uint32_t),                  \
                        MAX - sizeof(uint32_t));                            \
        return ps_des_##TYPE##_reply(&reader, pMSG);                        \
    }

// Generate C++ overloads for all service types
SRV_LIST(PS_CPP_SRV_SER_OVERLOAD, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED)
SRV_LIST(PS_CPP_SRV_DES_OVERLOAD, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED)

/** @} */

// Clean up the template macros
#undef PS_CPP_SER_OVERLOAD
#undef PS_CPP_DES_OVERLOAD
#undef PS_CPP_SRV_SER_OVERLOAD
#undef PS_CPP_SRV_DES_OVERLOAD

#endif

#endif /* PICOSERDES_H */
