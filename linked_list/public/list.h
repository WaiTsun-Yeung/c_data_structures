#include <stddef.h>

#include <omp.h>

#include "list_type.h"

struct cds_list_node* cds_create_list_node(
    const size_t bytes_per_element
);

struct cds_list* cds_create_list();

struct cds_list* cds_create_list_with_hint(
    const omp_sync_hint_t hint
);

struct cds_list_node* cds_copy_and_create_list_node(
    const struct cds_list_node* const src
);

struct cds_list* cds_copy_and_create_list(
    const struct cds_list* const src
);

void cds_destroy_next_list_node(
    struct cds_list_node* const prev, omp_lock_t* const lock
);

struct cds_list* cds_destroy_list(
    struct cds_list** const list
);