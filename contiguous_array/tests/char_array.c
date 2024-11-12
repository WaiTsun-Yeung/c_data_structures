#include <stdlib.h>

#include "status.h"

#include "array_variadic.h"
#include "array.h"

int main(){
    const size_t values_count = 10;
    char* const values = malloc(values_count * sizeof(char));
    char* values_ptr = values;
    *values_ptr++ = 11;
    *values_ptr++ = 124;
    *values_ptr++ = 69;
    *values_ptr++ = 8;
    *values_ptr++ = 7;
    *values_ptr++ = 106;
    *values_ptr++ = 127;
    *values_ptr++ = 32;
    *values_ptr++ = 49;
    *values_ptr++ = 99;
    enum cds_status return_state;
    struct cds_array* array
        = cds_create_char_array(
            &return_state, values_count, 11, 124, 69, 8, 7, 106, 127, 32, 49, 99
        );
    for (
        const char* array_ptr = cds_data(array), *values_ptr = values;
        array_ptr < (char*)cds_data(array) + array->elements_count;
        ++array_ptr, ++values_ptr
    ) if (*array_ptr != *values_ptr)
        return 256;
    cds_destroy_array(&array);
    free(values);
    return 0;
}