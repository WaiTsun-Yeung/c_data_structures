#include <stddef.h>
#include <stdlib.h>

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