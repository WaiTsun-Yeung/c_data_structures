#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <threads.h>

#include "status.h"
#include "alloc.h"
#include "mutex.h"
#include "singly_linked_list_type.h"
#include "singly_linked_list.h"

//TODO: Implement the splice function.

/// @brief Destroy the input list node.
///     If the input node is part of a list, the node will not be destroyed.
/// @param[in,out] node The node to destroy.
/// @return If the input node is part of a list, the function will return the
///     input node. Otherwise, the function will return nullptr.
struct cds_singly_linked_list_node* cds_destroy_free_singly_linked_list_node(
    struct cds_singly_linked_list_node *restrict *restrict const node_holder,
    enum cds_status *restrict const return_state
){
    struct cds_singly_linked_list_node* const node = *node_holder;
    if (!node){
        if (return_state) *return_state = CDS_NULL_ARG;
        return node;
    }
    if (node->list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return node;
    }
    free(node);
    *node_holder = (struct cds_singly_linked_list_node*)0;
    if (return_state) *return_state = CDS_SUCCESS;
    return *node_holder;
}

struct cds_singly_linked_list* 
cds_singly_linked_list_push_front_core(
    struct cds_singly_linked_list *restrict const list,
    struct cds_singly_linked_list_node *restrict const node
){
    node->list = list;
    node->next = list->front;
    list->front = node;
    return list;
}

/// @brief Push a node to the front of the list.
///     The ownership of the node is transferred to the list, and the node
///     will be destroyed when the list is destroyed.
/// @param[in,out] list The list to push the node to.
/// @param[in,out] node The node to push to the list.
/// @return The input list if successful; otherwise, nullptr.
struct cds_singly_linked_list* cds_singly_linked_list_push_front_with_timeout(
    struct cds_singly_linked_list *restrict const list,
    struct cds_singly_linked_list_node *restrict const node,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!list || !node){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    if (
        cds_mutex_lock(
            &list->mutex, mutex_timeout, list->mutex_type, return_state
        )
    ) return (struct cds_singly_linked_list*)0;
    (void)cds_singly_linked_list_push_front_core(list, node);
    (void)mtx_unlock(&list->mutex);
    if (return_state) *return_state = CDS_SUCCESS;
    return list;
}

struct cds_singly_linked_list_node*
cds_pop_next_singly_linked_list_node_core(
    struct cds_singly_linked_list_node* const prev
){
    struct cds_singly_linked_list_node* const node = prev->next;
    node->list = (struct cds_singly_linked_list*)0;
    prev->next = node->next;
    return node;
}

/// @brief Pop the node after the input node.
///     The ownership of the node is transferred to the caller, and the caller
///     is responsible for destroying the node by calling free().
/// @param[in,out] prev The node to pop the next node from.
/// @param[in] lock The mutex lock of the list.
/// @return The next node of the input node if successful; otherwise, nullptr.
struct cds_singly_linked_list_node* 
cds_pop_next_singly_linked_list_node_with_timeout(
    struct cds_singly_linked_list_node *restrict const prev,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!prev){
        if (return_state) *return_state = CDS_NULL_ARG;
        return prev;
    }
    struct cds_singly_linked_list* const list = prev->list;
    if (!list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_singly_linked_list_node*)0;
    }
    if (
        cds_mutex_lock(
            &list->mutex, mutex_timeout, list->mutex_type, return_state
        )
    ) return (struct cds_singly_linked_list_node*)0;
    if (!prev->next){
        (void)mtx_unlock(&list->mutex);
        if (return_state) *return_state = CDS_SUCCESS;
        return prev->next;
    }
    struct cds_singly_linked_list_node* node 
        = cds_pop_next_singly_linked_list_node_core(prev);
    (void)mtx_unlock(&list->mutex);
    if (return_state) *return_state = CDS_SUCCESS;
    return node;
}

