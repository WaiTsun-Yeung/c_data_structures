#include "array_variadic.h"
#include "array.h"

int main(){
    struct cds_array* array
        = cds_create_int16_t_array(
            10, 
            24065, 22311, 24949, 8873, -21217, -26215, 27941, 27079, 22208, 
            -26632
        );
    cds_destroy_array(&array);
    return 0;
}