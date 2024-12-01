#include <stdalign.h>

#include "array.h"
#include "rand.h"

int main(){
    for (int i = 0; i < 1000000; ++i){
        // 0x3FFF is the max size allocatable 
        // by AddressSanitizer's LargeMmapAllocator on Windows systems.
        const int max_array_bytes = 0x3FFF / 2; 
        enum cds_status return_state;
        struct cds_array* array 
            = cds_create_array(
                cds_rand_range(1, max_array_bytes / sizeof(int)), 
                sizeof(int), alignof(int), &return_state
            );
        if (return_state) return return_state;
        (void)cds_shrink_to_fit_array(&array, &return_state);
        if (return_state) return return_state;
        if (array->reserved_count != array->elements_count) return 256;
        cds_destroy_array(&array);
    }
}