/// @brief Destroy the following node of the input node.
///     Iterating through the list while calling this function in a
///     multithreaded environment will result in undefined behaviour.
/// @param[in] prev The node whose next node is to be destroyed.
enum cds_status cds_destroy_next_singly_linked_list_node_with_timeout(
    struct cds_singly_linked_list_node *restrict const prev,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!prev){
        if (return_state) *return_state = CDS_NULL_ARG;
        return CDS_NULL_ARG;
    }
    struct cds_singly_linked_list* const list = prev->list;
    if (!list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return CDS_INVALID_ARG;
    }
    const enum cds_status mutex_lock_error = cds_mutex_lock(
        &list->mutex, mutex_timeout, list->mutex_type, return_state
    );
    if (mutex_lock_error) return mutex_lock_error;
    if (!prev->next){
        (void)mtx_unlock(&list->mutex);
        if (return_state) *return_state = CDS_SUCCESS;
        return CDS_SUCCESS;
    }
    struct cds_singly_linked_list_node* const next_node = prev->next;
    prev->next = next_node->next;
    (void)mtx_unlock(&list->mutex);
    free(next_node);
    if (return_state) *return_state = CDS_SUCCESS;
    return CDS_SUCCESS;
}

struct cds_singly_linked_list* cds_singly_linked_list_push_front_with_toggle(
    struct cds_singly_linked_list *restrict const list,
    struct cds_singly_linked_list_node *restrict const node,
    const bool toggle_safety_guards,
    enum cds_status *restrict const return_state
){
    if (toggle_safety_guards) 
        return cds_singly_linked_list_push_front(list, node, return_state); 
    if (return_state) *return_state = CDS_SUCCESS;
    return cds_singly_linked_list_push_front_core(list, node);
}

/// @brief Invert the input list in place.
/// @param[in,out] list The list to invert.
/// @return The input list.
struct cds_singly_linked_list* cds_invert_singly_linked_list_with_timeout(
    struct cds_singly_linked_list *restrict const list,
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
    ) return (struct cds_singly_linked_list*)0;
    if (!list->front){
        (void)mtx_unlock(&list->mutex);
        if (return_state) *return_state = CDS_SUCCESS;
        return list;
    }
    struct cds_singly_linked_list_node* prev 
        = (struct cds_singly_linked_list_node*)0;
    struct cds_singly_linked_list_node* node = list->front;
    for (size_t i = 0; i < SIZE_MAX, node; ++i){
        struct cds_singly_linked_list_node* const next_node = node->next;
        node->next = prev;
        prev = node;
        node = next_node;
    }
    list->front = prev;
    (void)mtx_unlock(&list->mutex);
    if (return_state) *return_state = CDS_SUCCESS;
    return list;
}

enum cds_status cds_erase_following_singly_linked_list_nodes_with_timeout(
    struct cds_singly_linked_list_node *restrict const prev,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!prev){
        if (return_state) *return_state = CDS_NULL_ARG;
        return CDS_NULL_ARG;
    }
    struct cds_singly_linked_list* const list = prev->list;
    if (!list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return CDS_INVALID_ARG;
    }
    const enum cds_status mutex_lock_error = cds_mutex_lock(
        &list->mutex, mutex_timeout, list->mutex_type, return_state
    );
    if (mutex_lock_error) return mutex_lock_error;
    if (!prev->next){
        (void)mtx_unlock(&list->mutex);
        if (return_state) *return_state = CDS_SUCCESS;
        return CDS_SUCCESS;
    }
    cds_erase_following_linked_list_nodes_core(prev);
    prev->next = (struct cds_singly_linked_list_node*)0;
    (void)mtx_unlock(&list->mutex);
    if (return_state) *return_state = CDS_SUCCESS;
    return CDS_SUCCESS;
}

