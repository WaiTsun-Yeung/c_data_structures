#include "array.h"
#include "rand.h"

int main(){
    for (int i = 0; i < 1000000; ++i){
        struct cds_array array_0 
            = cds_create_array(cds_rand_positive(), sizeof(int));
        struct cds_array array_1 = cds_copy_and_create_array(&array_0);
        for (int j = 0; j < array_0.data_length; ++j)
            if (((int*)array_0.data)[j] != ((int*)array_1.data)[j])
                return 1;
        cds_destroy_array(&array_1);
        cds_destroy_array(&array_0);
    }
    return 0;
}