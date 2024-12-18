#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

#include "alloc.h"
#include "linked_list.h"
#include "mutex.h"
#include "doubly_linked_list_type.h"
#include "doubly_linked_list.h"

struct cds_doubly_linked_list_node* cds_create_doubly_linked_list_node(
    const size_t bytes_per_element, const size_t data_align,
    enum cds_status* const return_state
){
    struct cds_doubly_linked_list_node* const node 
        = cds_create_linked_list_node(
            sizeof(struct cds_doubly_linked_list_node), bytes_per_element,
            data_align,
            return_state
        );
    if (!node) return node;
    node->prev = (struct cds_doubly_linked_list_node*)0;
    return node;
}

struct cds_doubly_linked_list* cds_create_doubly_linked_list_with_mutex_type(
    const int mutex_type, enum cds_status* const return_state
){
    struct cds_doubly_linked_list* const list 
        = cds_create_linked_list_with_mutex_type(
            sizeof(struct cds_doubly_linked_list), mutex_type, return_state
        );
    if (!list) return list;
    list->back = (struct cds_doubly_linked_list_node*)0;
    return list;
}

static void cds_destroy_doubly_linked_list_node_core(
    struct cds_doubly_linked_list_node* const node,
    struct cds_doubly_linked_list_node* const next_node
){
    struct cds_doubly_linked_list_node* const this_prev = node->prev;
    struct cds_doubly_linked_list* const this_list = node->list;
    if (this_prev) this_prev->next = next_node;
    else this_list->front = next_node;
    if (next_node) next_node->prev = this_prev;
    else this_list->back = this_prev;
    free(node);
}

/// @brief Destroy the input list node.
/// @param[in,out] node The node to destroy.
/// @return If the input node is part of a list, the input node is returned.
///     Otherwise, a nullptr is returned.
struct cds_doubly_linked_list_node* 
cds_destroy_doubly_linked_list_node_with_timeout(
    struct cds_doubly_linked_list_node *restrict *restrict const node_holder,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    struct cds_doubly_linked_list_node* const node = *node_holder;
    if (!node){
        if (return_state) *return_state = CDS_NULL_ARG;
        return node;
    }
    struct cds_doubly_linked_list* const list = node->list;
    if (list){
        if (
            cds_mutex_lock(
                &list->mutex, mutex_timeout, list->mutex_type, return_state
            )
        ) return node;
        cds_destroy_doubly_linked_list_node_core(node, node->next);
        (void)mtx_unlock(&list->mutex);
    } 
    free(node);
    *node_holder = (struct cds_doubly_linked_list_node*)0;
    if (return_state) *return_state = CDS_SUCCESS;
    return *node_holder;
}

static void cds_push_doubly_linked_list_node_core(
    struct cds_doubly_linked_list_node* const node,
    struct cds_doubly_linked_list_node* const new_node
){
    new_node->list = node->list;
    new_node->prev = node->prev;
    new_node->next = node;
    node->prev = new_node;
}

static void cds_insert_node_to_empty_doubly_linked_list_core(
    struct cds_doubly_linked_list *restrict const list,
    struct cds_doubly_linked_list_node *restrict const node
){
    node->list = list;
    node->prev = node->next = (struct cds_doubly_linked_list_node*)0;
    list->front = list->back = node;
}

struct cds_doubly_linked_list* 
cds_doubly_linked_list_push_front_with_toggle_with_timeout(
    struct cds_doubly_linked_list *restrict const list,
    struct cds_doubly_linked_list_node *restrict const node,
    const bool toggle_safety_guards, 
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (toggle_safety_guards){
        if (!list || !node){
            if (return_state) *return_state = CDS_NULL_ARG;
            return (struct cds_doubly_linked_list*)0;
        }
        if (
            cds_mutex_lock(
                &list->mutex, mutex_timeout, list->mutex_type, return_state
            )
        ) return (struct cds_doubly_linked_list*)0;
    }
    if (return_state) *return_state = CDS_SUCCESS;
    if (!list->front){
        cds_insert_node_to_empty_doubly_linked_list_core(list, node);
        if (toggle_safety_guards) (void)mtx_unlock(&list->mutex);
        return list;
    }
    cds_push_doubly_linked_list_node_core(list->front, node);
    list->front = node;
    if (toggle_safety_guards) (void)mtx_unlock(&list->mutex);
    return list;
}

