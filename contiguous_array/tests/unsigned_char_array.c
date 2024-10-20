#include <stdlib.h>

#include "alloc.h"
#include "array_variadic.h"
#include "array.h"

int main(){
    const size_t values_count = 10;
    unsigned char* const values = malloc(values_count * sizeof(unsigned char));
    unsigned char* values_initialise_ptr = values;
    *values_initialise_ptr++ = 171;
    *values_initialise_ptr++ = 107;
    *values_initialise_ptr++ = 170;
    *values_initialise_ptr++ = 234;
    *values_initialise_ptr++ = 98;
    *values_initialise_ptr++ = 16;
    *values_initialise_ptr++ = 219;
    *values_initialise_ptr++ = 209;
    *values_initialise_ptr++ = 22;
    *values_initialise_ptr++ = 113;
    struct cds_array* array
        = cds_create_unsigned_char_array(
            values_count, 
            171, 107, 170, 234, 98, 16, 219, 209, 22, 113
        );
    for (
        const unsigned char* array_ptr = cds_data(array), 
            *values_ptr = values;
        array_ptr < (unsigned char*)cds_data(array) + array->data_length;
        ++array_ptr, ++values_ptr
    ) if (*array_ptr != *values_ptr) return 1;
    cds_destroy_array(&array);
    free(values);
    return 0;
}