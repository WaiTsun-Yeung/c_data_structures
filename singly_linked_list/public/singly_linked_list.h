#include <stddef.h>
#include <stdbool.h>
#include <threads.h>

#include "mutex.h"
#include "linked_list.h"
#include "singly_linked_list_type.h"

struct cds_singly_linked_list_node* cds_destroy_free_singly_linked_list_node(
    struct cds_singly_linked_list_node** const node
);

struct cds_singly_linked_list* 
cds_singly_linked_list_push_front_core(
    struct cds_singly_linked_list *restrict const list,
    struct cds_singly_linked_list_node *restrict const node
);

struct cds_singly_linked_list* cds_singly_linked_list_push_front_with_timeout(
    struct cds_singly_linked_list *restrict const list,
    struct cds_singly_linked_list_node *restrict const node,
    const struct timespec *restrict const mutex_timeout
);

struct cds_singly_linked_list_node* 
cds_pop_next_singly_linked_list_node_with_timeout(
    struct cds_singly_linked_list_node *restrict const prev,
    const struct timespec *restrict const mutex_timeout
);

enum cds_status cds_destroy_next_singly_linked_list_node_with_timeout(
    struct cds_singly_linked_list_node *restrict const prev,
    const struct timespec *restrict const mutex_timeout
);

struct cds_singly_linked_list* cds_invert_singly_linked_list_with_timeout(
    struct cds_singly_linked_list *restrict const list,
    const struct timespec *restrict const mutex_timeout
);

enum cds_status cds_erase_following_singly_linked_list_nodes_with_timeout(
    struct cds_singly_linked_list_node *restrict const prev,
    const struct timespec *restrict const mutex_timeout
);

struct cds_singly_linked_list* 
cds_erase_preceding_singly_linked_list_nodes_with_timeout(
    struct cds_singly_linked_list *restrict const list,
    struct cds_singly_linked_list_node *restrict const node,
    const bool is_inclusive,
    const struct timespec *restrict const mutex_timeout
);

