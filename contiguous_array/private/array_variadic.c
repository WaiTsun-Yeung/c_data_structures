#include <stdarg.h>
#include <stdint.h>
#include <stdalign.h>

#include "status.h"
#include "alloc.h"
#include "array_type.h"
#include "array.h"

/// @brief Create a heap-allocated uintmax_t cds_array 
//     with the specified values.
/// @param[in] values_count The number of values to be stored in the array.
/// @param[in] ... The values to be stored in the array.
/// @return The created array.
struct cds_array* cds_create_uintmax_t_array(
    enum cds_status* const return_state, const size_t values_count, ...
){
    struct cds_array* array 
        = cds_create_array(
            values_count, sizeof(uintmax_t), alignof(uintmax_t), return_state
        );
    if (!array) return array;
    va_list args;
    va_start(args, values_count);
    uintmax_t* data = cds_data(array);
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
struct cds_array* cds_create_int_array(
    enum cds_status* const return_state, const size_t values_count, ...
){
    struct cds_array* array 
        = cds_create_array(
            values_count, sizeof(int), alignof(int), return_state
        );
    if (!array) return array;
    va_list args;
    va_start(args, values_count);
    int* data = cds_data(array);
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
struct cds_array* cds_create_intmax_t_array(
    enum cds_status* const return_state, const size_t values_count, ...
){
    struct cds_array* array 
        = cds_create_array(
            values_count, sizeof(intmax_t), alignof(intmax_t), return_state
        );
    if (!array) return array;
    va_list args;
    va_start(args, values_count);
    intmax_t* data = cds_data(array);
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
struct cds_array* cds_create_uint8_t_array(
    enum cds_status* const return_state, const size_t values_count, ...
){
    struct cds_array* array 
        = cds_create_array(
            values_count, sizeof(uint8_t), alignof(uint8_t), return_state
        );
    if (!array) return array;
    va_list args;
    va_start(args, values_count);
    uint8_t* data = cds_data(array);
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
struct cds_array* cds_create_uint16_t_array(
    enum cds_status* const return_state, const size_t values_count, ...
){
    struct cds_array* array 
        = cds_create_array(
            values_count, sizeof(uint16_t), alignof(uint16_t), return_state
        );
    if (!array) return array;
    va_list args;
    va_start(args, values_count);
    uint16_t* data = cds_data(array);
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
struct cds_array* cds_create_uint32_t_array(
    enum cds_status* const return_state, const size_t values_count, ...
){
    struct cds_array* array 
        = cds_create_array(
            values_count, sizeof(uint32_t), alignof(uint32_t), return_state
        );
    if (!array) return array;
    va_list args;
    va_start(args, values_count);
    uint32_t* data = cds_data(array);
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
struct cds_array* cds_create_uint64_t_array(
    enum cds_status* const return_state, const size_t values_count, ...
){
    struct cds_array* array 
        = cds_create_array(
            values_count, sizeof(uint64_t), alignof(uint64_t), return_state
        );
    va_list args;
    va_start(args, values_count);
    uint64_t* data = cds_data(array);
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
struct cds_array* cds_create_int8_t_array(
    enum cds_status* const return_state, const size_t values_count, ...
){
    struct cds_array* array 
        = cds_create_array(
            values_count, sizeof(int8_t), alignof(int8_t), return_state
        );
    va_list args;
    va_start(args, values_count);
    int8_t* data = cds_data(array);
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
struct cds_array* cds_create_int16_t_array(
    enum cds_status* const return_state, const size_t values_count, ...){
    struct cds_array* array 
        = cds_create_array(
            values_count, sizeof(int16_t), alignof(int16_t), return_state
        );
    va_list args;
    va_start(args, values_count);
    int16_t* data = cds_data(array);
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
struct cds_array* cds_create_int32_t_array(
    enum cds_status* const return_state, const size_t values_count, ...
){
    struct cds_array* array 
        = cds_create_array(
            values_count, sizeof(int32_t), alignof(int32_t), return_state
        );
    va_list args;
    va_start(args, values_count);
    int32_t* data = cds_data(array);
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
struct cds_array* cds_create_int64_t_array(
    enum cds_status* const return_state, const size_t values_count, ...
){
    struct cds_array* array 
        = cds_create_array(
            values_count, sizeof(int64_t), alignof(int64_t), return_state
        );
    va_list args;
    va_start(args, values_count);
    int64_t* data = cds_data(array);
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
struct cds_array* cds_create_float_array(
    enum cds_status* const return_state, const size_t values_count, ...
){
    struct cds_array* array 
        = cds_create_array(
            values_count, sizeof(float), alignof(float), return_state
        );
    va_list args;
    va_start(args, values_count);
    float* data = cds_data(array);
    for (size_t i = 0; i < values_count; ++i, ++data)
    // The float values are promoted to double when retrieved from va_arg.
    // See: https://stackoverflow.com/a/11270603/12483740
        *data = (float)va_arg(args, double);
    va_end(args);
    return array;
}

/// @brief Create a heap-allocated double cds_array
//     with the specified values.
/// @param[in] values_count The number of values to be stored in the array.
/// @param[in] ... The values to be stored in the array.
/// @return The created array.
struct cds_array* cds_create_double_array(
    enum cds_status* const return_state, const size_t values_count, ...
){
    struct cds_array* array 
        = cds_create_array(
            values_count, sizeof(double), alignof(double), return_state
        );
    va_list args;
    va_start(args, values_count);
    double* data = cds_data(array);
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
struct cds_array* cds_create_long_double_array(
    enum cds_status* const return_state, const size_t values_count, ...
){
    struct cds_array* array 
        = cds_create_array(
            values_count, sizeof(long double), alignof(long double), 
            return_state
        );
    va_list args;
    va_start(args, values_count);
    long double* data = cds_data(array);
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
struct cds_array* cds_create_char_array(
    enum cds_status* const return_state, const size_t values_count, ...
){
    struct cds_array* array 
        = cds_create_array(
            values_count, sizeof(char), alignof(char), return_state
        );
    va_list args;
    va_start(args, values_count);
    char* data = cds_data(array);
    for (size_t i = 0; i < values_count; ++i, ++data)
        *data = (char)va_arg(args, int);
    va_end(args);
    return array;
}

/// @brief Create a heap-allocated unsigned char cds_array
//     with the specified values.
/// @param[in] values_count The number of values to be stored in the array.
/// @param[in] ... The values to be stored in the array.
/// @return The created array.
struct cds_array* cds_create_unsigned_char_array(
    enum cds_status* const return_state, const size_t values_count, ...
){
    struct cds_array* array 
        = cds_create_array(
            values_count, sizeof(unsigned char), alignof(unsigned char), 
            return_state
        );
    va_list args;
    va_start(args, values_count);
    unsigned char* data = cds_data(array);
    for (size_t i = 0; i < values_count; ++i, ++data)
        *data = (unsigned char)va_arg(args, int);
    va_end(args);
    return array;
}

/// @brief Create a heap-allocated signed char cds_array
//     with the specified values.
/// @param[in] values_count The number of values to be stored in the array.
/// @param[in] ... The values to be stored in the array.
/// @return The created array.
struct cds_array* cds_create_signed_char_array(
    enum cds_status* const return_state, const size_t values_count, ...
){
    struct cds_array* array 
        = cds_create_array(
            values_count, sizeof(signed char), alignof(signed char), 
            return_state
        );
    va_list args;
    va_start(args, values_count);
    signed char* data = cds_data(array);
    for (size_t i = 0; i < values_count; ++i, ++data)
        *data = (signed char)va_arg(args, int);
    va_end(args);
    return array;
}