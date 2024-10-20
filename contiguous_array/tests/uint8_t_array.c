#include <stdlib.h>

#include "utilities.h"
#include "array_variadic.h"
#include "array.h"

int main(){
    const size_t values_count = 10;
    uint8_t* const values = malloc(values_count * sizeof(uint8_t));
    uint8_t* values_initialise_ptr = values;
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
        = cds_create_uint8_t_array(
            values_count, 
            171, 107, 170, 234, 98, 16, 219, 209, 22, 113
        );
    for (
        const uint8_t* array_ptr = cds_data(array), *values_ptr = values;
        array_ptr < (uint8_t*)cds_data(array) + array->data_length;
        ++array_ptr, ++values_ptr
    ) if (*array_ptr != *values_ptr)
        return 1;
    cds_destroy_array(&array);
    free(values);
    return 0;
}