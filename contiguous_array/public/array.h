#include <stddef.h>

#include "array_type.h"

struct cds_array* cds_create_array(
    const size_t length, const size_t bytes_per_element
);

struct cds_array* cds_copy_and_create_array(
    const struct cds_array* const src
);

struct cds_array* cds_resize_array(
    struct cds_array* array, const size_t new_length
);

struct cds_array* cds_copy_array(
    struct cds_array* const dest, const struct cds_array* const src
);

struct cds_array* cds_destroy_array(struct cds_array* const array);