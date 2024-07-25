#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

//TODO: Rewrite the entire module to manage memory with this struct:
// struct cds_array{
//     size_t data_length;
//     size_t reserved_length;
//     size_t bytes_per_element;
//     void* data;
// };
//TODO: Add constructors for variadic arguments: https://en.cppreference.com/w/cpp/utility/variadic
//TODO: Attemp implement most if not all methods in std::vector.
//TODO: Implement Move constructor, move assignment and swap function. 

void* cds_create_byte_array(const size_t length){
    void* array = (void*)0;
    while(!array) array = malloc(length);
    return array;
}

void* cds_copy_byte_array(
    void* const new_array, const void* const old_array, const size_t length
){
    if (new_array == old_array) return new_array;
    return memcpy(new_array, old_array, length);
}

void* cds_copy_array(
    void* const new_array, const void* const old_array, const size_t length,
    const size_t bytes_per_element
){
    if (new_array == old_array) return new_array;
    return memcpy(new_array, old_array, length * bytes_per_element);
}

void* cds_resize_byte_array(void* array, const size_t new_length){
    void* new_array = (void*)0;
    while(!new_array) new_array = realloc(array, new_length);
    return new_array;
}

void* cds_resize_byte_array_s(
    void* array, const size_t new_length, const size_t old_length
){
    if (old_length > new_length) return array;
    return cds_resize_byte_array(array, new_length);
}

void* cds_resize_array_s(
    void* array, const size_t new_length, 
    const size_t bytes_per_element, const size_t old_length
){
    if (old_length > new_length) return array;
    return cds_resize_array(array, new_length, bytes_per_element);
}

void* cds_destroy_array(void* array){
    free(array);
    return (void*)0;
}