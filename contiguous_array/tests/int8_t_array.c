#include "array_variadic.h"
#include "array.h"

int main(){
    struct cds_array* array
        = cds_create_int8_t_array(
            10, 
            -100, -34, 71, 125, -121, 107, -76, 81, -84, 73
        );
    cds_destroy_array(&array);
    return 0;
}