struct cds_singly_linked_list* 
cds_erase_preceding_singly_linked_list_nodes_with_timeout(
    struct cds_singly_linked_list *restrict const list,
    struct cds_singly_linked_list_node *restrict const node,
    const bool is_inclusive,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!list){
        if (return_state) *return_state = CDS_NULL_ARG;
        return list;
    }
    if (!node) return cds_clear_singly_linked_list(list, return_state);
    if (!node->list || node->list != list || !list->front){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    if (
        cds_mutex_lock(
            &list->mutex, mutex_timeout, list->mutex_type, return_state
        )
    ) return (struct cds_singly_linked_list*)0;
    if (return_state) *return_state = CDS_SUCCESS;
    if (list->front == node){ 
        if (is_inclusive) (void)cds_linked_list_destroy_front_core(
            list, CDS_SINGLY_LINKED_LIST
        );
        (void)mtx_unlock(&list->mutex);
        return list;
    }
    struct cds_singly_linked_list_node* prev = list->front;
    struct cds_singly_linked_list_node* next = prev->next;
    for (
        size_t i = 0; 
        i < SIZE_MAX, next && next != (is_inclusive ? node->next : node);
        ++i
    ){
        free(prev);
        prev = next;
        next = next->next;
    }
    free(prev);
    list->front = next;
    (void)mtx_unlock(&list->mutex);
    return list;
}

static void cds_swap_next_singly_linked_list_nodes_core(
    struct cds_singly_linked_list_node* const prev_node_0,
    struct cds_singly_linked_list_node* const prev_node_1
){
    struct cds_singly_linked_list_node* const node_0 = prev_node_0->next;
    struct cds_singly_linked_list_node* const node_1 = prev_node_1->next;
    prev_node_0->next = node_1;
    prev_node_1->next = node_0;
    struct cds_singly_linked_list* const temp_list = node_0->list;
    node_0->list = node_1->list;
    node_1->list = temp_list;
    struct cds_singly_linked_list_node* const temp_next = node_0->next;
    node_0->next = node_1->next;
    node_1->next = temp_next;
}

enum cds_status cds_swap_next_singly_linked_list_nodes_with_timeout(
    struct cds_singly_linked_list_node* const prev_node_0,
    struct cds_singly_linked_list_node* const prev_node_1,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!prev_node_0 || !prev_node_1){
        if (return_state) *return_state = CDS_NULL_ARG; 
        return CDS_NULL_ARG;
    }
    struct cds_singly_linked_list* const list_0 = prev_node_0->list;
    struct cds_singly_linked_list* const list_1 = prev_node_1->list;
    if (!list_0 || !list_1){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return CDS_INVALID_ARG;
    }
    enum cds_status local_return_state = CDS_SUCCESS;
    if (!prev_node_0->next){
        prev_node_0->next = cds_pop_next_singly_linked_list_node_with_timeout(
            prev_node_1, mutex_timeout, &local_return_state
        );
        if (return_state) *return_state = local_return_state;
        return local_return_state;
    }
    if (!prev_node_1->next){
        prev_node_1->next = cds_pop_next_singly_linked_list_node_with_timeout(
            prev_node_0, mutex_timeout, &local_return_state
        );
        if (return_state) *return_state = local_return_state;
        return local_return_state;
    }
    const enum cds_status mutex_lock_error_0 = cds_mutex_lock(
        &list_0->mutex, mutex_timeout, list_0->mutex_type, return_state
    );
    if (mutex_lock_error_0) return mutex_lock_error_0;
    const bool needs_lock_node_1 = list_1 != list_0;
    if (needs_lock_node_1){
        const enum cds_status mutex_lock_error_1 = cds_mutex_lock(
            &list_1->mutex, mutex_timeout, list_1->mutex_type, return_state
        );
        if (mutex_lock_error_1){
            (void)mtx_unlock(&list_0->mutex);
            return mutex_lock_error_1;
        }
    }
    cds_swap_next_singly_linked_list_nodes_core(prev_node_0, prev_node_1);
    if (needs_lock_node_1) (void)mtx_unlock(&list_1->mutex);
    (void)mtx_unlock(&list_0->mutex);
    if (return_state) *return_state = CDS_SUCCESS; 
    return CDS_SUCCESS;
}

