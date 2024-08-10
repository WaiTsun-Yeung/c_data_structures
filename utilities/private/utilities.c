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