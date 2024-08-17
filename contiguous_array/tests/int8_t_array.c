#include <stdlib.h>

#include "array_variadic.h"
#include "array.h"

int main(){
    const size_t values_count = 10;
    int8_t* const values = malloc(values_count * sizeof(int8_t));
    int8_t* values_ptr = values;
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
        = cds_create_int8_t_array(
            values_count, 
            -100, -34, 71, 125, -121, 107, -76, 81, -84, 73
        );
    for (
        const int8_t* array_ptr = array->data, *values_ptr = values;
        array_ptr < (int8_t*)array->data + array->data_length;
        ++array_ptr, ++values_ptr
    ) if (*array_ptr != *values_ptr)
        return 1;
    cds_destroy_array(&array);
    free(values);
    return 0;
}