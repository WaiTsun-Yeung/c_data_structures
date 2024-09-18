#include <stddef.h>
#include <stdbool.h>

#include "array_type.h"

struct cds_array* cds_create_array(
    const size_t length, const size_t bytes_per_element, const size_t data_align
);

struct cds_array* cds_copy_and_create_array(
    const struct cds_array* const src
);

struct cds_array* cds_resize_array(
    struct cds_array** const array, const size_t new_length
);

struct cds_array* cds_copy_array(
#if _MSC_VER
    struct cds_array** const dest, const struct cds_array* const src
);
#else
    struct cds_array (* const dest)[static 1], 
    const struct cds_array const src[static 1]
);
#endif

struct cds_array* cds_destroy_array(struct cds_array** const array);

void* cds_get_array_element(
    const struct cds_array* const array, const size_t index
);

#ifndef CDS_ARRAY_H
#define CDS_ARRAY_H

    static inline bool cds_is_array_empty(const struct cds_array* const array){
        return !array->data_length;
    }

#endif // CDS_ARRAY_H