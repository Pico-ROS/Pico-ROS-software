/**
 ******************************************************************************
 * @file    test_picoserdes.c
 * @brief   Unit tests for picoserdes serialization/deserialization
 ******************************************************************************
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../src/picoserdes.h"

#undef NDEBUG

// Buffer size for serialization tests allocated on stack
#define TEST_BUFFER_SIZE 1024

// Formatting constants
#define HEADER_WIDTH 80
#define TEST_INDENT "    "
#define GREEN_TEXT "\033[0;32m"
#define RED_TEXT   "\033[0;31m"
#define YELLOW_TEXT "\033[0;33m"
#define RESET_TEXT "\033[0m"
#define BOLD_TEXT  "\033[1m"

// Helper functions for formatting
void print_header(const char* title) {
    printf("%s", BOLD_TEXT);
    //printf("═══════════════════════════════════════════════════════════════════════════════\n");
    printf("  %s\n", title);
    //printf("═══════════════════════════════════════════════════════════════════════════════\n");
    printf("%s", RESET_TEXT);
}

void print_test_result(const char* type_name, bool passed) {
    printf("%s%s[%s] Test %s: %s%s\n",
           TEST_INDENT,
           passed ? GREEN_TEXT : RED_TEXT,
           passed ? "✓" : "✗",
           type_name,
           passed ? "PASSED" : "FAILED",
           RESET_TEXT);
}


/* Test macro for generating type-specific test functions.
 * Works by:
 *      1. Serializing constant value of #type to buffer1
 *      2. Deserializing to new variable of #type from buffer1
 *      3. Serializing new variable to buffer2
 *      4. Compare buffer1 & buffer2
 * Comparing new variable to constant value fails if type includes strings.
 */
#define TEST_TYPE(type, ...) \
    do { \
        uint8_t buffer[TEST_BUFFER_SIZE]; \
        uint8_t buffer2[TEST_BUFFER_SIZE]; \
        type* original = &test_##type; \
        type deserialized; \
        /* Serialize */ \
        size_t len = _ps_serialize(buffer, original, TEST_BUFFER_SIZE); \
        /* Deserialize */ \
        _ps_deserialize(buffer, &deserialized, TEST_BUFFER_SIZE); \
        /* Serialize deserialized */ \
        _ps_serialize(buffer2, &deserialized, TEST_BUFFER_SIZE); \
        /* Compare serialized buffers and print result */ \
        bool test_passed = (memcmp(buffer, buffer2, len) == 0); \
        print_test_result(#type, test_passed); \
        if(!test_passed){ \
            some_test_failed = true; \
            printf("\n Original: \n"); \
            for (int i = 0; i < len; i++){\
                printf("%02x ", buffer[i]); \
                if((i+1)%32 == 0){printf("\n");}\
            }\
            printf("\n Deserialized: \n"); \
            for (int i = 0; i < len; i++){\
                printf("%02x ", buffer2[i]); \
                if((i+1)%32 == 0){printf("\n");}\
            }\
            printf("\n\n"); \
        } \
    } while (0);

/* Helper macros for constant values generation */
#define MAKE_TEST_BTYPE(TYPE, NAME, HASH, TYPE2, ...)  \
    TYPE test_##TYPE = test_##TYPE2;

#define MAKE_TEST_FIELD(TYPE, NAME) \
    .NAME = test_##TYPE,

#define MAKE_TEST_ARRAY(TYPE, NAME, SIZE) \
    .NAME = {1, 2},

#define MAKE_TEST_CTYPE(TYPE, NAME, HASH, ...)  \
    TYPE test_##TYPE = {  __VA_ARGS__ };


#define MAKE_SRV_TEST(TYPE, NAME, HASH, REQ, REP) \
    request_##TYPE test_request_##TYPE = {  REQ }; \
    reply_##TYPE test_reply_##TYPE = {  REP };

#define TEST_SRV(TYPE, ...) \
    TEST_TYPE(request_##TYPE) \
    TEST_TYPE(reply_##TYPE)


int main() {
    print_header("PICOSERDES UNIT TESTS");
    bool some_test_failed = false;

    // Input data - base types
    bool test_bool = true;
    int8_t test_int8_t = -11;
    uint8_t test_uint8_t = 201;
    int16_t test_int16_t = -30000;
    uint16_t test_uint16_t = 30000;
    int32_t test_int32_t = 0xFEFEFEFE;
    uint32_t test_uint32_t = 0xFEFEFEFE;
    int64_t test_int64_t  = 0xFEFEFEFEFEFEFEFE;
    uint64_t test_uint64_t = 0xFEFEFEFEFEFEFEFE;
    float test_float= 0.5f;
    double test_double = 0.12345;
    char* test_rstring = "This is a test string";

    // Input data - user types - link base types above
    MSG_LIST_EXPAND(MAKE_TEST_BTYPE, MAKE_TEST_CTYPE, MAKE_TEST_BTYPE, MAKE_TEST_FIELD, MAKE_TEST_ARRAY)
    SRV_LIST_EXPAND(MAKE_SRV_TEST, PS_EXPAND, PS_EXPAND, MAKE_TEST_FIELD, MAKE_TEST_ARRAY)

    print_header("Base Types Tests:");
    BASE_TYPES_LIST_EXPAND(TEST_TYPE)

    print_header("Message Types Tests:");
    MSG_LIST_EXPAND(PS_UNUSED, TEST_TYPE, PS_UNUSED, PS_UNUSED, PS_UNUSED)

    print_header("Service Types Tests:");
    SRV_LIST_EXPAND(TEST_SRV, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED)

    if(some_test_failed){
        printf("\n%s%s Some tests failed! %s\n\n",
               BOLD_TEXT, RED_TEXT, RESET_TEXT);
        return EXIT_FAILURE;
    }
    else{
        printf("\n%s%s All tests completed successfully! %s\n\n",
               BOLD_TEXT, GREEN_TEXT, RESET_TEXT);
        return EXIT_SUCCESS;
    }
}