enum cds_status cds_swap_free_and_next_singly_linked_list_nodes_with_timeout(
    struct cds_singly_linked_list_node *restrict const prev_node,
    struct cds_singly_linked_list_node *restrict *restrict const 
        free_node_holder,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    struct cds_singly_linked_list_node* const free_node = *free_node_holder;
    if (!prev_node || !free_node){
        if (return_state) *return_state = CDS_NULL_ARG;
        return CDS_NULL_ARG;
    }
    struct cds_singly_linked_list* const list = prev_node->list;
    if (!list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return CDS_INVALID_ARG;
    }
    if ((free_node && free_node->list)){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return CDS_INVALID_ARG;
    }
    const enum cds_status mutex_lock_error = cds_mutex_lock(
        &list->mutex, mutex_timeout, list->mutex_type, return_state
    );
    if (mutex_lock_error) return mutex_lock_error;
    if (return_state) *return_state = CDS_SUCCESS;
    if (!prev_node->next){
        (void)cds_push_next_linked_list_node_core(
            prev_node, free_node, CDS_SINGLY_LINKED_LIST
        );
        *free_node_holder = (struct cds_singly_linked_list_node*)0;
        (void)mtx_unlock(&list->mutex);
        return CDS_SUCCESS;
    }
    if (!free_node){
        *free_node_holder 
            = cds_pop_next_singly_linked_list_node_core(prev_node);
        prev_node->next = (struct cds_singly_linked_list_node*)0;
        (void)mtx_unlock(&list->mutex);
        return CDS_SUCCESS;
    }
    struct cds_singly_linked_list_node* const popped_node
        = cds_pop_next_singly_linked_list_node_core(prev_node);
    (void)cds_push_next_linked_list_node_core(
        prev_node, free_node, CDS_SINGLY_LINKED_LIST
    );
    *free_node_holder = popped_node;
    (void)mtx_unlock(&list->mutex);
    return CDS_SUCCESS;
}
enum cds_status cds_swap_singly_linked_list_with_timeout(
    struct cds_singly_linked_list* const list_0,
    struct cds_singly_linked_list* const list_1,
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
    cds_swap_linked_list_core(list_0, list_1);
    (void)mtx_unlock(&list_0->mutex);
    (void)mtx_unlock(&list_1->mutex);
    if (return_state) *return_state = CDS_SUCCESS;
    return CDS_SUCCESS;
}

static void cds_move_singly_linked_list(
    struct cds_singly_linked_list *restrict const dest_list,
    struct cds_singly_linked_list* const src_list,
    struct cds_singly_linked_list_node* const new_end_node
){
    struct cds_singly_linked_list_node* src_node_iter = src_list->front;
    src_node_iter->list = dest_list;
    if (src_node_iter->next){
        struct cds_singly_linked_list_node* src_prev = src_node_iter;
        src_node_iter = src_node_iter->next;
        for (
            size_t i = 0; 
            i < SIZE_MAX, src_node_iter;
            ++i, src_prev = src_node_iter, src_node_iter = src_node_iter->next
        ) src_node_iter->list = dest_list;
        src_prev->next = new_end_node;
    } else src_node_iter->next = new_end_node;
    src_list->front = (struct cds_singly_linked_list_node*)0;
}

