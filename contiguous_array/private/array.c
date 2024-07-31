#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

//TODO: Add constructors for variadic arguments: https://en.cppreference.com/w/cpp/utility/variadic
//TODO: Attemp implement most if not all methods in std::vector.
//TODO: Implement Move constructor, move assignment and swap function. 
//TODO: Create an iterator struct with associated functions. The iterator struct 
// should reference its parent container and cleanup of the parent container 
// should also cleanup the iterator.

static size_t cds_next_power_of_two(const size_t n){
    size_t power = 1;
    while(power < n) power <<= 1;
    return power;
}

void* cds_create_array(
    const size_t length, const size_t bytes_per_element
){
    void* array = (void*)0;
    const size_t reserved_length = cds_next_power_of_two(length);
    while(!array) array = malloc(
        reserved_length * bytes_per_element + sizeof(struct cds_array)
    );
    ((struct cds_array* const)array)->data_length = length;
    ((struct cds_array* const)array)->reserved_length = reserved_length;
    ((struct cds_array* const)array)->bytes_per_element = bytes_per_element;
    ((struct cds_array* const)array)->data 
        = (void*)((size_t)array + sizeof(struct cds_array));
    return array;
}

void* cds_resize_array(void* array, const size_t new_length){
    if (new_length <= ((struct cds_array* const)array)->reserved_length){
        ((struct cds_array* const)array)->data_length = new_length;
        return array;
    }
    const size_t new_reserved_length = cds_next_power_of_two(new_length);
    void* new_array = (void*)0;
    while(!new_array) new_array = realloc(
        array, 
        new_reserved_length 
                * ((struct cds_array* const)array)->bytes_per_element
            + sizeof(struct cds_array)
    );
    ((struct cds_array* const)new_array)->data_length = new_length;
    ((struct cds_array* const)new_array)->reserved_length = new_reserved_length;
    ((struct cds_array* const)new_array)->data 
        = (void*)((size_t)new_array + sizeof(struct cds_array));
    return new_array;
}

void* cds_copy_array(
    void* const new_array, const void* const old_array
){
    if (new_array == old_array) return new_array;
    if (
        ((const struct cds_array* const)old_array)->data_length
            > ((const struct cds_array* const)new_array)->reserved_length
    ) cds_resize_array(
        new_array, 
        ((const struct cds_array* const)old_array)->data_length,
        ((const struct cds_array* const)old_array)->bytes_per_element
    );
    return memcpy(
        new_array, 
        old_array, 
        ((const struct cds_array* const)old_array)->data_length
            * ((const struct cds_array* const)old_array)->bytes_per_element
    );
}

void* cds_destroy_array(void* array){
    free(array);
    return (void*)0;
}