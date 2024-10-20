#include <stdlib.h>

#include "alloc.h"
#include "array_variadic.h"
#include "array.h"

int main(){
    const size_t values_count = 10;
    long double* const values = malloc(values_count * sizeof(long double));
    long double* values_initialise_ptr = values;
    *values_initialise_ptr++ = 5.9311750e-17L;
    *values_initialise_ptr++ = 4.9210937e36L;
    *values_initialise_ptr++ = 1.2678624e24L;
    *values_initialise_ptr++ = 1.2221256e34L;
    *values_initialise_ptr++ = 1.9729493e10L;
    *values_initialise_ptr++ = 1.1444389e33L;
    *values_initialise_ptr++ = 1.7260791e11L;
    *values_initialise_ptr++ = 7.6375544e-5L;
    *values_initialise_ptr++ = 2.1075263e31L;
    *values_initialise_ptr++ = 3.0695549e28L;
    struct cds_array* array
        = cds_create_long_double_array(
            values_count, 
            5.9311750e-17L, 4.9210937e36L, 1.2678624e24L, 1.2221256e34L, 
            1.9729493e10L, 1.1444389e33L, 1.7260791e11L, 7.6375544e-5L, 
            2.1075263e31L, 3.0695549e28L
        );
    for (
        const long double* array_ptr = cds_data(array), 
            *values_ptr = values;
        array_ptr < (long double*)cds_data(array) + array->data_length;
        ++array_ptr, ++values_ptr
    ) if (*array_ptr != *values_ptr)
        return 1;
    cds_destroy_array(&array);
    free(values);
    return 0;
}