struct cds_doubly_linked_list* cds_doubly_linked_list_push_back_with_timeout(
    struct cds_doubly_linked_list *restrict const list,
    struct cds_doubly_linked_list_node *restrict const node,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!list || !node){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (struct cds_doubly_linked_list*)0;
    }
    if (
        cds_mutex_lock(
            &list->mutex, mutex_timeout, list->mutex_type, return_state
        )
    ) return (struct cds_doubly_linked_list*)0;
    if (return_state) *return_state = CDS_SUCCESS;
    if (!list->back){
        cds_insert_node_to_empty_doubly_linked_list_core(list, node);
        (void)mtx_unlock(&list->mutex);
        return list;
    }
    (void)cds_push_back_doubly_linked_list_node_core(list->back, node);
    list->back = node;
    (void)mtx_unlock(&list->mutex);
    return list;
}

/// @brief Push a new node before an existing node.
/// @param[in,out] node The node to push the new node before of.
/// @param[in,out] new_node The new node to push before the input node.
/// @return The new node if successful; otherwise, nullptr.
struct cds_doubly_linked_list_node* 
cds_push_doubly_linked_list_node_with_timeout(
    struct cds_doubly_linked_list_node* const node,
    struct cds_doubly_linked_list_node* const new_node,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!node || !new_node){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (struct cds_doubly_linked_list_node*)0;
    }
    struct cds_doubly_linked_list* const list = node->list;
    if (!list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_doubly_linked_list_node*)0;
    }
    if (
        cds_mutex_lock(
            &list->mutex, mutex_timeout, list->mutex_type, return_state
        )
    ) return (struct cds_doubly_linked_list_node*)0;
    cds_push_doubly_linked_list_node_core(node, new_node);
    struct cds_doubly_linked_list_node* const prev_node = node->prev;
    if (prev_node) prev_node->next = new_node; else list->front = new_node;
    (void)mtx_unlock(&list->mutex);
    if (return_state) *return_state = CDS_SUCCESS;
    return new_node;
}

struct cds_doubly_linked_list_node* cds_doubly_linked_list_pop_back_core(
    struct cds_doubly_linked_list* const list
){
    struct cds_doubly_linked_list_node* const node = list->back;
    struct cds_doubly_linked_list_node* const penultimate_node = node->prev;
    if (penultimate_node) penultimate_node->next = list->front 
        = (struct cds_doubly_linked_list_node*)0;
    list->back = penultimate_node;
    return node;
}

struct cds_doubly_linked_list_node* 
cds_doubly_linked_list_pop_back_with_timeout(
    struct cds_doubly_linked_list *restrict const list,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!list){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (struct cds_doubly_linked_list_node*)0;
    }
    if (
        cds_mutex_lock(
            &list->mutex, mutex_timeout, list->mutex_type, return_state
        )
    ) return (struct cds_doubly_linked_list_node*)0;
    if (return_state) *return_state = CDS_SUCCESS;
    if (!list->back){
        (void)mtx_unlock(&list->mutex);
        return list->back;
    }
    struct cds_doubly_linked_list_node* const node 
        = cds_doubly_linked_list_pop_back_core(list);
    (void)mtx_unlock(&list->mutex);
    node->list = (struct cds_doubly_linked_list*)0;
    node->prev = (struct cds_doubly_linked_list_node*)0;
    return node;
}

/// @brief Pop the input node from its list.
/// @param[in,out] node The node to pop from its list.
/// @return The input node.
struct cds_doubly_linked_list_node* 
cds_pop_doubly_linked_list_node_with_timeout(
    struct cds_doubly_linked_list_node *restrict const node,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!node){
        if (return_state) *return_state = CDS_NULL_ARG;
        return node;
    }
    struct cds_doubly_linked_list* const list = node->list;
    if (!list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return node;
    }
    if (
        cds_mutex_lock(
            &list->mutex, mutex_timeout, list->mutex_type, return_state
        )
    ) return node;
    struct cds_doubly_linked_list_node* prev_node = node->prev;
    struct cds_doubly_linked_list_node* next_node = node->next;
    if (prev_node) prev_node->next = next_node; else list->front = next_node;
    if (next_node) next_node->prev = prev_node; else list->back = prev_node;
    (void)mtx_unlock(&list->mutex);
    node->list = (struct cds_doubly_linked_list*)0;
    node->prev = node->next = (struct cds_doubly_linked_list_node*)0;
    if (return_state) *return_state = CDS_SUCCESS;
    return node;
}

