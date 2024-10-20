#include <stddef.h>
#include <stdbool.h>

#include "mutex.h"
#include "singly_linked_list_type.h"

void* cds_create_linked_list_node(
    const size_t bytes_per_node_type, const size_t bytes_per_element,
    const size_t data_align
);

void* cds_create_linked_list_with_mutex_type(
    const size_t bytes_per_list_type, const int mutex_type
);

void* cds_copy_and_create_linked_list_node(const void* const src);

void* cds_copy_and_create_linked_list_with_timeout(
    const size_t bytes_per_list_type, 
    void* restrict const src_list,
    void* (*doubly_linked_list_null_front_callback)(void* const dest_list),
    void (*doubly_linked_list_copy_node_callback)(void* const dest_node),
    void (*doubly_linked_list_closing_callback)
        (void* const dest_list, void* const dest_node), 
    const struct timespec *restrict const mutex_timeout
);

void* cds_change_linked_list_node_data_type(
    const size_t bytes_per_node_type, void** const node, 
    const size_t bytes_per_element, const size_t data_align
);

void* cds_copy_linked_list_node(
    void **restrict const dest, const void *restrict const src
);

void* cds_destroy_linked_list_with_timeout(
    void *restrict *restrict const list, 
    const struct timespec *restrict const mutex_timeout
);

void* cds_push_next_linked_list_node_core(
    void *restrict const node, void *restrict const new_node,
    void (*doubly_linked_list_callback)(void* const node, void* const new_node)
);

void* cds_push_next_linked_list_node_with_timeout(
    void *restrict const node,
    void *restrict const new_node,
    void (*doubly_linked_list_callback)(void* const node, void* const new_node),
    const struct timespec *restrict const mutex_timeout
);

void* cds_copy_and_create_reverse_linked_list_with_timeout(
    const size_t bytes_per_list_type,
    void *restrict const src_list,
    void* (*doubly_linked_list_null_front_callback)(void* const dest_list),
    void* (*push_list_front_callback)(
        void* const dest_list, void* const dest_node, 
        const bool toggle_safety_guards
    ),
    const struct timespec *restrict const mutex_timeout
);

void* cds_linked_list_pop_front_with_timeout(
    void *restrict const list, 
    void (*doubly_linked_list_callback)(void* list, void* const node),
    const struct timespec *restrict const mutex_timeout
);

enum cds_status cds_linked_list_destroy_front_with_timeout(
    void *restrict const list, 
    void (*doubly_linked_list_callback)
        (void* const list, const void* const node),
    const struct timespec *restrict const mutex_timeout
);

void* cds_empty_linked_list_with_timeout(
    void* restrict const list, 
    void (*doubly_linked_list_callback)(void* const list),
    const bool toggle_guards_and_cleanups, 
    const struct timespec *restrict const mutex_timeout
);

void cds_erase_following_linked_list_nodes_core(void* const node);

enum cds_status cds_swap_linked_list_nodes_with_timeout(
    void** const node_0, void** const node_1,
    void* (*doubly_linked_list_pop_node_callback)(void* const node),
    void (*doubly_linked_list_swap_nodes_callback)
        (void* const node_0, void* const node_1),
    const struct timespec *restrict const mutex_timeout
);

void* cds_linked_list_node_next(void** const node);

#ifndef CDS_LINKED_LIST_H
#define CDS_LINKED_LIST_H

    static inline bool cds_is_linked_list_empty(const void* const list){
        return !((const struct cds_singly_linked_list* const)list)->front;
    }

    static inline void* cds_copy_and_create_linked_list(
        const size_t bytes_per_list_type, 
        void* const src_list,
        void* (*doubly_linked_list_null_front_callback)(void* const dest_list),
        void (*doubly_linked_list_copy_node_callback)(void* const dest_node),
        void (*doubly_linked_list_closing_callback)
            (void* const dest_list, void* const dest_node)
    ){
        return cds_copy_and_create_linked_list_with_timeout(
            bytes_per_list_type, src_list,
            doubly_linked_list_null_front_callback,
            doubly_linked_list_copy_node_callback,
            doubly_linked_list_closing_callback, &cds_default_mutex_timeout
        );
    }

    static inline void* cds_empty_linked_list(
        void* const list, 
        void (*doubly_linked_list_callback)(void* const list),
        const bool toggle_guards_and_cleanups
    ){
        return cds_empty_linked_list_with_timeout(
            list, doubly_linked_list_callback, toggle_guards_and_cleanups, 
            &cds_default_mutex_timeout
        );
    }

    static inline void* cds_destroy_linked_list(void** const list){
        return cds_destroy_linked_list_with_timeout(
            list, &cds_default_mutex_timeout
        );
    }

    static inline void* cds_push_next_linked_list_node(
        void *restrict const node,
        void *restrict const new_node,
        void (*doubly_linked_list_callback)
            (void* const node, void* const new_node)
    ){
        return cds_push_next_linked_list_node_with_timeout(
            node, new_node, doubly_linked_list_callback, 
            &cds_default_mutex_timeout
        );
    }

    static inline void* cds_copy_and_create_reverse_linked_list(
        const size_t bytes_per_list_type,
        void* const src_list,
        void* (*doubly_linked_list_null_front_callback)(void* const dest_list),
        void* (*push_list_front_callback)(
            void* const dest_list, void* const dest_node, 
            const bool toggle_safety_guards
        )
    ){
        return cds_copy_and_create_reverse_linked_list_with_timeout(
            bytes_per_list_type, src_list,
            doubly_linked_list_null_front_callback, push_list_front_callback,
            &cds_default_mutex_timeout
        );
    }

    static inline void* cds_linked_list_pop_front(
        void* const list, 
        void (*doubly_linked_list_callback)(void* list, void* const node)
    ){
        return cds_linked_list_pop_front_with_timeout(
            list, doubly_linked_list_callback, &cds_default_mutex_timeout
        );
    }

    static inline enum cds_status cds_linked_list_destroy_front(
        void* const list, 
        void (*doubly_linked_list_callback)
            (void* const list, const void* const node)
    ){
        return cds_linked_list_destroy_front_with_timeout(
            list, doubly_linked_list_callback, &cds_default_mutex_timeout
        );
    }

    static inline enum cds_status cds_swap_linked_list_nodes(
        void** const node_0, void** const node_1,
        void* (*doubly_linked_list_pop_node_callback)(void* const node),
        void (*doubly_linked_list_swap_nodes_callback)
            (void* const node_0, void* const node_1)
    ){
        return cds_swap_linked_list_nodes_with_timeout(
            node_0, node_1, doubly_linked_list_pop_node_callback,
            doubly_linked_list_swap_nodes_callback, &cds_default_mutex_timeout
        );
    }

#endif // CDS_LINKED_LIST_H