#include <stddef.h>

void* cds_malloc_buffer(const size_t bytes);

void* cds_realloc_buffer(
    void** const buffer, const size_t new_bytes_count
);

void* cds_destroy_buffer(void** const buffer);

#ifndef CDS_UTILITIES_H
#define CDS_UTILITIES_H

    static inline size_t cds_compute_data_offset(
        const size_t bytes_per_type, const size_t data_align
    ){
        return ((bytes_per_type + data_align - 1) / data_align) 
            * data_align;
    }

#endif // CDS_UTILITIES_H