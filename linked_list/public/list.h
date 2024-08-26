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
    struct cds_list* const src
);

struct cds_list* cds_destroy_list(
    struct cds_list** const list
);

struct cds_list* cds_list_push_front(
    struct cds_list* const list,
    struct cds_list_node* const node
);

struct cds_list_node* cds_list_pop_front(
    struct cds_list* const list
);

void cds_list_destroy_front(struct cds_list* const list);

void cds_push_next_list_node(
    struct cds_list_node* const prev,
    struct cds_list_node* const node
);

struct cds_list_node* cds_pop_next_list_node(
    struct cds_list_node* const prev
);

void cds_destroy_next_list_node(
    struct cds_list_node* const prev
);

struct cds_list* cds_copy_and_create_reverse_list(
    struct cds_list* const src
);

void cds_erase_following_list_nodes(
    struct cds_list_node* const prev
);

void cds_swap_list_nodes(
    struct cds_list_node** const node_0,
    struct cds_list_node** const node_1
);

#ifndef CDS_LIST_H
#define CDS_LIST_H

    static inline struct cds_list_node* cds_list_begin(
        const struct cds_list* const list
    ){
        return list ? list->front : (struct cds_list_node*)0;
    }

    static inline struct cds_list_node* cds_list_end(
        const struct cds_list* const list
    ){
        return (struct cds_list_node*)0;
    }

#endif // CDS_LIST_H

struct cds_list_node* cds_list_node_next(
    struct cds_list_node** const node
);