struct cds_singly_linked_list* cds_splice_singly_linked_list_with_timeout(
    struct cds_singly_linked_list_node *restrict const dest_prev,
    struct cds_singly_linked_list *restrict const src_list,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!dest_prev){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    struct cds_singly_linked_list* const dest_list = dest_prev->list;
    if (!dest_list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    if (!src_list){
        if (return_state) *return_state = CDS_SUCCESS;
        return dest_prev->list;
    }
    enum cds_status local_return_state = cds_mutex_lock(
        &dest_list->mutex, mutex_timeout, dest_list->mutex_type, return_state
    );
    if (local_return_state) return (struct cds_singly_linked_list*)0;
    local_return_state = cds_mutex_lock(
        &src_list->mutex, mutex_timeout, src_list->mutex_type, return_state
    );
    if (local_return_state){
        (void)mtx_unlock(&dest_list->mutex);
        return (struct cds_singly_linked_list*)0;
    }
    struct cds_singly_linked_list_node* src_node = src_list->front;
    if (!src_node){
        (void)mtx_unlock(&src_list->mutex);
        (void)mtx_unlock(&dest_list->mutex);
        if (return_state) *return_state = CDS_SUCCESS;
        return dest_list;
    }
    cds_move_singly_linked_list(dest_list, src_list, dest_prev->next);
    dest_prev->next = src_list->front;
    (void)mtx_unlock(&src_list->mutex);
    (void)mtx_unlock(&dest_list->mutex);
    if (return_state) *return_state = CDS_SUCCESS;
    return dest_list;
}

struct cds_singly_linked_list* 
cds_prepend_singly_linked_list_with_list_with_timeout(
    struct cds_singly_linked_list* const dest_list,
    struct cds_singly_linked_list* const src_list,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!src_list){
        if (return_state) *return_state = CDS_SUCCESS;
        return dest_list;
    }
    if (!dest_list){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    enum cds_status local_return_state = cds_mutex_lock(
        &dest_list->mutex, mutex_timeout, dest_list->mutex_type, return_state
    );
    if (local_return_state) return (struct cds_singly_linked_list*)0;
    local_return_state = cds_mutex_lock(
        &src_list->mutex, mutex_timeout, src_list->mutex_type, return_state
    );
    if (local_return_state){
        (void)mtx_unlock(&dest_list->mutex);
        return (struct cds_singly_linked_list*)0;
    }
    struct cds_singly_linked_list_node* src_node = src_list->front;
    if (!src_node){
        (void)mtx_unlock(&src_list->mutex);
        (void)mtx_unlock(&dest_list->mutex);
        if (return_state) *return_state = CDS_SUCCESS;
        return dest_list;
    }
    cds_move_singly_linked_list(dest_list, src_list, dest_list->front);
    dest_list->front = src_list->front;
    (void)mtx_unlock(&src_list->mutex);
    (void)mtx_unlock(&dest_list->mutex);
    if (return_state) *return_state = CDS_SUCCESS;
    return dest_list;
}

static void cds_move_singly_linked_list_range(
    struct cds_singly_linked_list *restrict const dest_list,
    struct cds_singly_linked_list_node* src_node_iter,
    struct cds_singly_linked_list_node* const src_end_prev,
    struct cds_singly_linked_list_node* const new_end_node
){
    const struct cds_singly_linked_list_node* const src_end_node
        = src_end_prev->next;
    for (
        size_t i = 0; 
        i < SIZE_MAX, src_node_iter != src_end_node; 
        ++i, src_node_iter = src_node_iter->next
    ) src_node_iter->list = dest_list;
    src_end_prev->next = new_end_node;
}

static struct cds_singly_linked_list* cds_splice_singly_linked_list_range_core(
    struct cds_singly_linked_list_node* const dest_prev,
    struct cds_singly_linked_list_node* const src_begin_prev,
    struct cds_singly_linked_list_node* const src_end_prev
){
    struct cds_singly_linked_list_node* const src_end_node
        = src_end_prev->next;
    cds_move_singly_linked_list_range(
        dest_prev->list, src_begin_prev->next, src_end_prev, dest_prev->next
    );
    dest_prev->next = src_begin_prev->next;
    src_begin_prev->next = src_end_node;
    return dest_prev->list;
}

struct cds_singly_linked_list* cds_splice_singly_linked_list_range_with_timeout(
    struct cds_singly_linked_list_node* const dest_prev,
    struct cds_singly_linked_list_node* const src_begin_prev,
    struct cds_singly_linked_list_node* const src_end_prev,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!dest_prev){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    struct cds_singly_linked_list* const dest_list = dest_prev->list;
    if (!dest_list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    if (!src_begin_prev){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    if (!src_begin_prev->next && !src_end_prev){
        if (return_state) *return_state = CDS_SUCCESS;
        return dest_list;
    }
    if (!src_end_prev){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    if (!src_begin_prev->next){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    if (src_begin_prev->list != src_end_prev->list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    struct cds_singly_linked_list* const src_list = src_begin_prev->list;
    if (!src_list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    if (src_begin_prev == src_end_prev){
        if (return_state) *return_state = CDS_SUCCESS;
        return dest_list;
    }
    enum cds_status local_return_state = cds_mutex_lock(
        &dest_list->mutex, mutex_timeout, dest_list->mutex_type, return_state
    );
    if (local_return_state) return (struct cds_singly_linked_list*)0;
    local_return_state = cds_mutex_lock(
        &src_list->mutex, mutex_timeout, src_list->mutex_type, return_state
    );
    if (local_return_state){
        (void)mtx_unlock(&dest_list->mutex);
        return (struct cds_singly_linked_list*)0;
    }
    (void)cds_splice_singly_linked_list_range_core(
        dest_prev, src_begin_prev, src_end_prev
    );
    (void)mtx_unlock(&src_list->mutex);
    (void)mtx_unlock(&dest_list->mutex);
    if (return_state) *return_state = CDS_SUCCESS;
    return dest_list;
}

static struct cds_singly_linked_list* 
cds_prepend_singly_linked_list_with_range_core(
    struct cds_singly_linked_list *restrict const dest_list,
    struct cds_singly_linked_list_node* const src_begin_prev,
    struct cds_singly_linked_list_node* const src_end_prev
){
    struct cds_singly_linked_list_node* const src_end_node
        = src_end_prev->next;
    cds_move_singly_linked_list_range(
        dest_list, src_begin_prev->next, src_end_prev, dest_list->front
    );
    dest_list->front = src_begin_prev->next;
    src_begin_prev->next = src_end_node;
    return dest_list;
}

struct cds_singly_linked_list* 
cds_prepend_singly_linked_list_with_range_with_timeout(
    struct cds_singly_linked_list *restrict const dest_list,
    struct cds_singly_linked_list_node* const src_begin_prev,
    struct cds_singly_linked_list_node* const src_end_prev,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!src_begin_prev){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    if (!src_begin_prev->next && !src_end_prev){
        if (return_state) *return_state = CDS_SUCCESS;
        return dest_list;
    }
    if (!src_begin_prev->next){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    if (!src_end_prev){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    if (src_begin_prev->list != src_end_prev->list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    if (!dest_list){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    if (src_begin_prev == src_end_prev){
        if (return_state) *return_state = CDS_SUCCESS;
        return dest_list;
    }
    enum cds_status local_return_state = cds_mutex_lock(
        &dest_list->mutex, mutex_timeout, dest_list->mutex_type, return_state
    );
    if (local_return_state) return (struct cds_singly_linked_list*)0;
    struct cds_singly_linked_list* const src_list = src_begin_prev->list;
    if (!src_list){
        (void)mtx_unlock(&dest_list->mutex);
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_singly_linked_list*)0;
    } else {
        local_return_state = cds_mutex_lock(
            &src_list->mutex, mutex_timeout, src_list->mutex_type, return_state
        );
        if (local_return_state){
            (void)mtx_unlock(&dest_list->mutex);
            return (struct cds_singly_linked_list*)0;
        }
    }
    (void)cds_prepend_singly_linked_list_with_range_core(
        dest_list, src_begin_prev, src_end_prev
    );
    (void)mtx_unlock(&src_list->mutex);
    (void)mtx_unlock(&dest_list->mutex);
    if (return_state) *return_state = CDS_SUCCESS;
    return dest_list;
}

struct cds_singly_linked_list*
cds_splice_singly_linked_list_beginning_with_timeout(
    struct cds_singly_linked_list_node* const dest_prev,
    struct cds_singly_linked_list_node* const src_end_prev,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!dest_prev){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    struct cds_singly_linked_list* const dest_list = dest_prev->list;
    if (!dest_list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    if (!src_end_prev){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    struct cds_singly_linked_list* const src_list = src_end_prev->list;
    if (!src_list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    enum cds_status local_return_state = cds_mutex_lock(
        &dest_list->mutex, mutex_timeout, dest_list->mutex_type, return_state
    );
    if (local_return_state) return (struct cds_singly_linked_list*)0;
    local_return_state = cds_mutex_lock(
        &src_list->mutex, mutex_timeout, src_list->mutex_type, return_state
    );
    if (local_return_state){
        (void)mtx_unlock(&dest_list->mutex);
        return (struct cds_singly_linked_list*)0;
    }
    struct cds_singly_linked_list_node* const src_end_node
        = src_end_prev->next;
    cds_move_singly_linked_list_range(
        dest_list, src_list->front, src_end_prev, dest_prev->next
    );
    dest_prev->next = src_list->front;
    src_list->front = src_end_node;
    (void)mtx_unlock(&src_list->mutex);
    (void)mtx_unlock(&dest_list->mutex);
    if (return_state) *return_state = CDS_SUCCESS;
    return dest_list;
}

struct cds_singly_linked_list*
cds_prepend_singly_linked_list_with_beginning_with_timeout(
    struct cds_singly_linked_list *restrict const dest_list,
    struct cds_singly_linked_list_node *restrict const src_end_prev,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!src_end_prev){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    struct cds_singly_linked_list* const src_list = src_end_prev->list;
    if (!src_list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    if (!dest_list){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    enum cds_status local_return_state = cds_mutex_lock(
        &dest_list->mutex, mutex_timeout, dest_list->mutex_type, return_state
    );
    if (local_return_state) return (struct cds_singly_linked_list*)0;
    local_return_state = cds_mutex_lock(
        &src_list->mutex, mutex_timeout, src_list->mutex_type, return_state
    );
    if (local_return_state){
        (void)mtx_unlock(&dest_list->mutex);
        return (struct cds_singly_linked_list*)0;
    }
    struct cds_singly_linked_list_node* const src_end_node
        = src_end_prev->next;
    cds_move_singly_linked_list_range(
        dest_list, src_list->front, src_end_prev, dest_list->front
    );
    dest_list->front = src_list->front;
    src_list->front = src_end_node;
    (void)mtx_unlock(&src_list->mutex);
    (void)mtx_unlock(&dest_list->mutex);
    if (return_state) *return_state = CDS_SUCCESS;
    return dest_list;
}

struct cds_singly_linked_list*
cds_pop_singly_linked_list_range_with_timeout(
    struct cds_singly_linked_list_node* const begin_prev,
    struct cds_singly_linked_list_node* const end_prev,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!begin_prev){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    struct cds_singly_linked_list* const list = begin_prev->list;
    if (!list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    if (!end_prev){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    if (begin_prev->list != end_prev->list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    struct cds_singly_linked_list* return_list 
        = cds_create_singly_linked_list_with_mutex_type(
            list->mutex_type, return_state
        );
    if (!return_list) return return_list;
    if (begin_prev == end_prev){
        if (return_state) *return_state = CDS_SUCCESS;
        return return_list;
    }
    enum cds_status local_return_state = cds_mutex_lock(
        &list->mutex, mutex_timeout, list->mutex_type, return_state
    );
    if (local_return_state){
        (void)cds_destroy_singly_linked_list(&return_list, return_state);
        return (struct cds_singly_linked_list*)0;
    }
    struct cds_singly_linked_list_node* const end_node = end_prev->next;
    cds_move_singly_linked_list_range(
        return_list, begin_prev->next, end_prev, return_list->front
    );
    return_list->front = begin_prev->next;
    begin_prev->next = end_node;
    (void)mtx_unlock(&list->mutex);
    if (return_state) *return_state = CDS_SUCCESS;
    return return_list;
}

struct cds_singly_linked_list*
cds_pop_singly_linked_list_beginning_with_timeout(
    struct cds_singly_linked_list_node *restrict const end_prev,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!end_prev){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    struct cds_singly_linked_list* const list = end_prev->list;
    if (!list){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return (struct cds_singly_linked_list*)0;
    }
    struct cds_singly_linked_list* return_list 
        = cds_create_singly_linked_list_with_mutex_type(
            list->mutex_type, return_state
        );
    if (!return_list) return return_list;
    enum cds_status local_return_state = cds_mutex_lock(
        &list->mutex, mutex_timeout, list->mutex_type, return_state
    );
    if (local_return_state){
        (void)cds_destroy_singly_linked_list(&return_list, return_state);
        return (struct cds_singly_linked_list*)0;
    }
    struct cds_singly_linked_list_node* const end_node = end_prev->next;
    cds_move_singly_linked_list_range(
        return_list, list->front, end_prev, return_list->front
    );
    return_list->front = list->front;
    list->front = end_node;
    (void)mtx_unlock(&list->mutex);
    if (return_state) *return_state = CDS_SUCCESS;
    return return_list;
}