#include <stddef.h>

void* cds_create_linked_list_node(
    const size_t bytes_per_node_type, const size_t bytes_per_element,
    const size_t data_align
);

void* cds_create_linked_list_with_hint(
    const size_t bytes_per_list_type, const omp_sync_hint_t hint
);

void* cds_copy_and_create_linked_list_node(
    const size_t bytes_per_node_type, const void* const src
);

void* cds_copy_and_create_linked_list(
    const size_t bytes_per_list_type, const size_t bytes_per_node_type, 
    void* const src_list, 
    void* (*doubly_linked_list_null_front_callback)(void* const dest_list),
    void (*doubly_linked_list_copy_node_callback)(void* const dest_node),
    void (*doubly_linked_list_closing_callback)
        (void* const dest_list, void* const dest_node)
);

void* cds_change_linked_list_node_data_type(
    const size_t bytes_per_node_type, void** const node, 
    const size_t bytes_per_element, const size_t data_align
);

void* cds_copy_linked_list_node(
    void** const dest, const void* const src
);

void* cds_destroy_linked_list(void** const list);

void* cds_push_next_linked_list_node(
    void* const node,
    void* const new_node,
    void (*doubly_linked_list_callback)(void* const node, void* const new_node)
);

void* cds_copy_and_create_reverse_linked_list(
    const size_t bytes_per_list_type, const size_t bytes_per_node_type, 
    void* const src_list,
    void* (*doubly_linked_list_null_front_callback)(void* const dest_list),
    void* (*push_list_front_callback)(
        void* const dest_list, void* const dest_node, 
        const bool toggle_safety_guards
    )
);

void* cds_linked_list_pop_front(
    void* const list, 
    void (*doubly_linked_list_callback)(void* list, void* const node)
);

void cds_linked_list_destroy_front(
    void* const list, 
    void (*doubly_linked_list_callback)
        (void* const list, const void* const node)
);

void* cds_empty_linked_list(
    void* const list, void (*doubly_linked_list_callback)(void* const list),
    const bool toggle_guards_and_cleanups
);

void cds_erase_following_linked_list_nodes_core(void* const node);

void cds_swap_linked_list_nodes(
    void** const node_0, void** const node_1,
    void (*doubly_linked_list_callback)(void* const node_0, void* const node_1)
);

void* cds_linked_list_node_next(void** const node);