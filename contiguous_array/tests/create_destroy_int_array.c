#include "array.h"
#include "rand.h"

int main(){
    for (int i = 0; i < 1000000; ++i)
        cds_destroy_array(
            (int*)cds_create_array(cds_rand_positive(), sizeof(int))
        );
    return 0;
}