struct cds_singly_linked_list* cds_singly_linked_list_remove_if_with_timeout(
    struct cds_singly_linked_list *restrict const list,
    bool (*predicate)(const struct cds_singly_linked_list_node* const node),
    const struct timespec *restrict const mutex_timeout
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
    cds_create_singly_linked_list_with_mutex_type(const int mutex_type){
        return cds_create_linked_list_with_mutex_type(
            sizeof(struct cds_singly_linked_list), mutex_type
        );
    }

    static inline struct cds_singly_linked_list* 
    cds_create_singly_linked_list(){
        return cds_create_singly_linked_list_with_mutex_type(mtx_timed);
    }

    /// @brief Copy src node to create 
    ///     a new heap-allocated cds_singly_linked_list_node.
    ///     The address of the src node's next node 
    ///     and the address of the src list's mutex lock are also copied.
    ///     cds_mutex_lock() and cds_mutex_unlock() must be called before and 
    ///     after calling this function in a multithreaded environment.
    /// @param[in] src The source cds_singly_linked_list_node to copy from.
    /// @return The copied node if successful; otherwise, nullptr.
    static inline struct cds_singly_linked_list_node* 
    cds_copy_and_create_singly_linked_list_node(
        const struct cds_singly_linked_list_node* const src
    ){return cds_copy_and_create_linked_list_node(src);}

    static inline struct cds_singly_linked_list* 
    cds_copy_and_create_singly_linked_list(
        struct cds_singly_linked_list* const src_list
    ){
        return cds_copy_and_create_linked_list(
            sizeof(struct cds_singly_linked_list), 
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
        struct cds_singly_linked_list_node **restrict const dest,
        const struct cds_singly_linked_list_node* const src
    ){
        return cds_copy_linked_list_node((void**)dest, src);
    }

    static inline struct cds_singly_linked_list* cds_destroy_singly_linked_list(
        struct cds_singly_linked_list** const list
    ){return cds_destroy_linked_list((void** const)list);}

    static inline struct cds_singly_linked_list* 
    cds_singly_linked_list_push_front(
        struct cds_singly_linked_list *restrict const list,
        struct cds_singly_linked_list_node *restrict const node
    ){
        return cds_singly_linked_list_push_front_with_timeout(
            list, node, &cds_default_mutex_timeout
        );
    }

    static inline struct cds_singly_linked_list_node*
    cds_push_next_singly_linked_list_node_with_timeout(
        struct cds_singly_linked_list_node *restrict const prev,
        struct cds_singly_linked_list_node *restrict const node,
        const struct timespec *restrict const mutex_timeout
    ){
        return cds_push_next_linked_list_node_with_timeout(
            (void*)prev, (void*)node, (void*)0, mutex_timeout
        );
    }

    /// @brief Insert a free node after the input node.
    ///     The ownership of the node is transferred to the list, and the node
    ///     will be destroyed when the list is destroyed.
    /// @param[in,out] prev The node to insert the new free node after.
    /// @param[in,out] node The free node to insert.
    /// @return The inserted node if successful; otherwise, nullptr.
    static inline struct cds_singly_linked_list_node* 
    cds_push_next_singly_linked_list_node_with_timeout(
        struct cds_singly_linked_list_node *restrict const prev,
        struct cds_singly_linked_list_node *restrict const node,
        const struct timespec *restrict const mutex_timeout
    ){
        return cds_push_next_linked_list_node_with_timeout(
            prev, node, (void*)0, mutex_timeout
        );
    }

    static inline struct cds_singly_linked_list_node*
    cds_push_next_singly_linked_list_node(
        struct cds_singly_linked_list_node *restrict const prev,
        struct cds_singly_linked_list_node *restrict const node
    ){
        return cds_push_next_singly_linked_list_node_with_timeout(
            prev, node, &cds_default_mutex_timeout
        );
    }

    /// @brief Pop the front node of the list.
    ///     The ownership of the node is transferred to the caller, and 
    ///     the caller is responsible for destroying the node by calling free().
    /// @param[in,out] list The list to pop the front node from.
    /// @return The front node of the list if successful; otherwise, nullptr.
    static inline struct cds_singly_linked_list_node* 
    cds_singly_linked_list_pop_front(
        struct cds_singly_linked_list* const list
    ){return cds_linked_list_pop_front(list, (void*)0);}

    static inline struct cds_singly_linked_list_node*
    cds_pop_next_singly_linked_list_node(
        struct cds_singly_linked_list_node* const prev
    ){
        return cds_pop_next_singly_linked_list_node_with_timeout(
            prev, &cds_default_mutex_timeout
        );
    }

    static inline void cds_singly_linked_list_destroy_front(
        struct cds_singly_linked_list* const list
    ){
        cds_linked_list_destroy_front(list, (void*)0);
    }

    static inline enum cds_status cds_destroy_next_singly_linked_list_node(
        struct cds_singly_linked_list_node* const prev
    ){
        return cds_destroy_next_singly_linked_list_node_with_timeout(
            prev, &cds_default_mutex_timeout
        );
    }

    static inline struct cds_singly_linked_list*
    cds_singly_linked_list_push_front_with_toggle(
        struct cds_singly_linked_list *restrict const list,
        struct cds_singly_linked_list_node *restrict const node,
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
            src_list, 
            (void*)0,
            cds_singly_linked_list_push_front_with_toggle
        );
    }

    static inline struct cds_singly_linked_list*
    cds_invert_singly_linked_list(
        struct cds_singly_linked_list* const list
    ){
        return cds_invert_singly_linked_list_with_timeout(
            list, &cds_default_mutex_timeout
        );
    }

    static inline struct cds_singly_linked_list* cds_empty_singly_linked_list(
        struct cds_singly_linked_list* const list
    ){return cds_empty_linked_list(list, (void*)0, true);}

    static inline enum cds_status cds_erase_following_singly_linked_list_nodes(
        struct cds_singly_linked_list_node* const prev
    ){
        return cds_erase_following_singly_linked_list_nodes_with_timeout(
            prev, &cds_default_mutex_timeout
        );
    }

    static inline struct cds_singly_linked_list*
    cds_erase_preceding_singly_linked_list_nodes(
        struct cds_singly_linked_list *restrict const list,
        struct cds_singly_linked_list_node *restrict const node,
        const bool is_inclusive
    ){
        return cds_erase_preceding_singly_linked_list_nodes_with_timeout(
            list, node, is_inclusive, &cds_default_mutex_timeout
        );
    }

    static inline struct cds_singly_linked_list*
    cds_singly_linked_list_remove_if(
        struct cds_singly_linked_list* const list,
        bool (*predicate)(const struct cds_singly_linked_list_node* const node)
    ){
        return cds_singly_linked_list_remove_if_with_timeout(
            list, predicate, &cds_default_mutex_timeout
        );
    }

    static inline enum cds_status
    cds_swap_singly_linked_list_nodes_pf_with_timeout(
        struct cds_singly_linked_list_node *restrict const prev_node,
        struct cds_singly_linked_list_node *restrict *restrict const free_node,
        const struct timespec *restrict const mutex_timeout
    ){
        return cds_swap_linked_list_nodes_with_timeout(
            (void** const)&prev_node->next, (void** const)free_node, (void*)0,
            (void*)0, mutex_timeout
        );
    }

    static inline enum cds_status 
    cds_swap_free_and_next_singly_linked_list_nodes(
        struct cds_singly_linked_list_node *restrict const prev_node,
        struct cds_singly_linked_list_node *restrict *restrict const free_node
    ){
        return cds_swap_singly_linked_list_nodes_pf_with_timeout(
            prev_node, free_node, &cds_default_mutex_timeout
        );
    }

    static inline enum cds_status
    cds_swap_singly_linked_list_nodes_pp_with_timeout(
        struct cds_singly_linked_list_node* const prev_node_0,
        struct cds_singly_linked_list_node* const prev_node_1,
        const struct timespec *restrict const mutex_timeout
    ){
        return cds_swap_linked_list_nodes_with_timeout(
            (void** const)&prev_node_0->next, (void** const)&prev_node_1->next,
            (void*)0, (void*)0, mutex_timeout
        );
    }

    static inline enum cds_status
    cds_swap_singly_linked_list_nodes_pp(
        struct cds_singly_linked_list_node* const prev_node_0,
        struct cds_singly_linked_list_node* const prev_node_1
    ){
        return cds_swap_singly_linked_list_nodes_pp_with_timeout(
            prev_node_0, prev_node_1, &cds_default_mutex_timeout
        );
    }

    static inline struct cds_singly_linked_list_node* 
    cds_singly_linked_list_begin(
        const struct cds_singly_linked_list* const list
    ){
        return list ? list->front : (struct cds_singly_linked_list_node*)0;
    }

    static inline struct cds_singly_linked_list_node* 
    cds_singly_linked_list_end(){
        return (struct cds_singly_linked_list_node*)0;
    }

    static inline struct cds_singly_linked_list_node* 
    cds_singly_linked_list_node_next(
        struct cds_singly_linked_list_node** const node
    ){return cds_linked_list_node_next((void** const)node);}

    static inline bool cds_is_singly_linked_list_empty(
        const struct cds_singly_linked_list* const list
    ){return cds_is_linked_list_empty(list);}

#endif // CDS_SIGNLY_LINKED_LIST_H