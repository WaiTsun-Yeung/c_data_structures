#include <stdlib.h>

#include "alloc.h"
#include "status.h"

#include "array_variadic.h"
#include "array.h"

int main(){
    const size_t values_count = 10;
    double* const values = malloc(values_count * sizeof(double));
    double* values_initialise_ptr = values;
    *values_initialise_ptr++ = 5.9311750e-17;
    *values_initialise_ptr++ = 4.9210937e36;
    *values_initialise_ptr++ = 1.2678624e24;
    *values_initialise_ptr++ = 1.2221256e34;
    *values_initialise_ptr++ = 1.9729493e10;
    *values_initialise_ptr++ = 1.1444389e33;
    *values_initialise_ptr++ = 1.7260791e11;
    *values_initialise_ptr++ = 7.6375544e-5;
    *values_initialise_ptr++ = 2.1075263e31;
    *values_initialise_ptr++ = 3.0695549e28;
    enum cds_status return_state;
    struct cds_array* array
        = cds_create_double_array(
            &return_state, values_count, 
            5.9311750e-17, 4.9210937e36, 1.2678624e24, 1.2221256e34, 
            1.9729493e10, 1.1444389e33, 1.7260791e11, 7.6375544e-5, 
            2.1075263e31, 3.0695549e28
        );
    for (
        const double* array_ptr = cds_data(array), *values_ptr = values;
        array_ptr < (double*)cds_data(array) + array->elements_count;
        ++array_ptr, ++values_ptr
    ) if (*array_ptr != *values_ptr)
        return 256;
    cds_destroy_array(&array);
    free(values);
    return 0;
}