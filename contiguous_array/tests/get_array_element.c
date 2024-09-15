#include "array.h"
#include "rand.h"
#include <stdalign.h>

int main(){
    for (int i = 0; i < 1000000; ++i){
        // 0x3FFF is the max size allocatable 
        // by AddressSanitizer's LargeMmapAllocator on Windows systems.
        const int max_array_bytes = 0x3FFF / 2; 
        struct cds_array* array_0 
            = cds_create_array(
                cds_rand_range(1, max_array_bytes / sizeof(int)), 
                sizeof(int), alignof(int)
            );
        struct cds_array* array_1 = cds_copy_and_create_array(array_0);
        for (size_t j = 0; j < array_0->data_length; ++j)
            if (
                *(int*)cds_get_array_element(array_0, j) 
                    != ((int*)array_1->data)[j]
            ) return 1;
        cds_destroy_array(&array_1);
        cds_destroy_array(&array_0);
    }
    return 0;
}