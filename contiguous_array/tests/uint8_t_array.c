#include "array_variadic.h"
#include "array.h"
#include <stdlib.h>
int main(){
    struct cds_array* array
        = cds_create_uint8_t_array(
            10, 
            171, 107, 170, 234, 98, 16, 219, 209, 22, 113
        );
    cds_destroy_array(&array);
    return 0;
}