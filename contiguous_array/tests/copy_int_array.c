#include <stdalign.h>

#include "alloc.h"
#include "array.h"
#include "rand.h"

int main(){
    for (int i = 0; i < 1000000; ++i){
        // 0x3FFF is the max size allocatable 
        // by AddressSanitizer's LargeMmapAllocator on Windows systems.
        const int max_array_bytes = 0x3FFF / 2; 
        const int array_length 
            = cds_rand_range(1, max_array_bytes / sizeof(int));
        struct cds_array* array_0 
            = cds_create_array(
                array_length, sizeof(int), alignof(int), (enum cds_status*)0
            );
        struct cds_array* array_1 
            = cds_create_array(
                array_length, sizeof(int), alignof(int), (enum cds_status*)0
            );
        cds_copy_array(&array_1, array_0, true, (enum cds_status*)0);
        for (
            const int *data_0 = cds_data(array_0), 
                *data_1 = cds_data(array_1); 
            data_0 < (int*)cds_data(array_0) + array_length; 
            ++data_0, ++data_1
        ) if (*data_0 != *data_1)
            return 1;
        cds_destroy_array(&array_1);
        cds_destroy_array(&array_0);
    }
    return 0;
}