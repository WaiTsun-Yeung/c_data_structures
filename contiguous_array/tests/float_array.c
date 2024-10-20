#include <stdlib.h>

#include "alloc.h"
#include "array_variadic.h"
#include "array.h"

int main(){
    const size_t values_count = 10;
    float* const values = malloc(values_count * sizeof(float));
    float* values_initialise_ptr = values;
    *values_initialise_ptr++ = 5.9311750e-17f;
    *values_initialise_ptr++ = 4.9210937e36f;
    *values_initialise_ptr++ = 1.2678624e24f;
    *values_initialise_ptr++ = 1.2221256e34f;
    *values_initialise_ptr++ = 1.9729493e10f;
    *values_initialise_ptr++ = 1.1444389e33f;
    *values_initialise_ptr++ = 1.7260791e11f;
    *values_initialise_ptr++ = 7.6375544e-5f;
    *values_initialise_ptr++ = 2.1075263e31f;
    *values_initialise_ptr++ = 3.0695549e28f;
    struct cds_array* array
        = cds_create_float_array(
            10, 
            5.9311750e-17f, 4.9210937e36f, 1.2678624e24f, 1.2221256e34f, 
            1.9729493e10f, 1.1444389e33f, 1.7260791e11f, 7.6375544e-5f, 
            2.1075263e31f, 3.0695549e28f
        );
    for (
        const float* array_ptr = cds_data(array), *values_ptr = values;
        array_ptr < (float*)cds_data(array) + array->data_length;
        ++array_ptr, ++values_ptr
    ) if (*array_ptr != *values_ptr)
        return 1;
    cds_destroy_array(&array);
    free(values);
    return 0;
}