#include <stddef.h>
#include <stdlib.h>

/// @brief Heap-allocate a buffer of the specified number of bytes.
/// @param[in] bytes The number of bytes to be allocated.
/// @return The heap-allocated buffer.
void* cds_malloc_buffer(const size_t bytes){
    void* buffer;
    do buffer = malloc(bytes);
    while (!buffer);
    return buffer;
}

void* cds_realloc_buffer(
    void** const buffer, const size_t new_bytes_count
){
    void* realloced_buffer;
    do realloced_buffer = realloc(*buffer, new_bytes_count);
    while (!realloced_buffer);
    *buffer = realloced_buffer;
    return realloced_buffer;
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