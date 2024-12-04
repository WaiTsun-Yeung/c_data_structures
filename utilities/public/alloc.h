#include <stddef.h>

void* cds_destroy_buffer(void** const buffer);

#ifndef CDS_ALLOC_H
#define CDS_ALLOC_H

    static inline size_t cds_compute_offset_with_padding(
        const size_t bytes_per_type, const size_t data_align
    ){
        return ((bytes_per_type + data_align - 1) / data_align) 
            * data_align;
    }

    static inline void* cds_data(const void* const container){
        return (char*)container + *(size_t*)container;
    }

#endif // CDS_ALLOC_H