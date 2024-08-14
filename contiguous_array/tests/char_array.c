#include "array_variadic.h"
#include "array.h"

int main(){
    struct cds_array* array
        = cds_create_char_array(10, 11, 124, 69, 8, 7, 106, 127, 32, 49, 99);
    cds_destroy_array(&array);
    return 0;
}