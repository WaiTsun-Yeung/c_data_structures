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
    for (int i = 0; i < sizeof(size_t) * 8; ++i){
        if (power > n) return power;
        power <<= 1;
    }
    return power;
}

/// @brief Compute the buffer elements count required to allocate the cds_array 
///     struct.
/// @param[in] element_count The number of elements of the data buffer 
///     to be reserved.
/// @param[in] bytes_per_element The number of bytes per intended 
///     data element type. This is usually the return value of the sizeof()
///     operator.
/// @param[in] data_offset The offset of the data buffer from the start of the
///     allocated buffer.
static inline size_t cds_compute_array_bytes_count(
    const size_t element_count, const size_t bytes_per_element, 
    const size_t data_offset
){return data_offset + element_count * bytes_per_element;}

/// @brief This function heap-allocates an empty cds_array struct without
///     instantiating any of its fields, including the data pointer.
///     This function is meant for internal use only.
/// @param[in] reserved_count The number of elements of the data buffer 
///     to be reserved.
/// @param[in] bytes_per_element The number of bytes per element of the intended
///     data type. This is usually the return value of the sizeof() operator.
/// @param[in] data_offset The offset of the data buffer from the start of the
///     allocated buffer.
/// @return The allocated buffer.
static inline struct cds_array* cds_malloc_array(
    const size_t reserved_count, const size_t bytes_per_element, 
    const size_t data_offset
){
    return malloc(
        cds_compute_array_bytes_count(
            reserved_count, 
            bytes_per_element, 
            data_offset
        )
    );
}

/// @brief Create a heap-allocated cds_array.
/// @param[in] elements_count The number of data elements.
/// @param[in] bytes_per_element The number of bytes per element of the intended
///     data type. This is usually the return value of the sizeof() operator.
/// @param[in] data_align The number of bytes to align the data buffer to.
///     This is usually the return value of the alignof() operator.
/// @return The created array.
///     The user should free the array with cds_destroy_array()
///     at the end of its lifetime to prevent memory leaks.
struct cds_array* cds_create_array(
    const size_t elements_count, const size_t bytes_per_element, 
    const size_t data_align, enum cds_status* const return_state
){
    const size_t reserved_count = cds_next_power_of_two(elements_count);
    const size_t data_offset 
        = cds_compute_data_offset(sizeof(struct cds_array), data_align);
    struct cds_array* const array 
        = cds_malloc_array(reserved_count, bytes_per_element, data_offset);
    if (!array){
        if (return_state) *return_state = CDS_ALLOC_ERROR;
        return array;
    }
    array->elements_count = elements_count;
    array->reserved_count = reserved_count;
    array->bytes_per_element = bytes_per_element;
    array->data_offset = data_offset;
    if (return_state) *return_state = CDS_SUCCESS;
    return array;
}

/// @brief Copy src to create a new heap-allocated cds_array.
/// @param[in] src The source cds_array to copy from.
/// @return The copied array.
///     The user should free the array with cds_destroy_array()
///     at the end of its lifetime to prevent memory leaks.
struct cds_array* cds_copy_and_create_array(
    const struct cds_array *restrict const src, 
    enum cds_status *restrict const return_state
){
    if (!src){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (struct cds_array*)0;
    }
    const size_t array_bytes_count
        = cds_compute_array_bytes_count(
            src->reserved_count, src->bytes_per_element, src->data_offset
        );
    struct cds_array* const array = malloc(array_bytes_count);
    if (!array){
        if (return_state) *return_state = CDS_ALLOC_ERROR;
        return array;
    }
    const errno_t memcpy_error = memcpy_s(
        array, array_bytes_count, src, 
        cds_compute_array_bytes_count(
            src->elements_count, src->bytes_per_element, src->data_offset
        )
    );
    if (memcpy_error){
        cds_print_error_message(
            memcpy_error, __FILE__, __LINE__, __func__, "memcpy_s"
        );
        (void)cds_destroy_buffer((void**)&array);
        if (return_state) *return_state = CDS_COPY_ERROR;
    } else if (return_state) *return_state = CDS_SUCCESS;
    return array;
}

