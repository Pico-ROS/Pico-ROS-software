/**
  ******************************************************************************
  * @file    picoserdes.h
  * @date    2025-May-31
  * @brief   Pico CDR serdes
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
 *          <fields ...> \  // List of fields using FIELD macro with no commas between
 *      )                \
 *      REPLLY(          \
 *          <fields ...> \  // List of fields using FIELD macro with no commas between
 *      )                \
 * )                     \
 * note: New lines need to be escaped with \
 *       Name of generated request/reply types is request_<srv_name> and reply_<srv_name>
 */
#ifndef USER_TYPE_FILE
    // empty lists
    #define MSG_LIST(BTYPE, CTYPE, TTYPE, FIELD, ARRAY)
    #define SRV_LIST(SRV, REQUEST, REPLY, FIELD, ARRAY)
#else
    // user needs to provide MSG_LIST and SRV_LIST macros
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
#define PS_SER_FUNC_DEF(TYPE, ...)                                          \
    bool ps_ser_##TYPE(ucdrBuffer* writer, TYPE* msg);
#define PS_SER_SRV_FUNC_DEF(TYPE, ...)                                      \
    bool ps_ser_##TYPE##_request(ucdrBuffer* writer, request_##TYPE* msg);  \
    bool ps_ser_##TYPE##_reply(ucdrBuffer* writer, reply_##TYPE* msg);
MSG_LIST(PS_SER_FUNC_DEF, PS_SER_FUNC_DEF, PS_SER_FUNC_DEF, PS_UNUSED, PS_UNUSED)
SRV_LIST(PS_SER_SRV_FUNC_DEF, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED)
#undef PS_SER_FUNC_DEF
#undef PS_SER_SRV_FUNC_DEF

// type derialization function declarations
#define PS_DES_FUNC_DEF(TYPE, ...)                                          \
    bool ps_des_##TYPE(ucdrBuffer* reader, TYPE* msg);
#define PS_DES_SRV_FUNC_DEF(TYPE, ...)                                      \
    bool ps_des_##TYPE##_request(ucdrBuffer* writer, request_##TYPE* msg);  \
    bool ps_des_##TYPE##_reply(ucdrBuffer* writer, reply_##TYPE* msg);
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
#define PS_SEL_SRV_SER(TYPE, ...)                       \
            request_##TYPE*: ps_ser_##TYPE##_request,   \
            reply_##TYPE*: ps_ser_##TYPE##_reply,
#define PS_SEL_SRV_DES(TYPE, ...)                       \
            request_##TYPE*: ps_des_##TYPE##_request,   \
            reply_##TYPE*: ps_des_##TYPE##_reply,

#define ps_serialize(pBUF, pMSG, MAX)                                                   \
    ({                                                                                  \
        ucdrBuffer writer = {};                                                         \
        *((uint32_t*)pBUF) =  0x0100; /*Little endian header*/                          \
        ucdr_init_buffer(&writer, pBUF+4, MAX-4);                                       \
        _Generic((pMSG),                                                                \
            MSG_LIST(PS_SEL_SER, PS_SEL_SER, PS_UNUSED, PS_UNUSED, PS_UNUSED)           \
            SRV_LIST(PS_SEL_SRV_SER, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED)        \
            default: 0                                                                  \
        )(&writer, pMSG);                                                               \
        size_t _ret = ucdr_buffer_length(&writer) + 4;                                  \
        _ret;                                                                           \
    })

#define ps_deserialize(pBUF, pMSG, MAX)                                                 \
    ({                                                                                  \
        ucdrBuffer reader = {};                                                         \
        ucdr_init_buffer(&reader, pBUF + sizeof(uint32_t), MAX - sizeof(uint32_t));     \
        bool _ok = _Generic((pMSG),                                                     \
            MSG_LIST(PS_SEL_DES, PS_SEL_DES, PS_UNUSED, PS_UNUSED, PS_UNUSED)           \
            SRV_LIST(PS_SEL_SRV_DES, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED)        \
            default: 0                                                                  \
        )(&reader, pMSG);                                                               \
        _ok;                                                                            \
    })


bool            ucdr_deserialize_rstring(ucdrBuffer* ub, char** pstring);
bool            ucdr_serialize_rstring(ucdrBuffer* writer, char* pstring);
bool            ucdr_serialize_array_rstring(ucdrBuffer* ub, char** strings, uint32_t number);
bool            ucdr_deserialize_sequence_rstring(ucdrBuffer* ub, char** strings, uint32_t max_number, uint32_t* number);

ucdr_writer_t   ucdr_seq_start(ucdrBuffer* ub);
void            ucdr_seq_write_str(void* data, char* string);
void            ucdr_seq_set_size(ucdr_writer_t* writer, size_t len);
void            ucdr_seq_end(ucdr_writer_t* writer);

#ifdef __cplusplus
}
#endif

#endif /* PICOSERDES_H */
