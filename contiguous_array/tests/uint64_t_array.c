#include <stdlib.h>

#include "utilities.h"
#include "array_variadic.h"
#include "array.h"

int main(){
    const size_t values_count = 10;
    uint64_t* const values = malloc(values_count * sizeof(uint64_t));
    uint64_t* values_ptr = values;
    *values_ptr++ = 53031ULL;
    *values_ptr++ = 24694ULL;
    *values_ptr++ = 26116ULL;
    *values_ptr++ = 45451ULL;
    *values_ptr++ = 28016ULL;
    *values_ptr++ = 27920ULL;
    *values_ptr++ = 48665ULL;
    *values_ptr++ = 56380ULL;
    *values_ptr++ = 13220ULL;
    *values_ptr++ = 111ULL;
    struct cds_array* array
        = cds_create_uint64_t_array(
            values_count, 
            53031ULL, 24694ULL, 26116ULL, 45451ULL, 28016ULL, 27920ULL, 
            48665ULL, 56380ULL, 13220ULL, 111ULL
        );
    for (
        const uint64_t* array_ptr = cds_data(array), 
            *values_ptr = values;
        array_ptr < (uint64_t*)cds_data(array) + array->data_length;
        ++array_ptr, ++values_ptr
    ) if (*array_ptr != *values_ptr)
        return 1;
    cds_destroy_array(&array);
    free(values);
    return 0;
}