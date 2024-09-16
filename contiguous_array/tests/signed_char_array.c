#include <stdlib.h>

#include "utilities.h"
#include "array_variadic.h"
#include "array.h"

int main(){
    const size_t values_count = 10;
    signed char* const values = malloc(values_count * sizeof(signed char));
    signed char* values_ptr = values;
    *values_ptr++ = -100;
    *values_ptr++ = -34;
    *values_ptr++ = 71;
    *values_ptr++ = 125;
    *values_ptr++ = -121;
    *values_ptr++ = 107;
    *values_ptr++ = -76;
    *values_ptr++ = 81;
    *values_ptr++ = -84;
    *values_ptr++ = 73;
    struct cds_array* array
        = cds_create_signed_char_array(
            values_count, 
            -100, -34, 71, 125, -121, 107, -76, 81, -84, 73
        );
    for (
        const signed char* array_ptr = cds_data(array), 
            *values_ptr = values;
        array_ptr < (signed char*)cds_data(array) + array->data_length;
        ++array_ptr, ++values_ptr
    ) if (*array_ptr != *values_ptr)
        return 1;
    cds_destroy_array(&array);
    free(values);
    return 0;
}