enum cds_status cds_doubly_linked_list_destroy_back_with_timeout(
    struct cds_doubly_linked_list *restrict const list,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!list){
        if (return_state) *return_state = CDS_NULL_ARG;
        return CDS_NULL_ARG;
    }
    if (
        cds_mutex_lock(
            &list->mutex, mutex_timeout, list->mutex_type, return_state
        )
    ) return CDS_MUTEX_ERROR;
    if (return_state) *return_state = CDS_SUCCESS;
    if (!list->front){
        (void)mtx_unlock(&list->mutex);
        return CDS_SUCCESS;
    }
    struct cds_doubly_linked_list_node* const node 
        = cds_doubly_linked_list_pop_back_core(list);
    (void)mtx_unlock(&list->mutex);
    free(node);
    return CDS_SUCCESS;
}

/// @brief Invert the input list in place.
/// @param[in,out] list The list to invert.
/// @return The input list.
struct cds_doubly_linked_list* cds_invert_doubly_linked_list_with_timeout(
    struct cds_doubly_linked_list *restrict const list,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!list){
        if (return_state) *return_state = CDS_NULL_ARG;
        return list;
    }
    if (
        cds_mutex_lock(
            &list->mutex, mutex_timeout, list->mutex_type, return_state
        )
    ) return (struct cds_doubly_linked_list*)0;
    if (return_state) *return_state = CDS_SUCCESS;
    if (!list->front){
        (void)mtx_unlock(&list->mutex);
        return list;
    }
    struct cds_doubly_linked_list_node* node = list->front;
    for (size_t i = 0; i < SIZE_MAX, node; ++i){
        struct cds_doubly_linked_list_node* const next_node = node->next;
        node->next = node->prev;
        node->prev = next_node;
        node = next_node;
    }
    struct cds_doubly_linked_list_node* new_back_node = list->front;
    list->front = list->back;
    list->back = new_back_node;
    (void)mtx_unlock(&list->mutex);
    return list;
}

struct cds_doubly_linked_list* 
cds_erase_following_doubly_linked_list_nodes_with_timeout(
    struct cds_doubly_linked_list *restrict const list,
    struct cds_doubly_linked_list_node *restrict const node,
    const bool is_inclusive,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!list){
        if (return_state) *return_state = CDS_NULL_ARG;
        return list;
    }
    if (!node) return cds_clear_doubly_linked_list_with_timeout(
        list, true, mutex_timeout, return_state
    );
    if (!list->front){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_doubly_linked_list*)0;
    }
    struct cds_doubly_linked_list* const node_list = node->list;
    if (!node_list || node_list != list || !node->next){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return list;
    }
    if (
        cds_mutex_lock(
            &node_list->mutex, mutex_timeout, node_list->mutex_type, 
            return_state
        )
    ) return (struct cds_doubly_linked_list*)0;
    cds_erase_following_linked_list_nodes_core(node);
    if (is_inclusive){
        struct cds_doubly_linked_list_node* const prev_node = node->prev;
        prev_node->next = (struct cds_doubly_linked_list_node*)0;
        list->back = prev_node;
        free(node);
        if (!list->back) list->front = list->back;
    } else {
        node->next = (struct cds_doubly_linked_list_node*)0;
        list->back = node;
    }
    (void)mtx_unlock(&node_list->mutex);
    if (return_state) *return_state = CDS_SUCCESS;
    return list;
}

