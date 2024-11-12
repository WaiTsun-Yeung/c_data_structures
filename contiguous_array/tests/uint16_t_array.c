#include <stdlib.h>

#include "alloc.h"
#include "status.h"

#include "array_variadic.h"
#include "array.h"

int main(){
    const size_t values_count = 10;
    uint16_t* const values = malloc(values_count * sizeof(uint16_t));
    uint16_t* values_initialise_ptr = values;
    *values_initialise_ptr++ = 53031;
    *values_initialise_ptr++ = 24694;
    *values_initialise_ptr++ = 26116;
    *values_initialise_ptr++ = 45451;
    *values_initialise_ptr++ = 28016;
    *values_initialise_ptr++ = 27920;
    *values_initialise_ptr++ = 48665;
    *values_initialise_ptr++ = 56380;
    *values_initialise_ptr++ = 13220;
    *values_initialise_ptr++ = 111;
    enum cds_status return_state;
    struct cds_array* array
        = cds_create_uint16_t_array(
            &return_state, values_count, 
            53031, 24694, 26116, 45451, 28016, 27920, 48665, 56380, 13220, 111
        );
    for (
        const uint16_t* array_ptr = cds_data(array), *values_ptr = values;
        array_ptr < (uint16_t*)cds_data(array) + array->elements_count;
        ++array_ptr, ++values_ptr
    ) if (*array_ptr != *values_ptr)
        return 256;
    cds_destroy_array(&array);
    free(values);
    return 0;
}