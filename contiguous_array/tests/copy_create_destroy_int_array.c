#include "array.h"
#include "rand.h"

int main(){
    for (int i = 0; i < 1000000; ++i){
        // 0x3FFF is the max size allocatable 
        // by AddressSanitizer's LargeMmapAllocator on Windows systems.
        const int max_array_bytes = 0x3FFF / 2; 
        struct cds_array* array_0 
            = cds_create_array(
                cds_rand_range(1, max_array_bytes / sizeof(int)), 
                sizeof(int)
            );
        struct cds_array* array_1 = cds_copy_and_create_array(array_0);
        for (
            const int *data_0 = array_0->data, *data_1 = array_1->data; 
            data_0 < (int*)array_0->data + array_0->data_length; 
            ++data_0, ++data_1
        ) if (*data_0 != *data_1)
            return 1;
        cds_destroy_array(&array_1);
        cds_destroy_array(&array_0);
    }
    return 0;
}