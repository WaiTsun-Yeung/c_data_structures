#include <stddef.h>
#include <stdbool.h>

#include "status.h"
#include "alloc.h"
#include "array_type.h"

struct cds_array* cds_create_array(
    const size_t elements_count, const size_t bytes_per_element, 
    const size_t data_align, enum cds_status* const return_state
);

struct cds_array* cds_copy_and_create_array(
    const struct cds_array *restrict const src, 
    enum cds_status *restrict const return_state
);

struct cds_array* cds_resize_array(
    struct cds_array *restrict *restrict const array, 
    const size_t new_elements_count,
    enum cds_status *restrict const return_state
);

struct cds_array* cds_copy_array(
#if _MSC_VER
    struct cds_array **restrict const dest, const struct cds_array* const src,
    const bool is_realloc_enabled, enum cds_status *restrict const return_state
#else
    struct cds_array (* restrict const dest)[static 1], 
    const struct cds_array const src[static 1],
    const bool is_realloc_enabled, enum cds_status *restrict const return_state
#endif
);

void* cds_get_array_element(
    const struct cds_array *restrict const array, const size_t index,
    enum cds_status *restrict const return_state
);
void* cds_array_next(
    const void** const element, const size_t bytes_per_element
);

void* cds_array_prev(
    const void** const element, const size_t bytes_per_element
);


#ifndef CDS_ARRAY_H
#define CDS_ARRAY_H

    static inline bool cds_is_array_empty(const struct cds_array* const array){
        return !array->elements_count;
    }

    static inline struct cds_array* cds_destroy_array(
        struct cds_array** const array
    ){ return cds_destroy_buffer(array); }

    static inline void* cds_array_begin(const struct cds_array* const array){
        return cds_data(array);
    }

    static inline void* cds_array_end(const struct cds_array* const array){
        return (char*)cds_data(array) 
            + array->elements_count * array->bytes_per_element;
    }

    static inline void* cds_array_rbegin(const struct cds_array* const array){
        return (char*)cds_data(array) 
            + (array->elements_count - 1) * array->bytes_per_element;
    }

    static inline void* cds_array_rend(const struct cds_array* const array){
        return (char*)cds_data(array) - array->bytes_per_element;
    }

    static inline struct cds_array* cds_clear_array(
        struct cds_array *restrict *restrict const array, 
        enum cds_status *restrict const return_state
    ){
        return cds_resize_array(array, 0, return_state);
    }

#endif // CDS_ARRAY_H