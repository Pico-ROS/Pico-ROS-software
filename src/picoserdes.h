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

/* MSG_LIST(BTYPE, CTYPE, TTYPE, FIELD, ARRAY)
 *
 * User provided list of message types for creating serdes functions
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
 * // note: New lines need to be escaped with \
 * --------------------------------------------------------------------------
 *
 * SRV_LIST(SRV, REQUEST, REPLY, FIELD, ARRAY)
 *
 * User provided list of service types for creating serdes functions
 * SRV = Top level service name, hash and type
 * REQUEST = Request type, implemented as struct
 * REQUEST = Reply type, implemented as struct
 *      FIELD = Field of request/reply type FUNC(type, name)
 *      ARRAY = Array field of request/reply type FUNC(type, name, size)
 *
 * Each entry in table must have the following format
 * SRV(                  \
 *      type_cname,      \  // Name of type used in code
 *      type_rmw_name,   \  // Full string name of type used in RMW
 *      type_rihs_hash,  \  // Type hash string used in RMS
 *      REQUEST(         \
 *          <fields ...> \  // List of fields using FIELD macro with no commas between
 *      )                \
 *      REPLLY(          \
 *          <fields ...> \  // List of fields using FIELD macro with no commas between
 *      )                \
 * )                     \
 * // note: New lines need to be escaped with \
 *
 */
#ifndef USER_TYPE_FILE
    // empty lists
    #define MSG_LIST(BTYPE, CTYPE, TTYPE, FIELD, ARRAY)
    #define SRV_LIST(SRV, REQUEST, REPLY, FIELD, ARRAY)
#else
    // user list
    // needs to provide MSG_LIST and SRV_LIST macros
    #include USER_TYPE_FILE
#endif

/* Exported macro ------------------------------------------------------------*/
#define PS_UNUSED(...)

/* Exported types ------------------------------------------------------------*/
typedef char* rstring;


// Declare all types
#define CAT(x, y) x##y
#define FIELD_EXPAND(TYPE, NAME) TYPE NAME;
#define ARRAY_EXPAND(TYPE, NAME, SIZE) TYPE NAME[SIZE];
#define BTYPE_DECLARE(TYPE, NAME, HASH, TYPE2, ...) typedef TYPE2 TYPE;
#define CTYPE_DECLARE(TYPE, NAME, HASH, ...)    \
    typedef struct {                            \
        __VA_ARGS__                             \
    }TYPE;
#define SRV_DECLARE(TYPE, NAME, HASH, REQ, REP) CAT(REQ, TYPE); CAT(REP,TYPE);
#define REQUEST_DECLARE(...)                    \
    typedef struct {                            \
        __VA_ARGS__                             \
    }request_
#define REPLY_DECLARE(...)                      \
    typedef struct {                            \
        __VA_ARGS__                             \
    }reply_

MSG_LIST(BTYPE_DECLARE, CTYPE_DECLARE, BTYPE_DECLARE, FIELD_EXPAND, ARRAY_EXPAND)
SRV_LIST(SRV_DECLARE, REQUEST_DECLARE, REPLY_DECLARE, FIELD_EXPAND, ARRAY_EXPAND)

#undef CAT
#undef FIELD_EXPAND
#undef ARRAY_EXPAND
#undef BTYPE_DECLARE
#undef CTYPE_DECLARE
#undef SRV_DECLARE
#undef REQUEST_DECLARE
#undef REPLY_DECLARE

typedef struct{
    uint32_t*   p_size;
    ucdrBuffer* p_buffer;
    size_t      len;
}ucdr_writer_t;

/* Exported constants --------------------------------------------------------*/

// string name constants list
#define TYPE_NAME(TYPE, NAME, HASH, ...) extern char TYPE##_name[];
MSG_LIST(TYPE_NAME, TYPE_NAME, TYPE_NAME, PS_UNUSED, PS_UNUSED)
SRV_LIST(TYPE_NAME, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED)
#undef TYPE_NAME

