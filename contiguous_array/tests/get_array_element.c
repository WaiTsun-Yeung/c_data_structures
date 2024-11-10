#include <stdalign.h>

#include "status.h"
#include "alloc.h"
#include "array.h"
#include "rand.h"

int main(){
    for (int i = 0; i < 1000000; ++i){
        // 0x3FFF is the max size allocatable 
        // by AddressSanitizer's LargeMmapAllocator on Windows systems.
        const int max_array_bytes = 0x3FFF / 2; 
        enum cds_status return_state;
        struct cds_array* array_0 
            = cds_create_array(
                cds_rand_range(1, max_array_bytes / sizeof(int)), 
                sizeof(int), alignof(int), &return_state
            );
        if (return_state) return return_state;
        struct cds_array* array_1 
            = cds_copy_and_create_array(array_0, &return_state);
        if (return_state) return return_state;
        for (size_t j = 0; j < array_0->data_length; ++j)
            if (
                *(int*)cds_get_array_element(array_0, j, &return_state) 
                    != ((int*)cds_data(array_1))[j]
            ){
                cds_destroy_array(&array_1);
                cds_destroy_array(&array_0);
                if (return_state) return return_state;
                return 256;
            }
        cds_destroy_array(&array_1);
        cds_destroy_array(&array_0);
    }
    return 0;
}