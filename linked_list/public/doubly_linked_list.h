#include <stddef.h>
#include <stdbool.h>

#include "linked_list.h"
#include "doubly_linked_list_type.h"

struct cds_doubly_linked_list_node* cds_create_doubly_linked_list_node(
    const size_t bytes_per_element, const size_t data_align
);

struct cds_doubly_linked_list* cds_create_doubly_linked_list_with_mutex_type(
    const int mutex_type
);

struct cds_doubly_linked_list_node* 
cds_destroy_doubly_linked_list_node_with_timeout(
    struct cds_doubly_linked_list_node *restrict *restrict const node,
    const struct timespec *restrict const mutex_timeout
);

struct cds_doubly_linked_list* 
cds_doubly_linked_list_push_front_with_toggle_with_timeout(
    struct cds_doubly_linked_list *restrict const list,
    struct cds_doubly_linked_list_node *restrict const node,
    const bool toggle_safety_guards, 
    const struct timespec *restrict const mutex_timeout
);

struct cds_doubly_linked_list* cds_doubly_linked_list_push_back_with_timeout(
    struct cds_doubly_linked_list *restrict const list,
    struct cds_doubly_linked_list_node *restrict const node,
    const struct timespec *restrict const mutex_timeout
);

struct cds_doubly_linked_list_node* 
cds_push_doubly_linked_list_node_with_timeout(
    struct cds_doubly_linked_list_node* const node,
    struct cds_doubly_linked_list_node* const new_node,
    const struct timespec *restrict const mutex_timeout
);

struct cds_doubly_linked_list_node* 
cds_doubly_linked_list_pop_back_with_timeout(
    struct cds_doubly_linked_list *restrict const list,
    const struct timespec *restrict const mutex_timeout
);

struct cds_doubly_linked_list_node* 
cds_pop_doubly_linked_list_node_with_timeout(
    struct cds_doubly_linked_list_node *restrict const node,
    const struct timespec *restrict const mutex_timeout
);

enum cds_status cds_doubly_linked_list_destroy_back_with_timeout(
    struct cds_doubly_linked_list *restrict const list,
    const struct timespec *restrict const mutex_timeout
);

struct cds_doubly_linked_list* cds_invert_doubly_linked_list_with_timeout(
    struct cds_doubly_linked_list *restrict const list,
    const struct timespec *restrict const mutex_timeout
);

struct cds_doubly_linked_list* 
cds_erase_following_doubly_linked_list_nodes_with_timeout(
    struct cds_doubly_linked_list *restrict const list,
    struct cds_doubly_linked_list_node *restrict const node,
    const bool is_inclusive,
    const struct timespec *restrict const mutex_timeout
);

struct cds_doubly_linked_list* 
cds_erase_preceding_doubly_linked_list_nodes_with_timeout(
    struct cds_doubly_linked_list *restrict const list,
    struct cds_doubly_linked_list_node *restrict const node,
    const bool is_inclusive,
    const struct timespec *restrict const mutex_timeout
);

struct cds_doubly_linked_list_node* cds_doubly_linked_list_node_prev(
    struct cds_doubly_linked_list_node** const node
);

enum cds_status cds_swap_doubly_linked_list_nodes_with_timeout(
    struct cds_doubly_linked_list_node** const node_0,
    struct cds_doubly_linked_list_node** const node_1,
    const struct timespec *restrict const mutex_timeout
);

struct cds_doubly_linked_list* cds_doubly_linked_list_remove_if_with_timeout(
    struct cds_doubly_linked_list *restrict const list,
    bool (*predicate)(const struct cds_doubly_linked_list_node* const node),
    const struct timespec *restrict const mutex_timeout
);

