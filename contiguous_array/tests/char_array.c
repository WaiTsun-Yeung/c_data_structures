#include <stdlib.h>

#include "status.h"

#include "array_variadic.h"
#include "array.h"

int main(){
    const size_t values_count = 10;
    char* const values = malloc(values_count * sizeof(char));
    char* values_initialise_ptr = values;
    *values_initialise_ptr++ = 11;
    *values_initialise_ptr++ = 124;
    *values_initialise_ptr++ = 69;
    *values_initialise_ptr++ = 8;
    *values_initialise_ptr++ = 7;
    *values_initialise_ptr++ = 106;
    *values_initialise_ptr++ = 127;
    *values_initialise_ptr++ = 32;
    *values_initialise_ptr++ = 49;
    *values_initialise_ptr++ = 99;
    enum cds_status return_state;
    struct cds_array* array
        = cds_create_char_array(
            &return_state, values_count, 11, 124, 69, 8, 7, 106, 127, 32, 49, 99
        );
    for (
        const char* array_ptr = cds_array_begin(array), *values_ptr = values;
        array_ptr < (const char*)cds_array_end(array);
        cds_array_next(&array_ptr, sizeof(char)), 
        cds_array_next(&values_ptr, sizeof(char))
    ) if (*array_ptr != *values_ptr)
        return 256;
    cds_destroy_array(&array);
    free(values);
    return 0;
}