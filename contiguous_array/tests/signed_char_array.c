#include <stdlib.h>

#include "alloc.h"
#include "status.h"

#include "array_variadic.h"
#include "array.h"

int main(){
    const size_t values_count = 10;
    signed char* const values = malloc(values_count * sizeof(signed char));
    signed char* values_initialise_ptr = values;
    *values_initialise_ptr++ = -100;
    *values_initialise_ptr++ = -34;
    *values_initialise_ptr++ = 71;
    *values_initialise_ptr++ = 125;
    *values_initialise_ptr++ = -121;
    *values_initialise_ptr++ = 107;
    *values_initialise_ptr++ = -76;
    *values_initialise_ptr++ = 81;
    *values_initialise_ptr++ = -84;
    *values_initialise_ptr++ = 73;
    enum cds_status return_state;
    struct cds_array* array
        = cds_create_signed_char_array(
            &return_state, values_count, 
            -100, -34, 71, 125, -121, 107, -76, 81, -84, 73
        );
    for (
        const signed char* array_ptr = cds_data(array), 
            *values_ptr = values;
        array_ptr < (signed char*)cds_data(array) + array->data_length;
        ++array_ptr, ++values_ptr
    ) if (*array_ptr != *values_ptr)
        return 256;
    cds_destroy_array(&array);
    free(values);
    return 0;
}