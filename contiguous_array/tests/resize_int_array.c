#include "array.h"
#include "rand.h"

int main(){
    for (int i = 0; i < 1000000; ++i)
        cds_destroy_array(
            cds_resize_array(
                cds_create_array(cds_rand_positive(), sizeof(int)), 
                cds_rand_positive()
            )
        );
    return 0;
}