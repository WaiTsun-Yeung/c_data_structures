#include <stdlib.h>

#include "utilities.h"
#include "array_variadic.h"
#include "array.h"

int main(){
    const size_t values_count = 10;
    double* const values = malloc(values_count * sizeof(double));
    double* values_ptr = values;
    *values_ptr++ = 5.9311750e-17;
    *values_ptr++ = 4.9210937e36;
    *values_ptr++ = 1.2678624e24;
    *values_ptr++ = 1.2221256e34;
    *values_ptr++ = 1.9729493e10;
    *values_ptr++ = 1.1444389e33;
    *values_ptr++ = 1.7260791e11;
    *values_ptr++ = 7.6375544e-5;
    *values_ptr++ = 2.1075263e31;
    *values_ptr++ = 3.0695549e28;
    struct cds_array* array
        = cds_create_double_array(
            values_count, 
            5.9311750e-17, 4.9210937e36, 1.2678624e24, 1.2221256e34, 
            1.9729493e10, 1.1444389e33, 1.7260791e11, 7.6375544e-5, 
            2.1075263e31, 3.0695549e28
        );
    for (
        const double* array_ptr = cds_data(array), *values_ptr = values;
        array_ptr < (double*)cds_data(array) + array->data_length;
        ++array_ptr, ++values_ptr
    ) if (*array_ptr != *values_ptr)
        return 1;
    cds_destroy_array(&array);
    free(values);
    return 0;
}