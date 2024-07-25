#include "array.h"
#include "rand.h"

int main(){
    for (int i = 0; i < 1000000; ++i){
        const int array_length_0 = cds_rand_positive();
        cds_destroy_array(
            cds_resize_byte_array_s(
                cds_create_byte_array(array_length_0), 
                cds_rand_positive(), 
                array_length_0
            )
        );
    }
    return 0;
}