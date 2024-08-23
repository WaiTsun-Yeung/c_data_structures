#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "utilities.h"
#include "array_type.h"

//TODO: Attemp implement most if not all methods in std::vector.\
//TODO: Implement range-based copy constructor.
//TODO: Implement array_view.
//TODO: Create an iterator struct with associated functions. The iterator struct 
// should reference its parent container and cleanup of the parent container 
// should also cleanup the iterator.

/// @brief Compute the smallest power of two that is greater than n.
/// @param[in] n 
/// @return The smallest power of two that is greater than n.
static size_t cds_next_power_of_two(const size_t n){
    size_t power = 1;
    while(power < n) power <<= 1;
    return power;
}

/// @brief Compute the buffer length required to allocate the cds_array struct.
/// @param[in] reserved_length The number of elements of the data buffer 
///     to be reserved.
/// @param[in] bytes_per_element The number of bytes per intended 
///     data element type.
/// @return The buffer length required to allocate the cds_array struct.
static inline size_t cds_compute_array_buffer_length(
    const size_t reserved_length, const size_t bytes_per_element
){
    return sizeof(struct cds_array) + reserved_length * bytes_per_element;
}

/// @brief This function heap-allocates an empty cds_array struct without
///     instantiating any of its fields, including the data pointer.
///     This function is meant for internal use only.
/// @param[in] reserved_length The number of elements of the data buffer 
///     to be reserved.
/// @param[in] bytes_per_element The number of bytes per element of the intended
///     data type.
/// @return The allocated buffer.
static inline struct cds_array* cds_malloc_array(
    const size_t reserved_length, const size_t bytes_per_element
){
    return cds_malloc_buffer(
        cds_compute_array_buffer_length(reserved_length, bytes_per_element)
    );
}

/// @brief Create a heap-allocated cds_array.
/// @param[in] length The number of data elements.
/// @param[in] bytes_per_element The number of bytes per element of the intended
///     data type.
/// @return The created array.
///     The user should free the array with cds_destroy_array()
///     at the end of its lifetime to prevent memory leaks.
struct cds_array* cds_create_array(
    const size_t length, const size_t bytes_per_element
){
    const size_t reserved_length = cds_next_power_of_two(length);
    struct cds_array* const array 
        = cds_malloc_array(reserved_length, bytes_per_element);
    array->data_length = length;
    array->reserved_length = reserved_length;
    array->bytes_per_element = bytes_per_element;
    array->data = (uint8_t*)array + sizeof(struct cds_array);
    return array;
}

/// @brief Copy src to create a new heap-allocated cds_array.
/// @param[in] src The source cds_array to copy from.
/// @return The copied array.
///     The user should free the array with cds_destroy_array()
///     at the end of its lifetime to prevent memory leaks.
struct cds_array* cds_copy_and_create_array(
    const struct cds_array* const src
){
    if (!src) return (struct cds_array*)0;
    const size_t array_buffer_length 
        = cds_compute_array_buffer_length(
            src->reserved_length, src->bytes_per_element
        );
    struct cds_array* const array = cds_malloc_buffer(array_buffer_length);
    memcpy(array, src, array_buffer_length);
    return array;
}

/// @brief Resize the input array to new data buffer length.
///     The buffer will only be reallocated if new_length is greater than the
///     reserved length of the data buffer.
///     If the input array is null, this function will return null as there is
///     no bytes_per_element to determine the size of the data buffer.
/// @param[in,out] array (Pointer to the pointer to the) Array to be resized.
/// @param[in] new_length New data buffer length.
/// @return (Pointer to) The resized array.
///     The user should free the array with cds_destroy_array()
///     at the end of its lifetime to prevent memory leaks.
struct cds_array* cds_resize_array(
    struct cds_array** const array, const size_t new_length
){
    if (!*array) return (struct cds_array*)0;
    if (new_length > (*array)->reserved_length){
        (*array)->reserved_length = cds_next_power_of_two(new_length);
        struct cds_array* realloced_array;
        const size_t array_buffer_length 
            = cds_compute_array_buffer_length(
                (*array)->reserved_length, (*array)->bytes_per_element
            );
        do realloced_array = realloc(*array, array_buffer_length);
        while (!realloced_array);
        *array = realloced_array;
    }
    (*array)->data_length = new_length;
    return *array;
}

/// @brief Copies the data from src to dest. The length of the reserved buffer 
///     is not copied if dest already has a reserved buffer length 
///     greater than src.
///     If compiled with MSVC, this function will exit the program if either
///     dest or src is null.
///     With other compilers that fully support the C99 standard, this function
///     will return a compiler error if either dest or src is null.
/// @param[out] dest The destination of copied data.
/// @param[in] src The source of the copied data.
/// @return The same pointer as dest.
struct cds_array* cds_copy_array(
#if _MSC_VER
    struct cds_array* dest, const struct cds_array* const src
){
    if (!dest || !src) exit(1);
#else
    struct cds_array dest[static 1], const struct cds_array const src[static 1]
){
#endif
    if (dest == src) return dest;
    cds_resize_array(&dest, src->data_length);
    if (src->data_length)
        memcpy(
            dest->data, src->data, 
            src->data_length * src->bytes_per_element
        );
    dest->data_length = src->data_length;
    dest->bytes_per_element = src->bytes_per_element;
    return dest;
}

/// @brief Free the allocated buffer, and replace the pointer to the array with
///     a null pointer.
/// @param[in,out] array The (pointer to the pointer to the) array to be freed.
/// @return A null pointer.
struct cds_array* cds_destroy_array(struct cds_array** const array){
    if (!*array) return *array;
    free(*array);
    *array = (struct cds_array*)0;
    return *array;
}

/// @brief Get the data element at the specified index.
///     If the input array is null or the index is out of bounds, this function
///     will return a null pointer.
///     This function is only provided for convenience and is suboptimal for
///     repeated access to the data buffer. In most cases, it is better to
///     directly access the data buffer with pointer arithmetic
///     or use this library's iterator functions.
///     This function is the equivalent 
///     of (void*)(&(<element_type>*)array_1->data)[j].
/// @param[in] array The array to get the data element from.
/// @param[in] index The index of the data element to get.
/// @return The data element at the specified index.
void* cds_get_array_element(
    const struct cds_array* const array, const size_t index
){
    if (!array || index >= array->data_length) return (void*)0;
    return (uint8_t*)array->data + index * array->bytes_per_element;
}