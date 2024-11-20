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
            = cds_create_array(1, sizeof(int), alignof(int), &return_state);
        if (return_state) return return_state;
        const size_t new_min_reserved_count 
            = cds_rand_range(1, max_array_bytes / sizeof(int));
        array = cds_reserve_array(&array, new_min_reserved_count, &return_state);
        if (return_state) return return_state;
        if (array->reserved_count < new_min_reserved_count) return 256;
        cds_destroy_array(&array);
    }
}