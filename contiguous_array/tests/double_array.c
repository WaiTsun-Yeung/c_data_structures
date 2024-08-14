#include "array_variadic.h"
#include "array.h"

int main(){
    struct cds_array* array
        = cds_create_double_array(
            10, 
            5.9311750e-17L, 4.9210937e36, 1.2678624e24, 1.2221256e34, 
            1.9729493e10, 1.1444389e33, 1.7260791e11, 7.6375544e-5L, 
            2.1075263e31, 3.0695549e28
        );
    cds_destroy_array(&array);
    return 0;
}