// type hash constants list
#define TYPE_HASH(TYPE, NAME, HASH, ...) extern char TYPE##_hash[];
MSG_LIST(TYPE_HASH, TYPE_HASH, TYPE_HASH, PS_UNUSED, PS_UNUSED)
SRV_LIST(TYPE_HASH, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED)
#undef TYPE_HASH

#define ROSTYPE_NAME(TYPE) &TYPE##_name[0]
#define ROSTYPE_HASH(TYPE) &TYPE##_hash[0]

/* Exported functions --------------------------------------------------------*/

// type serialization function declarations
#define PS_SER_FUNC_DEF(TYPE, ...) \
    void ps_ser_##TYPE(ucdrBuffer* writer, TYPE* msg);
#define PS_SER_SRV_FUNC_DEF(TYPE, NAME, HASH, REQ, REP)                    \
    void ps_ser_##TYPE##_request(ucdrBuffer* writer, request_##TYPE* msg); \
    void ps_ser_##TYPE##_reply(ucdrBuffer* writer, reply_##TYPE* msg);

MSG_LIST(PS_SER_FUNC_DEF, PS_SER_FUNC_DEF, PS_SER_FUNC_DEF, PS_UNUSED, PS_UNUSED)
SRV_LIST(PS_SER_SRV_FUNC_DEF, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED)

#undef PS_SER_FUNC_DEF
#undef PS_SER_SRV_FUNC_DEF

// type derialization function declarations
#define PS_DES_FUNC_DEF(TYPE, ...) \
    void ps_des_##TYPE(ucdrBuffer* reader, TYPE* msg);
#define PS_DES_SRV_FUNC_DEF(TYPE, NAME, HASH, REQ, REP)                    \
    void ps_des_##TYPE##_request(ucdrBuffer* writer, request_##TYPE* msg); \
    void ps_des_##TYPE##_reply(ucdrBuffer* writer, reply_##TYPE* msg);

MSG_LIST(PS_DES_FUNC_DEF, PS_DES_FUNC_DEF, PS_DES_FUNC_DEF, PS_UNUSED, PS_UNUSED)
SRV_LIST(PS_DES_SRV_FUNC_DEF, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED)

#undef PS_DES_FUNC_DEF
#undef PS_DES_SRV_FUNC_DEF

// Generic serdes function macros
// pBUF = uint8_t* pointer to start of raw cdr message buffer
// pMSG = *ros_xxx type pointer
// MAX = max size of buffer
#define PS_SEL_SER(TYPE, ...)  TYPE*: ps_ser_##TYPE,
#define PS_SEL_DES(TYPE, ...)  TYPE*: ps_des_##TYPE,
#define PS_SEL_SRV_SER(TYPE, NAME, HASH, REQ, REP)      \
            request_##TYPE*: ps_ser_##TYPE##_request,   \
            reply_##TYPE*: ps_ser_##TYPE##_reply,
#define PS_SEL_SRV_DES(TYPE, NAME, HASH, REQ, REP)      \
            request_##TYPE*: ps_des_##TYPE##_request,   \
            reply_##TYPE*: ps_des_##TYPE##_reply,

#define ps_serialize(pBUF, pMSG, MAX)                                                   \
    ({                                                                                  \
        ucdrBuffer writer = {};                                                         \
        ucdr_init_buffer(&writer, pBUF, MAX);                                           \
        ucdr_serialize_uint32_t(&writer,  0x0100); /*Little endian */                   \
        _Generic((pMSG),                                                                \
            MSG_LIST(PS_SEL_SER, PS_SEL_SER, PS_UNUSED, PS_UNUSED, PS_UNUSED)           \
            SRV_LIST(PS_SEL_SRV_SER, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED)        \
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
            MSG_LIST(PS_SEL_DES, PS_SEL_DES, PS_UNUSED, PS_UNUSED, PS_UNUSED)           \
            SRV_LIST(PS_SEL_SRV_DES, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED)        \
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
