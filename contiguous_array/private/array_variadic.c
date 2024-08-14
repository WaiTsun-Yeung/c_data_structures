#include <stdarg.h>
#include <stdint.h>

#include "array_type.h"
#include "array.h"

/// @brief Create a heap-allocated uintmax_t cds_array 
//     with the specified values.
/// @param[in] values_count The number of values to be stored in the array.
/// @param[in] ... The values to be stored in the array.
/// @return The created array.
struct cds_array* cds_create_uintmax_t_array(const size_t values_count, ...){
    struct cds_array* array = cds_create_array(values_count, sizeof(uintmax_t));
    va_list args;
    va_start(args, values_count);
    uintmax_t* data = array->data;
    for (size_t i = 0; i < values_count; ++i, ++data)
        *data = va_arg(args, uintmax_t);
    va_end(args);
    return array;
}

/// @brief Create a heap-allocated int cds_array
//     with the specified values.
/// @param[in] values_count The number of values to be stored in the array.
/// @param[in] ... The values to be stored in the array.
/// @return The created array.
struct cds_array* cds_create_int_array(const size_t values_count, ...){
    struct cds_array* array = cds_create_array(values_count, sizeof(int));
    va_list args;
    va_start(args, values_count);
    int* data = array->data;
    for (size_t i = 0; i < values_count; ++i, ++data)
        *data = va_arg(args, int);
    va_end(args);
    return array;
}

/// @brief Create a heap-allocated intmax_t cds_array
//     with the specified values.
/// @param[in] values_count The number of values to be stored in the array.
/// @param[in] ... The values to be stored in the array.
/// @return The created array.
struct cds_array* cds_create_intmax_t_array(const size_t values_count, ...){
    struct cds_array* array = cds_create_array(values_count, sizeof(intmax_t));
    va_list args;
    va_start(args, values_count);
    intmax_t* data = array->data;
    for (size_t i = 0; i < values_count; ++i, ++data)
        *data = va_arg(args, intmax_t);
    va_end(args);
    return array;
}

/// @brief Create a heap-allocated uint8_t cds_array
//     with the specified values.
/// @param[in] values_count The number of values to be stored in the array.
/// @param[in] ... The values to be stored in the array.
/// @return The created array.
struct cds_array* cds_create_uint8_t_array(const size_t values_count, ...){
    struct cds_array* array = cds_create_array(values_count, sizeof(uint8_t));
    va_list args;
    va_start(args, values_count);
    uint8_t* data = array->data;
    for (size_t i = 0; i < values_count; ++i, ++data)
        *data = va_arg(args, uint8_t);
    va_end(args);
    return array;
}

/// @brief Create a heap-allocated uint16_t cds_array
//     with the specified values.
/// @param[in] values_count The number of values to be stored in the array.
/// @param[in] ... The values to be stored in the array.
/// @return The created array.
struct cds_array* cds_create_uint16_t_array(const size_t values_count, ...){
    struct cds_array* array = cds_create_array(values_count, sizeof(uint16_t));
    va_list args;
    va_start(args, values_count);
    uint16_t* data = array->data;
    for (size_t i = 0; i < values_count; ++i, ++data)
        *data = va_arg(args, uint16_t);
    va_end(args);
    return array;
}

/// @brief Create a heap-allocated uint32_t cds_array
//     with the specified values.
/// @param[in] values_count The number of values to be stored in the array.
/// @param[in] ... The values to be stored in the array.
/// @return The created array.
struct cds_array* cds_create_uint32_t_array(const size_t values_count, ...){
    struct cds_array* array = cds_create_array(values_count, sizeof(uint32_t));
    va_list args;
    va_start(args, values_count);
    uint32_t* data = array->data;
    for (size_t i = 0; i < values_count; ++i, ++data)
        *data = va_arg(args, uint32_t);
    va_end(args);
    return array;
}

/// @brief Create a heap-allocated uint64_t cds_array
//     with the specified values.
/// @param[in] values_count The number of values to be stored in the array.
/// @param[in] ... The values to be stored in the array.
/// @return The created array.
struct cds_array* cds_create_uint64_t_array(const size_t values_count, ...){
    struct cds_array* array = cds_create_array(values_count, sizeof(uint64_t));
    va_list args;
    va_start(args, values_count);
    uint64_t* data = array->data;
    for (size_t i = 0; i < values_count; ++i, ++data)
        *data = va_arg(args, uint64_t);
    va_end(args);
    return array;
}

/// @brief Create a heap-allocated int8_t cds_array
//     with the specified values.
/// @param[in] values_count The number of values to be stored in the array.
/// @param[in] ... The values to be stored in the array.
/// @return The created array.
struct cds_array* cds_create_int8_t_array(const size_t values_count, ...){
    struct cds_array* array = cds_create_array(values_count, sizeof(int8_t));
    va_list args;
    va_start(args, values_count);
    int8_t* data = array->data;
    for (size_t i = 0; i < values_count; ++i, ++data)
        *data = va_arg(args, int8_t);
    va_end(args);
    return array;
}

