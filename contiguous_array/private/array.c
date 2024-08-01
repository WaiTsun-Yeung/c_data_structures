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

struct cds_array cds_create_array(
    const size_t length, const size_t bytes_per_element
){
    struct cds_array array = {
        .data_length = length,
        .reserved_length = cds_next_power_of_two(length),
        .bytes_per_element = bytes_per_element
    };
    do array.data = malloc(array.reserved_length * bytes_per_element);
    while (!array.data);
    return array;
}

struct cds_array cds_copy_and_create_array(
    const struct cds_array* const old_array
){
    if (!old_array->reserved_length) return (struct cds_array){0};
    struct cds_array new_array = cds_create_array(
        old_array->data_length, old_array->bytes_per_element
    );
    if (old_array->data_length)
        memcpy(
            new_array.data, old_array->data, 
            old_array->data_length * old_array->bytes_per_element
        );
    return new_array;
}

struct cds_array* cds_resize_array(
    struct cds_array* const array, const size_t new_length
){
    if (new_length > array->reserved_length){
        array->reserved_length = cds_next_power_of_two(new_length);
        void* realloced_data = (void*)0;
        while (!realloced_data)
            realloced_data = realloc(
                array->data, array->reserved_length * array->bytes_per_element
            );
        array->data = realloced_data;
    }
    array->data_length = new_length;
    return array;
}

struct cds_array* cds_copy_array(
    struct cds_array* const new_array, const struct cds_array* const old_array
){
    if (new_array == old_array) return new_array;
    cds_resize_array(new_array, old_array->data_length);
    if (old_array->data_length)
        memcpy(
            new_array->data, old_array->data, 
            old_array->data_length * old_array->bytes_per_element
        );
    new_array->data_length = old_array->data_length;
    new_array->bytes_per_element = old_array->bytes_per_element;
    return new_array;
}

struct cds_array* cds_destroy_array(struct cds_array* const array){
    if (!array->reserved_length) return array;
    free(array->data);
    array->data = (void*)0;
    array->data_length = 0;
    array->reserved_length = 0;
    array->bytes_per_element = 0;
    return array;
}