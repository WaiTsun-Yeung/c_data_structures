#include <stddef.h>

#include <omp.h>

#include "singly_linked_list_type.h"

struct cds_singly_linked_list_node* cds_create_singly_linked_list_node(
    const size_t bytes_per_element
);

struct cds_singly_linked_list* cds_create_singly_linked_list_with_hint(
    const omp_sync_hint_t hint
);

struct cds_singly_linked_list_node* cds_copy_and_create_singly_linked_list_node(
    const struct cds_singly_linked_list_node* const src
);

struct cds_singly_linked_list* cds_copy_and_create_singly_linked_list(
    struct cds_singly_linked_list* const src
);

struct cds_singly_linked_list* cds_destroy_singly_linked_list(
    struct cds_singly_linked_list** const list
);

struct cds_singly_linked_list_node* cds_destroy_free_singly_linked_list_node(
    struct cds_singly_linked_list_node** const node
);

struct cds_singly_linked_list* cds_singly_linked_list_push_front(
    struct cds_singly_linked_list* const list,
    struct cds_singly_linked_list_node* const node
);

struct cds_singly_linked_list_node* cds_singly_linked_list_pop_front(
    struct cds_singly_linked_list* const list
);

void cds_singly_linked_list_destroy_front(
    struct cds_singly_linked_list* const list
);

void cds_push_next_singly_linked_list_node(
    struct cds_singly_linked_list_node* const prev,
    struct cds_singly_linked_list_node* const node
);

struct cds_singly_linked_list_node* cds_pop_next_singly_linked_list_node(
    struct cds_singly_linked_list_node* const prev
);

void cds_destroy_next_singly_linked_list_node(
    struct cds_singly_linked_list_node* const prev
);

struct cds_singly_linked_list* cds_copy_and_create_reverse_singly_linked_list(
    struct cds_singly_linked_list* const src
);

struct cds_singly_linked_list* cds_invert_singly_linked_list(
    struct cds_singly_linked_list* const list
);

struct cds_singly_linked_list* cds_empty_singly_linked_list(
    struct cds_singly_linked_list* const list
);

void cds_erase_following_singly_linked_list_nodes(
    struct cds_singly_linked_list_node* const prev
);

struct cds_singly_linked_list* cds_erase_preceding_singly_linked_list_nodes(
    struct cds_singly_linked_list* const list,
    struct cds_singly_linked_list_node* const node
);

void cds_swap_singly_linked_list_nodes(
    struct cds_singly_linked_list_node** const node_0,
    struct cds_singly_linked_list_node** const node_1
);

#ifndef CDS_LIST_H
#define CDS_LIST_H

    static inline struct cds_singly_linked_list* 
    cds_create_singly_linked_list(){
        return cds_create_singly_linked_list_with_hint(omp_sync_hint_none);
    }

    static inline struct cds_singly_linked_list_node* 
    cds_singly_linked_list_begin(
        const struct cds_singly_linked_list* const list
    ){
        return list ? list->front : (struct cds_singly_linked_list_node*)0;
    }

    static inline struct cds_singly_linked_list_node* 
    cds_singly_linked_list_end(
        const struct cds_singly_linked_list* const list
    ){
        return (struct cds_singly_linked_list_node*)0;
    }

#endif // CDS_LIST_H

struct cds_singly_linked_list_node* cds_singly_linked_list_node_next(
    struct cds_singly_linked_list_node** const node
);