/// @brief Create a heap-allocated int16_t cds_array
//     with the specified values.
/// @param[in] values_count The number of values to be stored in the array.
/// @param[in] ... The values to be stored in the array.
/// @return The created array.
struct cds_array* cds_create_int16_t_array(const size_t values_count, ...){
    struct cds_array* array = cds_create_array(values_count, sizeof(int16_t));
    va_list args;
    va_start(args, values_count);
    int16_t* data = array->data;
    for (size_t i = 0; i < values_count; ++i, ++data)
        *data = va_arg(args, int16_t);
    va_end(args);
    return array;
}

/// @brief Create a heap-allocated int32_t cds_array
//     with the specified values.
/// @param[in] values_count The number of values to be stored in the array.
/// @param[in] ... The values to be stored in the array.
/// @return The created array.
struct cds_array* cds_create_int32_t_array(const size_t values_count, ...){
    struct cds_array* array = cds_create_array(values_count, sizeof(int32_t));
    va_list args;
    va_start(args, values_count);
    int32_t* data = array->data;
    for (size_t i = 0; i < values_count; ++i, ++data)
        *data = va_arg(args, int32_t);
    va_end(args);
    return array;
}

/// @brief Create a heap-allocated int64_t cds_array
//     with the specified values.
/// @param[in] values_count The number of values to be stored in the array.
/// @param[in] ... The values to be stored in the array.
/// @return The created array.
struct cds_array* cds_create_int64_t_array(const size_t values_count, ...){
    struct cds_array* array = cds_create_array(values_count, sizeof(int64_t));
    va_list args;
    va_start(args, values_count);
    int64_t* data = array->data;
    for (size_t i = 0; i < values_count; ++i, ++data)
        *data = va_arg(args, int64_t);
    va_end(args);
    return array;
}

/// @brief Create a heap-allocated float cds_array
//     with the specified values.
/// @param[in] values_count The number of values to be stored in the array.
/// @param[in] ... The values to be stored in the array.
/// @return The created array.
struct cds_array* cds_create_float_array(const size_t values_count, ...){
    struct cds_array* array = cds_create_array(values_count, sizeof(float));
    va_list args;
    va_start(args, values_count);
    float* data = array->data;
    for (size_t i = 0; i < values_count; ++i, ++data)
        *data = va_arg(args, float);
    va_end(args);
    return array;
}

/// @brief Create a heap-allocated double cds_array
//     with the specified values.
/// @param[in] values_count The number of values to be stored in the array.
/// @param[in] ... The values to be stored in the array.
/// @return The created array.
struct cds_array* cds_create_double_array(const size_t values_count, ...){
    struct cds_array* array = cds_create_array(values_count, sizeof(double));
    va_list args;
    va_start(args, values_count);
    double* data = array->data;
    for (size_t i = 0; i < values_count; ++i, ++data)
        *data = va_arg(args, double);
    va_end(args);
    return array;
}

/// @brief Create a heap-allocated long double cds_array
//     with the specified values.
/// @param[in] values_count The number of values to be stored in the array.
/// @param[in] ... The values to be stored in the array.
/// @return The created array.
struct cds_array* cds_create_long_double_array(const size_t values_count, ...){
    struct cds_array* array 
        = cds_create_array(values_count, sizeof(long double));
    va_list args;
    va_start(args, values_count);
    long double* data = array->data;
    for (size_t i = 0; i < values_count; ++i, ++data)
        *data = va_arg(args, long double);
    va_end(args);
    return array;
}

/// @brief Create a heap-allocated char cds_array
//     with the specified values.
/// @param[in] values_count The number of values to be stored in the array.
/// @param[in] ... The values to be stored in the array.
/// @return The created array.
struct cds_array* cds_create_char_array(const size_t values_count, ...){
    struct cds_array* array = cds_create_array(values_count, sizeof(char));
    va_list args;
    va_start(args, values_count);
    char* data = array->data;
    for (size_t i = 0; i < values_count; ++i, ++data)
        *data = va_arg(args, int);
    va_end(args);
    return array;
}

/// @brief Create a heap-allocated unsigned char cds_array
//     with the specified values.
/// @param[in] values_count The number of values to be stored in the array.
/// @param[in] ... The values to be stored in the array.
/// @return The created array.
struct cds_array* cds_create_unsigned_char_array(
    const size_t values_count, ...
){
    struct cds_array* array 
        = cds_create_array(values_count, sizeof(unsigned char));
    va_list args;
    va_start(args, values_count);
    unsigned char* data = array->data;
    for (size_t i = 0; i < values_count; ++i, ++data)
        *data = va_arg(args, int);
    va_end(args);
    return array;
}

/// @brief Create a heap-allocated signed char cds_array
//     with the specified values.
/// @param[in] values_count The number of values to be stored in the array.
/// @param[in] ... The values to be stored in the array.
/// @return The created array.
struct cds_array* cds_create_signed_char_array(const size_t values_count, ...){
    struct cds_array* array 
        = cds_create_array(values_count, sizeof(signed char));
    va_list args;
    va_start(args, values_count);
    signed char* data = array->data;
    for (size_t i = 0; i < values_count; ++i, ++data)
        *data = va_arg(args, int);
    va_end(args);
    return array;
}