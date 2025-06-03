/**
 ******************************************************************************
 * @file    test_picoserdes.c
 * @brief   Unit tests for picoserdes serialization/deserialization
 ******************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "../src/picoserdes.h"

// Buffer size for serialization tests
#define TEST_BUFFER_SIZE 256

// Test function declarations
void test_bool();
void test_int8();
void test_uint8();
void test_int16();
void test_uint16();
void test_int32();
void test_uint32();
void test_int64();
void test_uint64();
void test_float();
void test_double();
void test_string();

// Test implementations
void test_bool() {
    printf("Testing bool serialization/deserialization...\n");
    uint8_t buffer[TEST_BUFFER_SIZE];
    ucdrBuffer ub;
    bool original = true;
    bool deserialized = false;

    // Initialize buffer for serialization
    ucdr_init_buffer(&ub, buffer, TEST_BUFFER_SIZE);
    
    // Serialize
    ps_serialize(buffer, &original, TEST_BUFFER_SIZE);
    
    // Reset buffer for deserialization
    ucdr_init_buffer(&ub, buffer + 4, TEST_BUFFER_SIZE - 4);
    
    // Deserialize
    ps_deserialize(buffer, &deserialized, TEST_BUFFER_SIZE);
    
    // Compare
    assert(original == deserialized);
    printf("bool test passed\n");
}

void test_int8() {
    printf("Testing int8_t serialization/deserialization...\n");
    uint8_t buffer[TEST_BUFFER_SIZE];
    ucdrBuffer ub;
    int8_t original = -42;
    int8_t deserialized = 0;

    // Initialize buffer for serialization
    ucdr_init_buffer(&ub, buffer, TEST_BUFFER_SIZE);
    
    // Serialize
    ps_serialize(buffer, &original, TEST_BUFFER_SIZE);
    
    // Reset buffer for deserialization
    ucdr_init_buffer(&ub, buffer + 4, TEST_BUFFER_SIZE - 4);
    
    // Deserialize
    ps_deserialize(buffer, &deserialized, TEST_BUFFER_SIZE);
    
    // Compare
    assert(original == deserialized);
    printf("int8_t test passed\n");
}

void test_uint8() {
    printf("Testing uint8_t serialization/deserialization...\n");
    uint8_t buffer[TEST_BUFFER_SIZE];
    ucdrBuffer ub;
    uint8_t original = 200;
    uint8_t deserialized = 0;

    // Initialize buffer for serialization
    ucdr_init_buffer(&ub, buffer, TEST_BUFFER_SIZE);
    
    // Serialize
    ps_serialize(buffer, &original, TEST_BUFFER_SIZE);
    
    // Reset buffer for deserialization
    ucdr_init_buffer(&ub, buffer + 4, TEST_BUFFER_SIZE - 4);
    
    // Deserialize
    ps_deserialize(buffer, &deserialized, TEST_BUFFER_SIZE);
    
    // Compare
    assert(original == deserialized);
    printf("uint8_t test passed\n");
}

void test_int16() {
    printf("Testing int16_t serialization/deserialization...\n");
    uint8_t buffer[TEST_BUFFER_SIZE];
    ucdrBuffer ub;
    int16_t original = -12345;
    int16_t deserialized = 0;

    // Initialize buffer for serialization
    ucdr_init_buffer(&ub, buffer, TEST_BUFFER_SIZE);
    
    // Serialize
    ps_serialize(buffer, &original, TEST_BUFFER_SIZE);
    
    // Reset buffer for deserialization
    ucdr_init_buffer(&ub, buffer + 4, TEST_BUFFER_SIZE - 4);
    
    // Deserialize
    ps_deserialize(buffer, &deserialized, TEST_BUFFER_SIZE);
    
    // Compare
    assert(original == deserialized);
    printf("int16_t test passed\n");
}

void test_uint16() {
    printf("Testing uint16_t serialization/deserialization...\n");
    uint8_t buffer[TEST_BUFFER_SIZE];
    ucdrBuffer ub;
    uint16_t original = 54321;
    uint16_t deserialized = 0;

    // Initialize buffer for serialization
    ucdr_init_buffer(&ub, buffer, TEST_BUFFER_SIZE);
    
    // Serialize
    ps_serialize(buffer, &original, TEST_BUFFER_SIZE);
    
    // Reset buffer for deserialization
    ucdr_init_buffer(&ub, buffer + 4, TEST_BUFFER_SIZE - 4);
    
    // Deserialize
    ps_deserialize(buffer, &deserialized, TEST_BUFFER_SIZE);
    
    // Compare
    assert(original == deserialized);
    printf("uint16_t test passed\n");
}

void test_int32() {
    printf("Testing int32_t serialization/deserialization...\n");
    uint8_t buffer[TEST_BUFFER_SIZE];
    ucdrBuffer ub;
    int32_t original = -1234567;
    int32_t deserialized = 0;

    // Initialize buffer for serialization
    ucdr_init_buffer(&ub, buffer, TEST_BUFFER_SIZE);
    
    // Serialize
    ps_serialize(buffer, &original, TEST_BUFFER_SIZE);
    
    // Reset buffer for deserialization
    ucdr_init_buffer(&ub, buffer + 4, TEST_BUFFER_SIZE - 4);
    
    // Deserialize
    ps_deserialize(buffer, &deserialized, TEST_BUFFER_SIZE);
    
    // Compare
    assert(original == deserialized);
    printf("int32_t test passed\n");
}

void test_uint32() {
    printf("Testing uint32_t serialization/deserialization...\n");
    uint8_t buffer[TEST_BUFFER_SIZE];
    ucdrBuffer ub;
    uint32_t original = 4294967295;
    uint32_t deserialized = 0;

    // Initialize buffer for serialization
    ucdr_init_buffer(&ub, buffer, TEST_BUFFER_SIZE);
    
    // Serialize
    ps_serialize(buffer, &original, TEST_BUFFER_SIZE);
    
    // Reset buffer for deserialization
    ucdr_init_buffer(&ub, buffer + 4, TEST_BUFFER_SIZE - 4);
    
    // Deserialize
    ps_deserialize(buffer, &deserialized, TEST_BUFFER_SIZE);
    
    // Compare
    assert(original == deserialized);
    printf("uint32_t test passed\n");
}

void test_int64() {
    printf("Testing int64_t serialization/deserialization...\n");
    uint8_t buffer[TEST_BUFFER_SIZE];
    ucdrBuffer ub;
    int64_t original = -1234567890123LL;
    int64_t deserialized = 0;

    // Initialize buffer for serialization
    ucdr_init_buffer(&ub, buffer, TEST_BUFFER_SIZE);
    
    // Serialize
    ps_serialize(buffer, &original, TEST_BUFFER_SIZE);
    
    // Reset buffer for deserialization
    ucdr_init_buffer(&ub, buffer + 4, TEST_BUFFER_SIZE - 4);
    
    // Deserialize
    ps_deserialize(buffer, &deserialized, TEST_BUFFER_SIZE);
    
    // Compare
    assert(original == deserialized);
    printf("int64_t test passed\n");
}

void test_uint64() {
    printf("Testing uint64_t serialization/deserialization...\n");
    uint8_t buffer[TEST_BUFFER_SIZE];
    ucdrBuffer ub;
    uint64_t original = 18446744073709551615ULL;
    uint64_t deserialized = 0;

    // Initialize buffer for serialization
    ucdr_init_buffer(&ub, buffer, TEST_BUFFER_SIZE);
    
    // Serialize
    ps_serialize(buffer, &original, TEST_BUFFER_SIZE);
    
    // Reset buffer for deserialization
    ucdr_init_buffer(&ub, buffer + 4, TEST_BUFFER_SIZE - 4);
    
    // Deserialize
    ps_deserialize(buffer, &deserialized, TEST_BUFFER_SIZE);
    
    // Compare
    assert(original == deserialized);
    printf("uint64_t test passed\n");
}

void test_float() {
    printf("Testing float serialization/deserialization...\n");
    uint8_t buffer[TEST_BUFFER_SIZE];
    ucdrBuffer ub;
    float original = 3.14159f;
    float deserialized = 0.0f;

    // Initialize buffer for serialization
    ucdr_init_buffer(&ub, buffer, TEST_BUFFER_SIZE);
    
    // Serialize
    ps_serialize(buffer, &original, TEST_BUFFER_SIZE);
    
    // Reset buffer for deserialization
    ucdr_init_buffer(&ub, buffer + 4, TEST_BUFFER_SIZE - 4);
    
    // Deserialize
    ps_deserialize(buffer, &deserialized, TEST_BUFFER_SIZE);
    
    // Compare with small epsilon for floating point comparison
    float epsilon = 0.000001f;
    assert(fabsf(original - deserialized) < epsilon);
    printf("float test passed\n");
}

void test_double() {
    printf("Testing double serialization/deserialization...\n");
    uint8_t buffer[TEST_BUFFER_SIZE];
    ucdrBuffer ub;
    double original = 3.14159265359;
    double deserialized = 0.0;

    // Initialize buffer for serialization
    ucdr_init_buffer(&ub, buffer, TEST_BUFFER_SIZE);
    
    // Serialize
    ps_serialize(buffer, &original, TEST_BUFFER_SIZE);
    
    // Reset buffer for deserialization
    ucdr_init_buffer(&ub, buffer + 4, TEST_BUFFER_SIZE - 4);
    
    // Deserialize
    ps_deserialize(buffer, &deserialized, TEST_BUFFER_SIZE);
    
    // Compare with small epsilon for floating point comparison
    double epsilon = 0.000000001;
    assert(fabs(original - deserialized) < epsilon);
    printf("double test passed\n");
}

void test_string() {
    printf("Testing string serialization/deserialization...\n");
    uint8_t buffer[TEST_BUFFER_SIZE];
    ucdrBuffer ub;
    char* original = "Hello, World!";
    char* deserialized = NULL;

    // Initialize buffer for serialization
    ucdr_init_buffer(&ub, buffer, TEST_BUFFER_SIZE);
    
    // Serialize
    ps_serialize(buffer, &original, TEST_BUFFER_SIZE);
    
    // Reset buffer for deserialization
    ucdr_init_buffer(&ub, buffer + 4, TEST_BUFFER_SIZE - 4);
    
    // Deserialize
    ps_deserialize(buffer, &deserialized, TEST_BUFFER_SIZE);
    
    // Compare
    assert(strcmp(original, deserialized) == 0);
    printf("string test passed\n");
}

int main() {
    printf("Starting picoserdes unit tests...\n");

    // Run all tests
    test_bool();
    test_int8();
    test_uint8();
    test_int16();
    test_uint16();
    test_int32();
    test_uint32();
    test_int64();
    test_uint64();
    test_float();
    test_double();
    test_string();

    printf("All tests passed successfully!\n");
    return 0;
}