#include "array_variadic.h"
#include "array.h"

int main(){
    struct cds_array* array
        = cds_create_unsigned_char_array(
            10, 
            171, 107, 170, 234, 98, 16, 219, 209, 22, 113
        );
    cds_destroy_array(&array);
    return 0;
}