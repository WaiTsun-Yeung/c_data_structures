#include "array.h"
#include "rand.h"

int main(){
    for (int i = 0; i < 1000000; ++i){
        const int array_length = cds_rand_positive();
        struct cds_array array_0 = cds_create_array(array_length, sizeof(int));
        struct cds_array array_1 = cds_create_array(array_length, sizeof(int));
        cds_copy_array(&array_1, &array_0);
        cds_destroy_array(&array_1);
        cds_destroy_array(&array_0);
    }
    return 0;
}