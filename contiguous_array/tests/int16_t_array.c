#include <stdlib.h>

#include "alloc.h"
#include "status.h"

#include "array_variadic.h"
#include "array.h"

int main(){
    const size_t values_count = 10;
    int16_t* const values = malloc(values_count * sizeof(int16_t));
    int16_t* values_initialise_ptr = values;
    *values_initialise_ptr++ = 24065;
    *values_initialise_ptr++ = 22311;
    *values_initialise_ptr++ = 24949;
    *values_initialise_ptr++ = 8873;
    *values_initialise_ptr++ = -21217;
    *values_initialise_ptr++ = -26215;
    *values_initialise_ptr++ = 27941;
    *values_initialise_ptr++ = 27079;
    *values_initialise_ptr++ = 22208;
    *values_initialise_ptr++ = -26632;
    enum cds_status return_state;
    struct cds_array* array
        = cds_create_int16_t_array(
            &return_state, values_count, 
            24065, 22311, 24949, 8873, -21217, -26215, 27941, 27079, 22208, 
            -26632
        );
    for (
        const int16_t* array_ptr = cds_data(array), *values_ptr = values;
        array_ptr < (int16_t*)cds_data(array) + array->elements_count;
        ++array_ptr, ++values_ptr
    ) if (*array_ptr != *values_ptr)
        return 256;
    cds_destroy_array(&array);
    free(values);
    return 0;
}