struct cds_doubly_linked_list* 
cds_erase_preceding_doubly_linked_list_nodes_with_timeout(
    struct cds_doubly_linked_list *restrict const list,
    struct cds_doubly_linked_list_node *restrict const node,
    const bool is_inclusive,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!list){
        if (return_state) *return_state = CDS_NULL_ARG;
        return list;
    }
    if (!node) return cds_clear_doubly_linked_list_with_timeout(
        list, true, mutex_timeout, return_state
    );
    if (!list->front){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_doubly_linked_list*)0;
    }
    struct cds_doubly_linked_list* const node_list = node->list;
    if (!node_list || node_list != list || list->front == node){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_doubly_linked_list*)0;
    }
    if (
        cds_mutex_lock(
            &node_list->mutex, mutex_timeout, node_list->mutex_type, 
            return_state
        )
    ) return (struct cds_doubly_linked_list*)0;
    struct cds_doubly_linked_list_node* prev_node = node->prev;
    for (size_t i = 0; i < SIZE_MAX, prev_node; ++i){
        struct cds_doubly_linked_list_node* const prev_prev_node 
            = prev_node->prev;
        free(prev_node);
        prev_node = prev_prev_node;
    }
    if (is_inclusive){
        struct cds_doubly_linked_list_node* const next_node = node->next;
        next_node->prev = (struct cds_doubly_linked_list_node*)0;
        list->front = next_node;
        free(node);
        if (!list->front) list->back = list->front;
    } else {
        node->prev = (struct cds_doubly_linked_list_node*)0;
        list->front = node;
    }
    (void)mtx_unlock(&node_list->mutex);
    if (return_state) *return_state = CDS_SUCCESS;
    return list;
}

struct cds_doubly_linked_list_node* cds_doubly_linked_list_node_prev(
    struct cds_doubly_linked_list_node** const node_holder
){
    struct cds_doubly_linked_list_node* const node = *node_holder;
    *node_holder = node->prev;
    return *node_holder;
}

static void cds_swap_doubly_linked_list_nodes_core(
    struct cds_doubly_linked_list_node** const node_0_holder,
    struct cds_doubly_linked_list_node** const node_1_holder
){
    struct cds_doubly_linked_list_node* const node_0 = *node_0_holder;
    struct cds_doubly_linked_list_node* const node_1 = *node_1_holder;
    struct cds_doubly_linked_list_node* const temp_node = *node_0_holder;
    *node_0_holder = *node_1_holder;
    *node_1_holder = temp_node;
    struct cds_doubly_linked_list* const temp_list = node_0->list;
    node_1->list = node_0->list;
    node_0->list = temp_list;
    struct cds_doubly_linked_list_node* const temp_prev = node_1->prev;
    node_1->prev = node_0->prev;
    node_0->prev = temp_prev;
    struct cds_doubly_linked_list_node* const temp_next = node_1->next;
    node_1->next = node_0->next;
    node_0->next = temp_next;
}

enum cds_status cds_swap_doubly_linked_list_nodes_with_timeout(
    struct cds_doubly_linked_list_node** const node_0_holder,
    struct cds_doubly_linked_list_node** const node_1_holder,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    struct cds_doubly_linked_list_node* const node_0 = *node_0_holder;
    struct cds_doubly_linked_list_node* const node_1 = *node_1_holder;
    enum cds_status local_return_state = CDS_SUCCESS;
    if (!node_0 && !node_1){
        if (return_state) *return_state = CDS_SUCCESS; return CDS_SUCCESS;
    }
    if (!node_0){
        *node_0_holder = cds_pop_doubly_linked_list_node_with_timeout(
            node_1, mutex_timeout, &local_return_state
        );
        *node_1_holder = (struct cds_doubly_linked_list_node*)0;
        if (return_state) *return_state = local_return_state;
        return local_return_state;
    }
    if (!node_1){
        *node_1_holder = cds_pop_doubly_linked_list_node_with_timeout(
            node_0, mutex_timeout, &local_return_state
        );
        *node_0_holder = (struct cds_doubly_linked_list_node*)0;
        if (return_state) *return_state = local_return_state;
        return local_return_state;
    }
    struct cds_doubly_linked_list* const list_0 = node_0->list;
    if (list_0){
        local_return_state = cds_mutex_lock(
            &list_0->mutex, mutex_timeout, list_0->mutex_type, return_state
        );
        if (local_return_state) return local_return_state;
    }
    struct cds_doubly_linked_list* const list_1 = node_1->list;
    const bool needs_lock_node_1 = list_1 && list_1 != list_0;
    if (needs_lock_node_1){
        local_return_state = cds_mutex_lock(
            &list_1->mutex, mutex_timeout, list_1->mutex_type, return_state
        );
        if (local_return_state){
            if (list_0) (void)mtx_unlock(&list_0->mutex);
            return local_return_state;
        }
    }
    cds_swap_doubly_linked_list_nodes_core(node_0_holder, node_1_holder);
    if (needs_lock_node_1) (void)mtx_unlock(&list_1->mutex);
    if (list_0) (void)mtx_unlock(&list_0->mutex);
    if (return_state) *return_state = CDS_SUCCESS;
    return CDS_SUCCESS;
}

