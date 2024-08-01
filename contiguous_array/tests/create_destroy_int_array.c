#include "array.h"
#include "rand.h"

int main(){
    for (int i = 0; i < 1000000; ++i){
        struct cds_array array 
            = cds_create_array(cds_rand_positive(), sizeof(int));
        cds_destroy_array(&array);
    }
    return 0;
}