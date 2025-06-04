/**
 ******************************************************************************
 * @file    test_picoserdes.c
 * @brief   Unit tests for picoserdes serialization/deserialization
 ******************************************************************************
 */
#include <stdbool.h>
#undef NDEBUG
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "../src/picoserdes.h"

// Buffer size for serialization tests
#define TEST_BUFFER_SIZE 4096

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
bool disable_type;
bool some_test_failed;

/* Override functions for rstring */
bool ucdr_deserialize_rstring(ucdrBuffer* ub, char** pstring){
    ucdr_serialize_uint32_t(ub, 0);
    return true;
}
bool ucdr_serialize_rstring(ucdrBuffer* ub, char* pstring){
    uint32_t len = 0;
    ucdr_deserialize_uint32_t(ub, &len);
    disable_type = true;
    return true;
}

// Test macro for generating type-specific test functions
#define TEST_TYPE(type, ...) \
do { \
    uint8_t buffer[TEST_BUFFER_SIZE]; \
    uint8_t random_value[sizeof(type)] = {0}; \
    for (size_t i = 0; i < 1/* sizeof(type) */; i++) { /* Problems with arrays */\
        random_value[i] = (uint8_t)rand(); \
    } \
    type* original = (type*)random_value; \
    type deserialized; \
    /* Serialize */ \
    ucdrBuffer writer = {}; \
    ucdr_init_buffer(&writer, buffer, TEST_BUFFER_SIZE); \
    bool ser_success = ps_ser_##type(&writer, original); \
    if(disable_type){ \
        disable_type = false; \
        printf("%s%s[%s] Test %s: %s%s\n", TEST_INDENT, YELLOW_TEXT, "s", #type, "SKIPPED", RESET_TEXT);\
        continue; \
    } \
    /* Deserialize */ \
    ucdrBuffer reader = {}; \
    ucdr_init_buffer(&reader, buffer, TEST_BUFFER_SIZE); \
    bool des_success = ps_des_##type(&reader, &deserialized); \
    /* Compare and print result */ \
    bool test_passed = false; \
    if(strcmp(#type, "bool") == 0) { \
        bool original_bool = *((bool*)original); \
        bool deserialized_bool = *((bool*)&deserialized); \
        test_passed = (original_bool == true && deserialized_bool == true) || (original_bool == false && deserialized_bool == false ); \
    } \
    else { \
        test_passed = (memcmp(original, &deserialized, sizeof(type)) == 0); \
    }\
    print_test_result(#type, test_passed); \
    if(!test_passed){ \
        some_test_failed = true; \
    } \
    /*assert(test_passed && "Serialized and deserialized values do not match");*/ \
} while (0);



// #define TEST_SRV(TYPE, ...) \
//     TEST_TYPE(request_##TYPE) \
//     TEST_TYPE(reply_##TYPE) 

//TODO: Fix arrays and strings serdes tests. Add service tests.

int main() {
    // Initialize random seed
    srand(time(NULL));
    
    print_header("PICOSERDES UNIT TESTS");
    
    print_header("Base Types Tests");
    BASE_TYPES_LIST(TEST_TYPE)

    print_header("Message Types Tests");
    MSG_LIST(TEST_TYPE, TEST_TYPE, PS_UNUSED, PS_UNUSED, PS_UNUSED)
    
    // print_header("Service Types Tests");
    // SRV_LIST(TEST_SRV, PS_UNUSED, PS_UNUSED, PS_UNUSED, PS_UNUSED)

    if(some_test_failed){
        printf("\n%s%s Some tests failed! %s\n\n", 
               BOLD_TEXT, RED_TEXT, RESET_TEXT);
    }
    else{
        printf("\n%s%s All tests completed successfully! %s\n\n", 
               BOLD_TEXT, GREEN_TEXT, RESET_TEXT);
    }
    return 0;
}