enum cds_status cds_swap_doubly_linked_list_with_timeout(
    struct cds_doubly_linked_list* const list_0,
    struct cds_doubly_linked_list* const list_1,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!list_0 && !list_1){
        if (return_state) *return_state = CDS_SUCCESS;
        return CDS_SUCCESS;
    }
    if (!list_0){
        if (return_state) *return_state = CDS_NULL_ARG;
        return CDS_NULL_ARG;
    }
    if (!list_1){
        if (return_state) *return_state = CDS_NULL_ARG;
        return CDS_NULL_ARG;
    }
    enum cds_status local_return_state = cds_mutex_lock(
        &list_0->mutex, mutex_timeout, list_0->mutex_type, return_state
    );
    if (local_return_state) return local_return_state;
    local_return_state = cds_mutex_lock(
        &list_1->mutex, mutex_timeout, list_1->mutex_type, return_state
    );
    if (local_return_state){
        (void)mtx_unlock(&list_0->mutex);
        return local_return_state;
    }
    struct cds_doubly_linked_list_node* const temp_back = list_0->back;
    list_0->back = list_1->back;
    list_1->back = temp_back;
    cds_swap_linked_list_core(list_0, list_1);
    (void)mtx_unlock(&list_0->mutex);
    (void)mtx_unlock(&list_1->mutex);
    if (return_state) *return_state = CDS_SUCCESS;
    return CDS_SUCCESS;
}

struct cds_doubly_linked_list* cds_splice_doubly_linked_list_with_timeout(
    struct cds_doubly_linked_list_node* const dest_node,
    struct cds_doubly_linked_list* const src_list,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!dest_node){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (struct cds_doubly_linked_list*)0;
    }
    struct cds_doubly_linked_list* const dest_list = dest_node->list;
    if (!dest_list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_doubly_linked_list*)0;
    }
    if (!src_list){
        if (return_state) *return_state = CDS_SUCCESS;
        return dest_node->list;
    }
    enum cds_status local_return_state = cds_mutex_lock(
        &dest_list->mutex, mutex_timeout, dest_list->mutex_type, return_state
    );
    if (local_return_state) return (struct cds_doubly_linked_list*)0;
    local_return_state = cds_mutex_lock(
        &src_list->mutex, mutex_timeout, src_list->mutex_type, return_state
    );
    if (local_return_state){
        (void)mtx_unlock(&dest_list->mutex);
        return (struct cds_doubly_linked_list*)0;
    }
    struct cds_doubly_linked_list_node* src_node = src_list->front;
    if (!src_node){
        (void)mtx_unlock(&src_list->mutex);
        (void)mtx_unlock(&dest_list->mutex);
        if (return_state) *return_state = CDS_SUCCESS;
        return dest_list;
    }
    for (size_t i = 0; i < SIZE_MAX, src_node; ++i, src_node = src_node->next)
        src_node->list = dest_list;
    struct cds_doubly_linked_list_node* const src_front = src_list->front;
    struct cds_doubly_linked_list_node* const src_back = src_list->back;
    src_list->front = src_list->back = (struct cds_doubly_linked_list_node*)0;
    struct cds_doubly_linked_list_node* const dest_prev = dest_node->prev;
    if (dest_prev) dest_prev->next = src_front;
    else dest_list->front = src_front;
    src_front->prev = dest_prev;
    dest_node->prev = src_back;
    src_back->next = dest_node;
    (void)mtx_unlock(&src_list->mutex);
    (void)mtx_unlock(&dest_list->mutex);
    if (return_state) *return_state = CDS_SUCCESS;
    return dest_list;
}

