#include "array.h"
#include "rand.h"

int main(){
    for (int i = 0; i < 1000000; ++i){
        const int array_length_0 = cds_rand_positive();
        cds_destroy_array(
            cds_resize_array_s(
                cds_create_array(array_length_0, sizeof(int)), 
                cds_rand_positive(), 
                sizeof(int),
                array_length_0
            )
        );
    }
    return 0;
}