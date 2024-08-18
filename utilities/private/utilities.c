#include <stddef.h>
#include <stdlib.h>

/// @brief Heap-allocate a buffer of the specified number of bytes.
/// @param[in] buffer_length The number of bytes to be allocated.
/// @return The heap-allocated buffer.
void* cds_malloc_buffer(const size_t buffer_length){
    void* buffer;
    do buffer = malloc(buffer_length);
    while (!buffer);
    return buffer;
}

/// @brief Free the allocated buffer, and replace the pointer to the array with
///     a null pointer.
/// @param[in,out] buffer The (pointer to the pointer to the) buffer 
//      to be freed.
/// @return A null pointer.
void* cds_destroy_buffer(void** const buffer){
    if (!*buffer) return *buffer;
    free(*buffer);
    *buffer = (void*)0;
    return *buffer;
}