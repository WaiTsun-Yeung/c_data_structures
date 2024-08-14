#include "array_variadic.h"
#include "array.h"

int main(){
    struct cds_array* array
        = cds_create_uint16_t_array(
            10, 
            53031, 24694, 26116, 45451, 28016, 27920, 48665, 56380, 13220, 111
        );
    cds_destroy_array(&array);
    return 0;
}