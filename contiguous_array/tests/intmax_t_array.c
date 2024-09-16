#include <stdlib.h>

#include "utilities.h"
#include "array_variadic.h"
#include "array.h"

int main(){
    const size_t values_count = 10;
    intmax_t* const values = malloc(values_count * sizeof(intmax_t));
    intmax_t* values_ptr = values;
    *values_ptr++ = 24065LL;
    *values_ptr++ = 22311LL;
    *values_ptr++ = 24949LL;
    *values_ptr++ = 8873LL;
    *values_ptr++ = -21217LL;
    *values_ptr++ = -26215LL;
    *values_ptr++ = 27941LL;
    *values_ptr++ = 27079LL;
    *values_ptr++ = 22208LL;
    *values_ptr++ = -26632LL;
    struct cds_array* array
        = cds_create_intmax_t_array(
            values_count, 
            24065LL, 22311LL, 24949LL, 8873LL, -21217LL, -26215LL, 27941LL, 
            27079LL, 22208LL, -26632LL
        );
    for (
        const intmax_t* array_ptr = cds_data(array), 
            *values_ptr = values;
        array_ptr < (intmax_t*)cds_data(array) + array->data_length;
        ++array_ptr, ++values_ptr
    ) if (*array_ptr != *values_ptr)
        return 1;
    cds_destroy_array(&array);
    free(values);
    return 0;
}