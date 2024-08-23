#include <stddef.h>

#include "array_type.h"

struct cds_array* cds_create_array(
    const size_t length, const size_t bytes_per_element
);

struct cds_array* cds_copy_and_create_array(
    const struct cds_array* const src
);

struct cds_array* cds_resize_array(
    struct cds_array** const array, const size_t new_length
);

struct cds_array* cds_copy_array(
#if _MSC_VER
    struct cds_array* dest, const struct cds_array* const src
);
#else
    struct cds_array dest[static 1], const struct cds_array const src[static 1]
);
#endif

struct cds_array* cds_destroy_array(struct cds_array** const array);

void* cds_get_array_element(
    const struct cds_array* const array, const size_t index
);