static struct cds_doubly_linked_list* 
cds_splice_doubly_linked_list_range_core_without_linking_range_end(
    struct cds_doubly_linked_list* const dest_list,
    struct cds_doubly_linked_list_node* const src_begin_node,
    struct cds_doubly_linked_list_node* const src_end_node,
    struct cds_doubly_linked_list* const src_list,
    struct cds_doubly_linked_list_node* const new_src_begin_prev,
    struct cds_doubly_linked_list_node** const src_end_prev
){
    struct cds_doubly_linked_list_node* src_node = src_begin_node;
    for (
        size_t i = 0; 
        i < SIZE_MAX, src_node != src_end_node; 
        ++i, src_node = src_node->next
    ) src_node->list = dest_list;
    struct cds_doubly_linked_list_node* const src_begin_prev 
        = src_begin_node->prev;
    if (src_begin_prev) src_begin_prev->next = src_end_node;
    else src_list->front = src_end_node;
    if (src_end_node){
        *src_end_prev = src_end_node->prev;
        src_end_node->prev = src_begin_prev;
    } else {
        *src_end_prev = src_list->back;
        src_list->back = src_begin_prev;
    }
    if (new_src_begin_prev) new_src_begin_prev->next = src_begin_node;
    else dest_list->front = src_begin_node;
    src_begin_node->prev = new_src_begin_prev;
    return dest_list;
}

static struct cds_doubly_linked_list* cds_splice_doubly_linked_list_range_core(
    struct cds_doubly_linked_list_node* const dest_node,
    struct cds_doubly_linked_list* const dest_list,
    struct cds_doubly_linked_list_node* const src_begin_node,
    struct cds_doubly_linked_list_node* const src_end_node,
    struct cds_doubly_linked_list* const src_list
){
    struct cds_doubly_linked_list_node* src_end_prev;
    (void)cds_splice_doubly_linked_list_range_core_without_linking_range_end(
        dest_list, src_begin_node, src_end_node, src_list, dest_node->prev, 
        &src_end_prev
    );
    dest_node->prev = src_end_prev;
    src_end_prev->next = dest_node;
    return dest_list;
}

struct cds_doubly_linked_list* cds_splice_doubly_linked_list_range_with_timeout(
    struct cds_doubly_linked_list_node* const dest_node,
    struct cds_doubly_linked_list_node* const src_begin_node,
    struct cds_doubly_linked_list_node* const src_end_node,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!dest_node){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (struct cds_doubly_linked_list*)0;
    }
    struct cds_doubly_linked_list* const dest_list = dest_node->list;
    if (!dest_list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_doubly_linked_list*)0;
    }
    if (!src_begin_node){
        if (src_end_node){
            if (return_state) *return_state = CDS_NULL_ARG;
            return (struct cds_doubly_linked_list*)0;
        }
        if (return_state) *return_state = CDS_SUCCESS;
        return dest_list;
    }
    if (src_end_node && src_begin_node->list != src_end_node->list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_doubly_linked_list*)0;
    }
    struct cds_doubly_linked_list* const src_list = src_begin_node->list;
    if (!src_list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_doubly_linked_list*)0;
    }
    if (src_begin_node == src_end_node){
        if (return_state) *return_state = CDS_SUCCESS;
        return dest_list;
    }
    enum cds_status local_return_state = cds_mutex_lock(
        &dest_list->mutex, mutex_timeout, dest_list->mutex_type, return_state
    );
    if (local_return_state) return (struct cds_doubly_linked_list*)0;
    local_return_state = cds_mutex_lock(
        &src_list->mutex, mutex_timeout, src_list->mutex_type, return_state
    );
    if (local_return_state){
        (void)mtx_unlock(&dest_list->mutex);
        return (struct cds_doubly_linked_list*)0;
    }
    (void)cds_splice_doubly_linked_list_range_core(
        dest_node, dest_list, src_begin_node, src_end_node, src_list
    );
    (void)mtx_unlock(&src_list->mutex);
    (void)mtx_unlock(&dest_list->mutex);
    if (return_state) *return_state = CDS_SUCCESS;
    return dest_list;
}

