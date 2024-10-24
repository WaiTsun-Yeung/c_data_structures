#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "status.h"
#include "alloc.h"
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
/// @param[in] element_count The number of elements of the data buffer 
///     to be reserved.
/// @param[in] bytes_per_element The number of bytes per intended 
///     data element type. This is usually the return value of the sizeof()
///     operator.
/// @param[in] data_offset The offset of the data buffer from the start of the
///     allocated buffer.
static inline size_t cds_compute_array_bytes_count(
    const size_t element_count, const size_t bytes_per_element, 
    const ptrdiff_t data_offset
){return data_offset + element_count * bytes_per_element;}

/// @brief This function heap-allocates an empty cds_array struct without
///     instantiating any of its fields, including the data pointer.
///     This function is meant for internal use only.
/// @param[in] reserved_length The number of elements of the data buffer 
///     to be reserved.
/// @param[in] bytes_per_element The number of bytes per element of the intended
///     data type. This is usually the return value of the sizeof() operator.
/// @param[in] data_offset The offset of the data buffer from the start of the
///     allocated buffer.
/// @return The allocated buffer.
static inline struct cds_array* cds_malloc_array(
    const size_t reserved_length, const size_t bytes_per_element, 
    const ptrdiff_t data_offset
){
    return cds_malloc_buffer(
        cds_compute_array_bytes_count(
            reserved_length, 
            bytes_per_element, 
            data_offset
        )
    );
}

/// @brief Create a heap-allocated cds_array.
/// @param[in] length The number of data elements.
/// @param[in] bytes_per_element The number of bytes per element of the intended
///     data type. This is usually the return value of the sizeof() operator.
/// @param[in] data_align The number of bytes to align the data buffer to.
///     This is usually the return value of the alignof() operator.
/// @return The created array.
///     The user should free the array with cds_destroy_array()
///     at the end of its lifetime to prevent memory leaks.
struct cds_array* cds_create_array(
    const size_t length, const size_t bytes_per_element, const size_t data_align
){
    const size_t reserved_length = cds_next_power_of_two(length);
    const ptrdiff_t data_offset 
        = cds_compute_data_offset(sizeof(struct cds_array), data_align);
    struct cds_array* const array 
        = cds_malloc_array(reserved_length, bytes_per_element, data_offset);
    array->data_length = length;
    array->reserved_length = reserved_length;
    array->bytes_per_element = bytes_per_element;
    array->data_offset = data_offset;
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
    const size_t array_bytes_count
        = cds_compute_array_bytes_count(
                src->reserved_length, src->bytes_per_element, src->data_offset
        );
    struct cds_array* const array = cds_malloc_buffer(array_bytes_count);
    const errno_t memcpy_error = memcpy_s(
        array, array_bytes_count, src, 
        cds_compute_array_bytes_count(
            src->data_length, src->bytes_per_element, src->data_offset
        )
    );
    if (memcpy_error){
        cds_print_error_message(
            memcpy_error, __FILE__, __LINE__, __func__, "memcpy_s"
        );
        cds_destroy_buffer((void**)&array);
    }
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
    struct cds_array** const array_holder, const size_t new_length
){
    struct cds_array* array = *array_holder;
    if (!array) return (struct cds_array*)0;
    if (new_length > array->reserved_length){
        array->reserved_length = cds_next_power_of_two(new_length);
        cds_realloc_buffer(
            array_holder, 
            cds_compute_array_bytes_count(
                array->reserved_length, 
                array->bytes_per_element, 
                array->data_offset
            )
        );
    }
    struct cds_array* const new_array = *array_holder;
    new_array->data_length = new_length;
    return new_array;
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
    struct cds_array **restrict const dest_holder, 
    const struct cds_array* const src
){
    struct cds_array* const dest = *dest_holder;
    if (!dest || !src) return (struct cds_array*)0;
#else
    struct cds_array (* const dest)[static 1], 
    const struct cds_array const src[static 1]
){
    struct cds_array* const dest = *dest_holder;
#endif
    if (dest == src) return dest;
    const size_t src_active_bytes_count 
        = cds_compute_array_bytes_count(
            src->data_length, src->bytes_per_element, src->data_offset
        );
    size_t dest_full_bytes_count 
        = cds_compute_array_bytes_count(
            dest->reserved_length, dest->bytes_per_element, 
            src->data_offset
        );
    const bool is_realloc_needed 
        = dest_full_bytes_count < src_active_bytes_count;
    if (is_realloc_needed){
        dest_full_bytes_count
            = cds_compute_array_bytes_count(
                src->reserved_length, src->bytes_per_element, src->data_offset
        );
        cds_realloc_buffer(dest_holder, dest_full_bytes_count);
    }
    struct cds_array* const new_dest = *dest_holder;
    const errno_t memcpy_error 
        = memcpy_s(
            new_dest, dest_full_bytes_count, src, src_active_bytes_count
        );
    if (memcpy_error){
        cds_print_error_message(
            memcpy_error, __FILE__, __LINE__, __func__, "memcpy_s"
        );
        return cds_destroy_buffer(dest_holder);
    }
    if (!is_realloc_needed)
        new_dest->reserved_length = (dest_full_bytes_count - src->data_offset) 
            / new_dest->bytes_per_element;
    return new_dest;
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
    return (char*)array + array->data_offset 
        + index * array->bytes_per_element;
}