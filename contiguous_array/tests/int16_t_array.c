#include <stdlib.h>

#include "utilities.h"
#include "array_variadic.h"
#include "array.h"

int main(){
    const size_t values_count = 10;
    int16_t* const values = malloc(values_count * sizeof(int16_t));
    int16_t* values_ptr = values;
    *values_ptr++ = 24065;
    *values_ptr++ = 22311;
    *values_ptr++ = 24949;
    *values_ptr++ = 8873;
    *values_ptr++ = -21217;
    *values_ptr++ = -26215;
    *values_ptr++ = 27941;
    *values_ptr++ = 27079;
    *values_ptr++ = 22208;
    *values_ptr++ = -26632;
    struct cds_array* array
        = cds_create_int16_t_array(
            values_count, 
            24065, 22311, 24949, 8873, -21217, -26215, 27941, 27079, 22208, 
            -26632
        );
    for (
        const int16_t* array_ptr = cds_data(array), *values_ptr = values;
        array_ptr < (int16_t*)cds_data(array) + array->data_length;
        ++array_ptr, ++values_ptr
    ) if (*array_ptr != *values_ptr)
        return 1;
    cds_destroy_array(&array);
    free(values);
    return 0;
}