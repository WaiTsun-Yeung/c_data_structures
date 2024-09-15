#include <stddef.h>
#include <stdbool.h>

#include <omp.h>

#include "linked_list.h"
#include "singly_linked_list_type.h"

struct cds_singly_linked_list_node* cds_destroy_free_singly_linked_list_node(
    struct cds_singly_linked_list_node** const node
);

struct cds_singly_linked_list* 
cds_singly_linked_list_push_front_core(
    struct cds_singly_linked_list* const list,
    struct cds_singly_linked_list_node* const node
);

struct cds_singly_linked_list* cds_singly_linked_list_push_front(
    struct cds_singly_linked_list* const list,
    struct cds_singly_linked_list_node* const node
);

struct cds_singly_linked_list_node* cds_pop_next_singly_linked_list_node(
    struct cds_singly_linked_list_node* const prev
);

void cds_destroy_next_singly_linked_list_node(
    struct cds_singly_linked_list_node* const prev
);

struct cds_singly_linked_list* cds_invert_singly_linked_list(
    struct cds_singly_linked_list* const list
);

void cds_erase_following_singly_linked_list_nodes(
    struct cds_singly_linked_list_node* const prev
);

struct cds_singly_linked_list* cds_erase_preceding_singly_linked_list_nodes(
    struct cds_singly_linked_list* const list,
    struct cds_singly_linked_list_node* const node,
    const bool is_inclusive
);

#ifndef CDS_SINGLY_LINKED_LIST_H
#define CDS_SINGLY_LINKED_LIST_H

    static inline struct cds_singly_linked_list_node* 
    cds_create_singly_linked_list_node(
        const size_t bytes_per_element, const size_t data_align
    ){
        return cds_create_linked_list_node(
            sizeof(struct cds_singly_linked_list_node), bytes_per_element, 
            data_align
        );
    }

    static inline struct cds_singly_linked_list* 
    cds_create_singly_linked_list_with_hint(
        const omp_sync_hint_t hint
    ){
        return cds_create_linked_list_with_hint(
            sizeof(struct cds_singly_linked_list), hint
        );
    }

    static inline struct cds_singly_linked_list* 
    cds_create_singly_linked_list(){
        return cds_create_singly_linked_list_with_hint(omp_sync_hint_none);
    }

    /// @brief Copy src node to create 
    ///     a new heap-allocated cds_singly_linked_list_node.
    ///     The address of the src node's next node 
    ///     and the address of the src list's mutex lock are also copied.
    ///     omp_set_lock() and omp_unset_lock() must be called before and after
    ///     calling this function in a multithreaded environment.
    /// @param[in] src The source cds_singly_linked_list_node to copy from.
    /// @return The copied node if successful; otherwise, nullptr.
    static inline struct cds_singly_linked_list_node* 
    cds_copy_and_create_singly_linked_list_node(
        const struct cds_singly_linked_list_node* const src
    ){
        return cds_copy_and_create_linked_list_node(
            sizeof(struct cds_singly_linked_list_node), src
        );
    }

    static inline struct cds_singly_linked_list* 
    cds_copy_and_create_singly_linked_list(
        struct cds_singly_linked_list* const src_list
    ){
        return cds_copy_and_create_linked_list(
            sizeof(struct cds_singly_linked_list), 
            sizeof(struct cds_singly_linked_list_node), 
            src_list, (void*)0, (void*)0, (void*)0
        );
    }

    static inline struct cds_singly_linked_list_node*
    cds_change_singly_linked_list_node_data_type(
        struct cds_singly_linked_list_node** const node,
        const size_t bytes_per_element, const size_t data_align
    ){
        return cds_change_linked_list_node_data_type(
            sizeof(struct cds_singly_linked_list_node), 
            (void**)node, bytes_per_element, data_align
        );
    }

    static inline struct cds_singly_linked_list_node*
    cds_copy_singly_linked_list_node(
        struct cds_singly_linked_list_node** const dest,
        const struct cds_singly_linked_list_node* const src
    ){
        return cds_copy_linked_list_node((void**)dest, src);
    }

    static inline struct cds_singly_linked_list* cds_destroy_singly_linked_list(
        struct cds_singly_linked_list** const list
    ){return cds_destroy_linked_list((void** const)list);}

    /// @brief Insert a node after the input node.
    ///     The ownership of the node is transferred to the list, and the node
    ///     will be destroyed when the list is destroyed.
    /// @param[in,out] prev The node to insert the new node after.
    /// @param[in,out] node The node to insert.
    /// @return The inserted node if successful; otherwise, nullptr.
    static inline struct cds_singly_linked_list_node* 
    cds_push_next_singly_linked_list_node(
        struct cds_singly_linked_list_node* const prev,
        struct cds_singly_linked_list_node* const node
    ){return cds_push_next_linked_list_node(prev, node, (void*)0);}

    /// @brief Pop the front node of the list.
    ///     The ownership of the node is transferred to the caller, and 
    ///     the caller is responsible for destroying the node by calling free().
    /// @param[in,out] list The list to pop the front node from.
    /// @return The front node of the list if successful; otherwise, nullptr.
    static inline struct cds_singly_linked_list_node* 
    cds_singly_linked_list_pop_front(
        struct cds_singly_linked_list* const list
    ){return cds_linked_list_pop_front(list, (void*)0);}

    static inline void cds_singly_linked_list_destroy_front(
        struct cds_singly_linked_list* const list
    ){
        cds_linked_list_destroy_front(list, (void*)0);
    }

    static inline struct cds_singly_linked_list*
    cds_singly_linked_list_push_front_with_toggle(
        struct cds_singly_linked_list* const list,
        struct cds_singly_linked_list_node* const node,
        const bool toggle_safety_guards
    ){
        return toggle_safety_guards 
            ? cds_singly_linked_list_push_front(list, node) 
            : cds_singly_linked_list_push_front_core(list, node);
    }

    static inline struct cds_singly_linked_list* 
    cds_copy_and_create_reverse_singly_linked_list(
        struct cds_singly_linked_list* const src_list
    ){
        return cds_copy_and_create_reverse_linked_list(
            sizeof(struct cds_singly_linked_list), 
            sizeof(struct cds_singly_linked_list_node), 
            src_list, 
            (void*)0,
            cds_singly_linked_list_push_front_with_toggle
        );
    }

    static inline struct cds_singly_linked_list* cds_empty_singly_linked_list(
        struct cds_singly_linked_list* const list
    ){return cds_empty_linked_list(list, (void*)0, true);}

    /// @brief Swap the nodes of the input list. If one of the nodes 
    ///     is a nullptr, the function will return immediately.
    /// @param[in,out] node_0 The first node to swap.
    /// @param[in,out] node_1 The second node to swap.
    static inline void cds_swap_singly_linked_list_nodes(
        struct cds_singly_linked_list_node** const node_0,
        struct cds_singly_linked_list_node** const node_1
    ){
        cds_swap_linked_list_nodes(
            (void** const)node_0, (void** const)node_1, (void*)0
        );
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

    static inline struct cds_singly_linked_list_node* 
    cds_singly_linked_list_node_next(
        struct cds_singly_linked_list_node** const node
    ){return cds_linked_list_node_next((void** const)node);}

#endif // CDS_SIGNLY_LINKED_LIST_H