/// @brief Resize the input array to new data buffer length.
///     The buffer will only be reallocated if new_elements_count is greater 
///     than the reserved length of the data buffer.
///     If the input array is null, this function will return null as there is
///     no bytes_per_element to determine the size of the data buffer.
/// @param[in,out] array (Pointer to the pointer to the) Array to be resized.
/// @param[in] new_elements_count New data buffer length.
/// @return (Pointer to) The resized array.
///     The user should free the array with cds_destroy_array()
///     at the end of its lifetime to prevent memory leaks.
struct cds_array* cds_resize_array(
    struct cds_array *restrict *restrict const array_holder, 
    const size_t new_elements_count,
    enum cds_status *restrict const return_state
){
    struct cds_array* const array = *array_holder;
    if (!array){
        if (return_state) *return_state = CDS_NULL_ARG;
        return array;
    }
    if (new_elements_count > array->reserved_count){
        array->reserved_count = cds_next_power_of_two(new_elements_count);
        struct cds_array* const realloced_array = realloc(
            *array_holder, 
            cds_compute_array_bytes_count(
                array->reserved_count, 
                array->bytes_per_element, 
                array->data_offset
            )
        );
        if (!realloced_array){
            if (return_state) *return_state = CDS_ALLOC_ERROR;
            return realloced_array;
        } 
        else *array_holder = realloced_array;
    }
    struct cds_array* const new_array = *array_holder;
    new_array->elements_count = new_elements_count;
    if (return_state) *return_state = CDS_SUCCESS;
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
    const struct cds_array* const src, const bool is_realloc_enabled,
    enum cds_status *restrict const return_state
){
    struct cds_array* const dest = *dest_holder;
    if (!dest || !src) return (struct cds_array*)0;
#else
    struct cds_array (* restrict const dest)[static 1], 
    const struct cds_array const src[static 1], const bool is_realloc_enabled,
    enum cds_status *restrict const return_state
){
    struct cds_array* const dest = *dest_holder;
#endif
    if (dest == src){ 
        if (return_state) *return_state = CDS_INVALID_ARG;
        return dest;
    }
    const size_t src_active_bytes_count 
        = cds_compute_array_bytes_count(
            src->elements_count, src->bytes_per_element, src->data_offset
        );
    size_t dest_full_bytes_count 
        = cds_compute_array_bytes_count(
            dest->reserved_count, dest->bytes_per_element, 
            src->data_offset
        );
    const bool is_realloc_needed 
        = dest_full_bytes_count < src_active_bytes_count;
    if (is_realloc_needed){
        if (!is_realloc_enabled){
            if (return_state) *return_state = CDS_BUFFER_OVERFLOW;
            return cds_destroy_buffer(dest_holder);
        }
        dest_full_bytes_count
            = cds_compute_array_bytes_count(
                src->reserved_count, src->bytes_per_element, src->data_offset
        );
        struct cds_array* realloced_dest 
            = realloc(*dest_holder, dest_full_bytes_count);
        if (!realloced_dest){
            if (return_state) *return_state = CDS_ALLOC_ERROR;
            return realloced_dest;
        } 
        else *dest_holder = realloced_dest;
    } 
    struct cds_array* const new_dest = *dest_holder;
    const errno_t memcpy_error = memcpy_s(
            new_dest, dest_full_bytes_count, src, src_active_bytes_count
        );
    if (memcpy_error){
        cds_print_error_message(
            memcpy_error, __FILE__, __LINE__, __func__, "memcpy_s"
        );
        if (return_state) *return_state = CDS_COPY_ERROR;
        return cds_destroy_buffer(dest_holder);
    }
    if (!is_realloc_needed)
        new_dest->reserved_count = (dest_full_bytes_count - src->data_offset) 
            / new_dest->bytes_per_element;
    if (return_state) *return_state = CDS_SUCCESS;
    return new_dest;
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
    const struct cds_array *restrict const array, const size_t index,
    enum cds_status *restrict const return_state
){
    if (!array){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (void*)0;
    }
    if (index >= array->elements_count){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (void*)0;
    }
    if (return_state) *return_state = CDS_SUCCESS;
    return (char*)array + array->data_offset 
        + index * array->bytes_per_element;
}

const void* cds_array_next(
    const void** const element, const size_t bytes_per_element
){
    *element = (char*)*element + bytes_per_element;
    return *element;
}

const void* cds_array_prev(
    const void** const element, const size_t bytes_per_element
){
    *element = (char*)*element - bytes_per_element;
    return *element;
}


/// @brief Reserve at least new_reserved_count elements for the input array.
///     The new array's exact reserved_count will be indeterminate in order to 
///     fit the behaviours of other functions in this module, as only 
///     elements_count should be determinate.
/// @param[in,out] array The array to reserve elements for.
/// @param[in] new_reserved_count The minimum number of elements to reserve.
/// @param[out] return_state The status of the operation.
/// @return The reserved array.
struct cds_array* cds_reserve_array(
    struct cds_array *restrict *restrict const array, 
    const size_t new_reserved_count,
    enum cds_status *restrict const return_state
){
    if (!*array){
        if (return_state) *return_state = CDS_NULL_ARG;
        return *array;
    }
    const size_t elements_count = (*array)->elements_count;
    (void)cds_resize_array(array, new_reserved_count, return_state);
    if (return_state && *return_state != CDS_SUCCESS) return *array;
    (*array)->elements_count = elements_count;
    return *array;
}