#ifndef CDS_DOUBLY_LINKED_LIST_H
#define CDS_DOUBLY_LINKED_LIST_H

    static inline struct cds_doubly_linked_list* 
    cds_create_doubly_linked_list(){
        return cds_create_doubly_linked_list_with_mutex_type(mtx_timed);
    }

    static inline struct cds_doubly_linked_list_node* 
    cds_copy_and_create_doubly_linked_list_node(
        const struct cds_doubly_linked_list_node* const src
    ){
        return cds_copy_and_create_linked_list_node(src);
    }

    static inline struct cds_doubly_linked_list*
    cds_copy_and_create_doubly_linked_list_with_timeout(
        struct cds_doubly_linked_list* const src_list,
        const struct timespec *restrict const mutex_timeout
    ){
        return cds_copy_and_create_linked_list_with_timeout(
            sizeof(struct cds_doubly_linked_list), 
            src_list, 
            CDS_DOUBLY_LINKED_LIST,
            mutex_timeout
        );
    }

    static inline struct cds_doubly_linked_list*
    cds_copy_and_create_doubly_linked_list(
        struct cds_doubly_linked_list* const src_list
    ){
        return cds_copy_and_create_doubly_linked_list_with_timeout(
            src_list, &cds_default_mutex_timeout
        );
    }

    static inline struct cds_doubly_linked_list_node*
    cds_change_doubly_linked_list_node_data_type(
        struct cds_doubly_linked_list_node** const node, 
        const size_t bytes_per_element, const size_t data_align
    ){
        return cds_change_linked_list_node_data_type(
            sizeof(struct cds_doubly_linked_list_node), 
            (void**)node, bytes_per_element, data_align
        );
    }

    static inline struct cds_doubly_linked_list_node*
    cds_copy_doubly_linked_list_node(
        struct cds_doubly_linked_list_node **restrict const dest,
        const struct cds_doubly_linked_list_node* const src
    ){
        return cds_copy_linked_list_node((void**)dest, src);
    }

    static inline struct cds_doubly_linked_list* 
    cds_empty_doubly_linked_list_with_timeout(
        struct cds_doubly_linked_list *restrict const list, 
        const bool toggle_guards_and_cleanups,
        const struct timespec *restrict const mutex_timeout
    ){
        return cds_empty_linked_list_with_timeout(
            list, 
            CDS_DOUBLY_LINKED_LIST,
            toggle_guards_and_cleanups,
            mutex_timeout
        );
    }

    static inline struct cds_doubly_linked_list*
    cds_empty_doubly_linked_list(
        struct cds_doubly_linked_list* const list,
        const bool toggle_guards_and_cleanups
    ){
        return cds_empty_doubly_linked_list_with_timeout(
            list, toggle_guards_and_cleanups, &cds_default_mutex_timeout
        );
    }

    static inline struct cds_doubly_linked_list* 
    cds_destroy_doubly_linked_list_with_timeout(
        struct cds_doubly_linked_list** const list,
        const struct timespec *restrict const mutex_timeout
    ){
        return cds_destroy_linked_list_with_timeout(
            (void**)list, true, mutex_timeout
        );
    }

    static inline struct cds_doubly_linked_list*
    cds_destroy_doubly_linked_list(
        struct cds_doubly_linked_list** const list
    ){
        return cds_destroy_doubly_linked_list_with_timeout(
            list, &cds_default_mutex_timeout
        );
    }

    static inline struct cds_doubly_linked_list*
    cds_doubly_linked_list_push_front_with_toggle(
        struct cds_doubly_linked_list* const list,
        struct cds_doubly_linked_list_node* const node,
        const bool toggle_safety_guards
    ){
        return cds_doubly_linked_list_push_front_with_toggle_with_timeout(
            list, node, toggle_safety_guards, &cds_default_mutex_timeout
        );
    }

    static inline struct cds_doubly_linked_list*
    cds_doubly_linked_list_push_front_with_timeout(
        struct cds_doubly_linked_list *restrict const list,
        struct cds_doubly_linked_list_node *restrict const node,
        const struct timespec *restrict const mutex_timeout
    ){
        return cds_doubly_linked_list_push_front_with_toggle_with_timeout(
            list, node, true, mutex_timeout
        );
    }

    static inline struct cds_doubly_linked_list_node*
    cds_push_doubly_linked_list_node(
        struct cds_doubly_linked_list_node* const node,
        struct cds_doubly_linked_list_node* const new_node
    ){
        return cds_push_doubly_linked_list_node_with_timeout(
            node, new_node, &cds_default_mutex_timeout
        );
    }

    static inline struct cds_doubly_linked_list* 
    cds_doubly_linked_list_push_front(
        struct cds_doubly_linked_list *restrict const list,
        struct cds_doubly_linked_list_node *restrict const node
    ){
        return cds_doubly_linked_list_push_front_with_toggle(
            list, node, true
        );
    }

    static inline struct cds_doubly_linked_list_node*
    cds_push_back_doubly_linked_list_node_core(
        struct cds_doubly_linked_list_node* const node,
        struct cds_doubly_linked_list_node* const new_node
    ){
        return cds_push_next_linked_list_node_core(
            node, new_node, CDS_DOUBLY_LINKED_LIST 
        );
    }

    static inline struct cds_doubly_linked_list*
    cds_doubly_linked_list_push_back(
        struct cds_doubly_linked_list *restrict const list,
        struct cds_doubly_linked_list_node *restrict const node
    ){
        return cds_doubly_linked_list_push_back_with_timeout(
            list, node, &cds_default_mutex_timeout
        );
    }

    /// @brief Push a new node after an existing node.
    /// @param[in,out] node The node to push the new node after of.
    /// @param[in,out] new_node The new node to push after the input node.
    /// @return The new node if successful; otherwise, nullptr.
    static inline struct cds_doubly_linked_list_node* 
    cds_push_next_doubly_linked_list_node_with_timeout(
        struct cds_doubly_linked_list_node* const node,
        struct cds_doubly_linked_list_node* const new_node,
        const struct timespec *restrict const mutex_timeout
    ){
        return cds_push_next_linked_list_node_with_timeout(
            node, new_node, CDS_DOUBLY_LINKED_LIST, mutex_timeout
        );
    }

    static inline struct cds_doubly_linked_list_node*
    cds_push_next_doubly_linked_list_node(
        struct cds_doubly_linked_list_node* const node,
        struct cds_doubly_linked_list_node* const new_node
    ){
        return cds_push_next_doubly_linked_list_node_with_timeout(
            node, new_node, &cds_default_mutex_timeout
        );
    }

    static inline struct cds_doubly_linked_list* 
    cds_copy_and_create_reverse_doubly_linked_list_with_timeout(
        struct cds_doubly_linked_list* const src_list,
        const struct timespec *restrict const mutex_timeout
    ){
        return cds_copy_and_create_reverse_linked_list_with_timeout(
            sizeof(struct cds_doubly_linked_list), 
            src_list, 
            CDS_DOUBLY_LINKED_LIST,
            mutex_timeout
        );
    }

    static inline struct cds_doubly_linked_list*
    cds_copy_and_create_reverse_doubly_linked_list(
        struct cds_doubly_linked_list* const src_list
    ){
        return cds_copy_and_create_reverse_doubly_linked_list_with_timeout(
            src_list, &cds_default_mutex_timeout
        );
    }

    static inline struct cds_doubly_linked_list_node* 
    cds_doubly_linked_list_pop_front_with_timeout(
        struct cds_doubly_linked_list *restrict const list,
        const struct timespec *restrict const mutex_timeout
    ){
        return cds_linked_list_pop_front_with_timeout(
            list, CDS_DOUBLY_LINKED_LIST, mutex_timeout
        );
    }

    static inline struct cds_doubly_linked_list_node*
    cds_doubly_linked_list_pop_front(
        struct cds_doubly_linked_list* const list
    ){
        return cds_doubly_linked_list_pop_front_with_timeout(
            list, &cds_default_mutex_timeout
        );
    }

    static inline void cds_doubly_linked_list_destroy_front_with_timeout(
        struct cds_doubly_linked_list *restrict const list,
        const struct timespec *restrict const mutex_timeout
    ){
        cds_linked_list_destroy_front_with_timeout(
            list, CDS_DOUBLY_LINKED_LIST, mutex_timeout
        );
    }

    static inline void cds_doubly_linked_list_destroy_front(
        struct cds_doubly_linked_list* const list
    ){
        cds_doubly_linked_list_destroy_front_with_timeout(
            list, &cds_default_mutex_timeout
        );
    }

    static inline struct cds_doubly_linked_list_node*
    cds_destroy_doubly_linked_list_node(
        struct cds_doubly_linked_list_node** const node
    ){
        return cds_destroy_doubly_linked_list_node_with_timeout(
            node, &cds_default_mutex_timeout
        );
    }

    static inline struct cds_doubly_linked_list_node* 
    cds_doubly_linked_list_pop_back(
        struct cds_doubly_linked_list* const list
    ){
        return cds_doubly_linked_list_pop_back_with_timeout(
            list, &cds_default_mutex_timeout
        );
    }
    
    static inline enum cds_status cds_doubly_linked_list_destroy_back(
        struct cds_doubly_linked_list* const list
    ){
        return cds_doubly_linked_list_destroy_back_with_timeout(
            list, &cds_default_mutex_timeout
        );
    }
    
    static inline struct cds_doubly_linked_list_node* 
    cds_pop_doubly_linked_list_node(
        struct cds_doubly_linked_list_node* const node
    ){
        return cds_pop_doubly_linked_list_node_with_timeout(
            node, &cds_default_mutex_timeout
        );
    }
    
    static inline struct cds_doubly_linked_list* cds_invert_doubly_linked_list(
        struct cds_doubly_linked_list* const list
    ){
        return cds_invert_doubly_linked_list_with_timeout(
            list, &cds_default_mutex_timeout
        );
    }
    
    static inline struct cds_doubly_linked_list* 
    cds_erase_following_doubly_linked_list_nodes(
        struct cds_doubly_linked_list *restrict const list,
        struct cds_doubly_linked_list_node *restrict const node,
        const bool is_inclusive
    ){
        return cds_erase_following_doubly_linked_list_nodes_with_timeout(
            list, node, is_inclusive, &cds_default_mutex_timeout
        );
    }
    
    static inline struct cds_doubly_linked_list* 
    cds_erase_preceding_doubly_linked_list_nodes(
        struct cds_doubly_linked_list *restrict const list,
        struct cds_doubly_linked_list_node *restrict const node,
        const bool is_inclusive
    ){
        return cds_erase_preceding_doubly_linked_list_nodes_with_timeout(
            list, node, is_inclusive, &cds_default_mutex_timeout
        );
    }

    static inline struct cds_doubly_linked_list*
    cds_doubly_linked_list_remove_if(
        struct cds_doubly_linked_list* const list,
        bool (*predicate)(const struct cds_doubly_linked_list_node* const node)
    ){
        return cds_doubly_linked_list_remove_if_with_timeout(
            list, predicate, &cds_default_mutex_timeout
        );
    }

    static inline enum cds_status cds_swap_doubly_linked_list_nodes(
        struct cds_doubly_linked_list_node** const node_0,
        struct cds_doubly_linked_list_node** const node_1
    ){
        return cds_swap_doubly_linked_list_nodes_with_timeout(
            node_0, node_1, &cds_default_mutex_timeout
        );
    }

    static inline struct cds_doubly_linked_list_node* 
    cds_doubly_linked_list_node_next(
        struct cds_doubly_linked_list_node** const node
    ){return cds_linked_list_node_next((void**)node);}

    static inline struct cds_doubly_linked_list_node* 
    cds_doubly_linked_list_begin(
        struct cds_doubly_linked_list* const list
    ){return list->front;}

    static inline struct cds_doubly_linked_list_node*
    cds_doubly_linked_list_end(){return (struct cds_doubly_linked_list_node*)0;}

    static inline struct cds_doubly_linked_list_node*
    cds_doubly_linked_list_rbegin(
        struct cds_doubly_linked_list* const list
    ){return list->back;}

    static inline struct cds_doubly_linked_list_node*
    cds_doubly_linked_list_rend(){
        return (struct cds_doubly_linked_list_node*)0;
    }

    static inline bool cds_is_doubly_linked_list_empty(
        struct cds_doubly_linked_list* const list
    ){return cds_is_linked_list_empty(list);}

#endif // CDS_DOUBLY_LINKED_LIST_H