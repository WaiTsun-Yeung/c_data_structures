#include <stdlib.h>

#include "alloc.h"
#include "status.h"

#include "array_variadic.h"
#include "array.h"

int main(){
    const size_t values_count = 10;
    uint64_t* const values = malloc(values_count * sizeof(uint64_t));
    uint64_t* values_initialise_ptr = values;
    *values_initialise_ptr++ = 53031ULL;
    *values_initialise_ptr++ = 24694ULL;
    *values_initialise_ptr++ = 26116ULL;
    *values_initialise_ptr++ = 45451ULL;
    *values_initialise_ptr++ = 28016ULL;
    *values_initialise_ptr++ = 27920ULL;
    *values_initialise_ptr++ = 48665ULL;
    *values_initialise_ptr++ = 56380ULL;
    *values_initialise_ptr++ = 13220ULL;
    *values_initialise_ptr++ = 111ULL;
    enum cds_status return_state;
    struct cds_array* array
        = cds_create_uint64_t_array(
            &return_state, values_count, 
            53031ULL, 24694ULL, 26116ULL, 45451ULL, 28016ULL, 27920ULL, 
            48665ULL, 56380ULL, 13220ULL, 111ULL
        );
    for (
        const uint64_t* array_ptr = cds_data(array), 
            *values_ptr = values;
        array_ptr < (uint64_t*)cds_data(array) + array->data_length;
        ++array_ptr, ++values_ptr
    ) if (*array_ptr != *values_ptr)
        return 256;
    cds_destroy_array(&array);
    free(values);
    return 0;
}