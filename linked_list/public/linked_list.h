#include <stddef.h>
#include <stdbool.h>

#include "mutex.h"

#include "doubly_linked_list_type.h"

#ifndef CDS_LINKED_LIST_H
#define CDS_LINKED_LIST_H

    enum cds_linked_list_type{
        CDS_SINGLY_LINKED_LIST = 1,
        CDS_DOUBLY_LINKED_LIST = 2
    };

    static inline bool cds_is_linked_list_empty(const void* const list){
        return !((const struct cds_doubly_linked_list* const)list)->front;
    }

#endif // CDS_LINKED_LIST_H

void* cds_create_linked_list_node(
    const size_t bytes_per_node_type, const size_t bytes_per_element,
    const size_t data_align, enum cds_status* const return_state
);

void* cds_create_linked_list_with_mutex_type(
    const size_t bytes_per_list_type, const int mutex_type,
    enum cds_status* const return_state
);

void* cds_copy_and_create_linked_list_node(
    const void *restrict const src, enum cds_status *restrict const return_state
);

void* cds_copy_and_create_linked_list_with_timeout(
    const size_t bytes_per_list_type, 
    void* restrict const src_list,
    const enum cds_linked_list_type linked_list_type,
    const struct timespec *restrict const mutex_timeout
);

void* cds_change_linked_list_node_data_type(
    const size_t bytes_per_node_type, 
    void *restrict * restrict const node_holder, 
    const size_t bytes_per_element, const size_t data_align,
    enum cds_status *restrict const return_state
);

void* cds_copy_linked_list_node(
    void **restrict const dest, const void *const src
);

void* cds_destroy_linked_list_with_timeout(
    void *restrict *restrict const list, 
    const bool uses_mutex_lock,
    const struct timespec *restrict const mutex_timeout
);

void* cds_push_next_linked_list_node_core(
    void *restrict const node, void *restrict const new_node,
    const enum cds_linked_list_type linked_list_type
);

void* cds_push_next_linked_list_node_with_timeout(
    void *restrict const node,
    void *restrict const new_node,
    const enum cds_linked_list_type linked_list_type,
    const struct timespec *restrict const mutex_timeout
);

void* cds_copy_and_create_reverse_linked_list_with_timeout(
    const size_t bytes_per_list_type,
    void *restrict const src_list,
    const enum cds_linked_list_type linked_list_type,
    const struct timespec *restrict const mutex_timeout
);

void* cds_linked_list_pop_front_with_timeout(
    void *restrict const list, 
    const enum cds_linked_list_type linked_list_type,
    const struct timespec *restrict const mutex_timeout
);

enum cds_status cds_linked_list_destroy_front_with_timeout(
    void *restrict const list, 
    const enum cds_linked_list_type linked_list_type,
    const struct timespec *restrict const mutex_timeout
);

void* cds_empty_linked_list_with_timeout(
    void* restrict const list, 
    const enum cds_linked_list_type linked_list_type,
    const bool toggle_guards_and_cleanups, 
    const struct timespec *restrict const mutex_timeout
);

void cds_erase_following_linked_list_nodes_core(void* const node);

void* cds_linked_list_node_next(void** const node);