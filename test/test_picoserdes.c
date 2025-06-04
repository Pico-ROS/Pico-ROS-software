/**
 ******************************************************************************
 * @file    test_picoserdes.c
 * @brief   Unit tests for picoserdes serialization/deserialization
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "../src/picoserdes.h"

// Buffer size for serialization tests
#define TEST_BUFFER_SIZE 4096


/* Override functions for rstring */
bool ucdr_deserialize_rstring(ucdrBuffer* ub, char** pstring){
    ucdr_serialize_uint32_t(ub, 0);
    return true;
}
bool ucdr_serialize_rstring(ucdrBuffer* ub, char* pstring){
    uint32_t len = 0;
    ucdr_deserialize_uint32_t(ub, &len);
    return true;
}

// Test macro for generating type-specific test functions
#define TEST_TYPE(type, ...) \
do { \
    printf("Testing " #type " serialization/deserialization...\n"); \
    uint8_t buffer[TEST_BUFFER_SIZE]; \
    uint8_t random_value[sizeof(type)]; \
    for (size_t i = 0; i < sizeof(type); i++) { \
        random_value[i] = (uint8_t)rand(); \
    } \
    type* original = (type*)random_value; \
    type deserialized; \
    /* Serialize */ \
    ucdrBuffer writer = {}; \
    ucdr_init_buffer(&writer, buffer, TEST_BUFFER_SIZE); \
    ps_ser_##type(&writer, original); \
    /* Deserialize */ \
    ucdrBuffer reader = {}; \
    ucdr_init_buffer(&reader, buffer, TEST_BUFFER_SIZE); \
    ps_des_##type(&reader, &deserialized); \
    /* Compare */ \
    assert(memcmp(original, &deserialized, sizeof(type)) == 0); \
    printf(#type " test passed\n"); \
} while (0);


int main() {
    // Initialize random seed
    srand(time(NULL));
    
    printf("Starting picoserdes unit tests...\n");

    // Generate tests for base types
    BASE_TYPES_LIST(TEST_TYPE)

    MSG_LIST(TEST_TYPE, TEST_TYPE, PS_UNUSED, PS_UNUSED, PS_UNUSED)
    
    SRV_LIST(PS_UNUSED, TEST_TYPE, TEST_TYPE, PS_UNUSED, PS_UNUSED)

    printf("All tests passed successfully!\n");
    return 0;
}