static struct cds_doubly_linked_list* 
cds_append_doubly_linked_list_with_range_core(
    struct cds_doubly_linked_list* const dest_list,
    struct cds_doubly_linked_list_node* const src_begin_node,
    struct cds_doubly_linked_list_node* const src_end_node,
    struct cds_doubly_linked_list* const src_list
){
    struct cds_doubly_linked_list_node* src_end_prev;
    (void)cds_splice_doubly_linked_list_range_core_without_linking_range_end(
        dest_list, src_begin_node, src_end_node, src_list, dest_list->back, 
        &src_end_prev
    );
    dest_list->back = src_end_prev;
    src_end_prev->next = (struct cds_doubly_linked_list_node*)0;
    return dest_list;
}

struct cds_doubly_linked_list* 
cds_append_doubly_linked_list_with_range_with_timeout(
    struct cds_doubly_linked_list *restrict const dest_list,
    struct cds_doubly_linked_list_node* const src_begin_node,
    struct cds_doubly_linked_list_node* const src_end_node,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!dest_list){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (struct cds_doubly_linked_list*)0;
    }
    if (!src_begin_node){
        if (src_end_node){
            if (return_state) *return_state = CDS_NULL_ARG;
            return (struct cds_doubly_linked_list*)0;
        }
        if (return_state) *return_state = CDS_SUCCESS;
        return dest_list;
    }
    if (src_end_node && src_begin_node->list != src_end_node->list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_doubly_linked_list*)0;
    }
    struct cds_doubly_linked_list* const src_list = src_begin_node->list;
    if (!src_list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_doubly_linked_list*)0;
    }
    if (src_begin_node == src_end_node){
        if (return_state) *return_state = CDS_SUCCESS;
        return dest_list;
    }
    enum cds_status local_return_state = cds_mutex_lock(
        &dest_list->mutex, mutex_timeout, dest_list->mutex_type, return_state
    );
    if (local_return_state) return (struct cds_doubly_linked_list*)0;
    local_return_state = cds_mutex_lock(
        &src_list->mutex, mutex_timeout, src_list->mutex_type, return_state
    );
    if (local_return_state){
        (void)mtx_unlock(&dest_list->mutex);
        return (struct cds_doubly_linked_list*)0;
    }
    (void)cds_append_doubly_linked_list_with_range_core(
        dest_list, src_begin_node, src_end_node, src_list
    );
    (void)mtx_unlock(&src_list->mutex);
    (void)mtx_unlock(&dest_list->mutex);
    if (return_state) *return_state = CDS_SUCCESS;
    return dest_list;
}

struct cds_doubly_linked_list* cds_pop_doubly_linked_list_range_with_timeout(
    struct cds_doubly_linked_list_node* const begin_node,
    struct cds_doubly_linked_list_node* const end_node,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!begin_node){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (struct cds_doubly_linked_list*)0;
    }
    if (end_node && begin_node->list != end_node->list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_doubly_linked_list*)0;
    }
    struct cds_doubly_linked_list* const list = begin_node->list;
    if (!list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_doubly_linked_list*)0;
    }
    struct cds_doubly_linked_list* return_list
        = cds_create_doubly_linked_list_with_mutex_type(
            list->mutex_type, return_state
        );
    if (!return_list) return (struct cds_doubly_linked_list*)0;
    if (begin_node == end_node){
        if (return_state) *return_state = CDS_SUCCESS;
        return return_list;
    }
    enum cds_status local_return_state = cds_mutex_lock(
        &list->mutex, mutex_timeout, list->mutex_type, return_state
    );
    if (local_return_state){
        (void)cds_destroy_doubly_linked_list_with_timeout(
            &return_list, mutex_timeout, &local_return_state
        );
        return (struct cds_doubly_linked_list*)0;
    }
    (void)cds_append_doubly_linked_list_with_range_core(
        return_list, begin_node, end_node, list
    );
    (void)mtx_unlock(&list->mutex);
    if (return_state) *return_state = CDS_SUCCESS;
    return return_list;
}