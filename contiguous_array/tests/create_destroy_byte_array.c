#include "array.h"
#include "rand.h"

int main(){
    for (int i = 0; i < 1000000; ++i)
        cds_destroy_array(cds_create_byte_array(cds_rand_positive()));
    return 0;
}