#include "array.h"
#include "rand.h"

int main(){
    for (int i = 0; i < 1000000; ++i){
        const int array_length = cds_rand_positive();
        int* const array_0 = cds_create_array(array_length, sizeof(int));
        cds_destroy_array(
            (int*)cds_copy_and_create_array(array_0, array_length, sizeof(int))
        );
        cds_destroy_array(array_0);
    }
    return 0;
}