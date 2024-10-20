#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "alloc.h"
#include "mutex.h"
#include "singly_linked_list_type.h"
#include "singly_linked_list.h"

/// @brief Destroy the input list node.
///     If the input node is part of a list, the node will not be destroyed.
/// @param[in,out] node The node to destroy.
/// @return If the input node is part of a list, the function will return the
///     input node. Otherwise, the function will return nullptr.
struct cds_singly_linked_list_node* cds_destroy_free_singly_linked_list_node(
    struct cds_singly_linked_list_node** const node_holder
){
    struct cds_singly_linked_list_node* const node = *node_holder;
    if (!node || node->list) return (struct cds_singly_linked_list_node*)0;
    free(node);
    *node_holder = (struct cds_singly_linked_list_node*)0;
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
    const struct timespec *restrict const mutex_timeout
){
    if (
        !list || !node 
            || cds_mutex_lock(&list->mutex, mutex_timeout, list->mutex_type)
                == CDS_MUTEX_TIMEOUT
    ) return (struct cds_singly_linked_list*)0;
    cds_singly_linked_list_push_front_core(list, node);
    cds_mutex_unlock(&list->mutex);
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
    const struct timespec *restrict const mutex_timeout
){
    if (!prev) return (struct cds_singly_linked_list_node*)0;
    struct cds_singly_linked_list* const list = prev->list;
    if (
        !list 
            || cds_mutex_lock(&list->mutex, mutex_timeout, list->mutex_type) 
                == CDS_MUTEX_TIMEOUT
    ) return (struct cds_singly_linked_list_node*)0;
    if (!prev->next){
        cds_mutex_unlock(&list->mutex);
        return prev->next;
    }
    struct cds_singly_linked_list_node* node 
        = cds_pop_next_singly_linked_list_node_core(prev);
    cds_mutex_unlock(&list->mutex);
    return node;
}

/// @brief Destroy the following node of the input node.
///     Iterating through the list while calling this function in a
///     multithreaded environment will result in undefined behaviour.
/// @param[in] prev The node whose next node is to be destroyed.
enum cds_status cds_destroy_next_singly_linked_list_node_with_timeout(
    struct cds_singly_linked_list_node *restrict const prev,
    const struct timespec *restrict const mutex_timeout
){
    if (!prev) return CDS_NULL_ARG;
    struct cds_singly_linked_list* const list = prev->list;
    if (!list) return CDS_NULL_ARG;
    if (
        cds_mutex_lock(&list->mutex, mutex_timeout, list->mutex_type) 
            == CDS_MUTEX_TIMEOUT
    ) return CDS_MUTEX_TIMEOUT;
    if (!prev->next){
        cds_mutex_unlock(&list->mutex);
        return CDS_SUCCESS;
    }
    struct cds_singly_linked_list_node* const next_node = prev->next;
    prev->next = next_node->next;
    cds_mutex_unlock(&list->mutex);
    free(next_node);
    return CDS_SUCCESS;
}

/// @brief Invert the input list in place.
/// @param[in,out] list The list to invert.
/// @return The input list.
struct cds_singly_linked_list* cds_invert_singly_linked_list_with_timeout(
    struct cds_singly_linked_list *restrict const list,
    const struct timespec *restrict const mutex_timeout
){
    if (!list) return list;
    if (
        cds_mutex_lock(&list->mutex, mutex_timeout, list->mutex_type)
            == CDS_MUTEX_TIMEOUT
    ) return (struct cds_singly_linked_list*)0;
    if (!list->front){
        cds_mutex_unlock(&list->mutex);
        return list;
    }
    struct cds_singly_linked_list_node* prev 
        = (struct cds_singly_linked_list_node*)0;
    struct cds_singly_linked_list_node* node = list->front;
    while (node){
        struct cds_singly_linked_list_node* const next_node = node->next;
        node->next = prev;
        prev = node;
        node = next_node;
    }
    list->front = prev;
    cds_mutex_unlock(&list->mutex);
    return list;
}

enum cds_status cds_erase_following_singly_linked_list_nodes_with_timeout(
    struct cds_singly_linked_list_node *restrict const prev,
    const struct timespec *restrict const mutex_timeout
){
    if (!prev) return CDS_NULL_ARG;
    struct cds_singly_linked_list* const list = prev->list;
    if (!list) return CDS_NULL_ARG;
    if (
        cds_mutex_lock(&list->mutex, mutex_timeout, list->mutex_type)
            == CDS_MUTEX_TIMEOUT
    ) return CDS_MUTEX_TIMEOUT;
    if (!prev->next){
        cds_mutex_unlock(&list->mutex);
        return CDS_SUCCESS;
    }
    cds_erase_following_linked_list_nodes_core(prev);
    prev->next = (struct cds_singly_linked_list_node*)0;
    cds_mutex_unlock(&list->mutex);
    return CDS_SUCCESS;
}

struct cds_singly_linked_list* 
cds_erase_preceding_singly_linked_list_nodes_with_timeout(
    struct cds_singly_linked_list *restrict const list,
    struct cds_singly_linked_list_node *restrict const node,
    const bool is_inclusive,
    const struct timespec *restrict const mutex_timeout
){
    if (!list) return list;
    if (!node) return cds_empty_singly_linked_list(list);
    if (
        !node->list || node->list != list
            || cds_mutex_lock(&list->mutex, mutex_timeout, list->mutex_type)
                == CDS_MUTEX_TIMEOUT
    ) return (struct cds_singly_linked_list*)0;
    if (!list->front || list->front == node){ 
        cds_mutex_unlock(&list->mutex);
        return list;
    }
    struct cds_singly_linked_list_node* prev = list->front;
    struct cds_singly_linked_list_node* next = prev->next;
    while (next && next != ((is_inclusive && node->next) ? node->next : node)){
        free(prev);
        prev = next;
        next = next->next;
    }
    free(prev);
    list->front = next;
    cds_mutex_unlock(&list->mutex);
    return list;
}

struct cds_singly_linked_list* cds_singly_linked_list_remove_if_with_timeout(
    struct cds_singly_linked_list *restrict const list,
    bool (*predicate)(const struct cds_singly_linked_list_node* const node),
    const struct timespec *restrict const mutex_timeout
){
    if (!list || !predicate) return list;
    if (
        cds_mutex_lock(&list->mutex, mutex_timeout, list->mutex_type)
            == CDS_MUTEX_TIMEOUT
    ) return (struct cds_singly_linked_list*)0;
    struct cds_singly_linked_list_node* prev 
        = (struct cds_singly_linked_list_node*)0;
    struct cds_singly_linked_list_node* node = list->front;
    while (node){
        if (predicate(node)){
            struct cds_singly_linked_list_node* const next_node = node->next;
            if (prev) prev->next = next_node; else list->front = next_node;
            free(node);
            node = next_node;
            continue;
        }
        prev = node;
        node = node->next;
    }
    cds_mutex_unlock(&list->mutex);
    return list;
}

enum cds_status cds_swap_next_singly_linked_list_nodes_with_timeout(
    struct cds_singly_linked_list_node* const prev_node_0,
    struct cds_singly_linked_list_node* const prev_node_1,
    const struct timespec *restrict const mutex_timeout
){
    if (!prev_node_0 || !prev_node_1) return CDS_NULL_ARG;
    struct cds_singly_linked_list* const list_0 = prev_node_0->list;
    struct cds_singly_linked_list* const list_1 = prev_node_1->list;
    if (!list_0 || !list_1) return CDS_NULL_ARG;
    struct cds_singly_linked_list_node* const node_0 = prev_node_0->next;
    struct cds_singly_linked_list_node* const node_1 = prev_node_1->next;
    if (!node_0){
        prev_node_0->next = cds_pop_next_singly_linked_list_node_with_timeout(
            prev_node_1, mutex_timeout
        );
        return CDS_SUCCESS;
    }
    if (!node_1){
        prev_node_1->next = cds_pop_next_singly_linked_list_node_with_timeout(
            prev_node_0, mutex_timeout
        );
        return CDS_SUCCESS;
    }
    if (
        cds_mutex_lock(&list_0->mutex, mutex_timeout, list_0->mutex_type)
            == CDS_MUTEX_TIMEOUT
    ) return CDS_MUTEX_TIMEOUT;
    const bool needs_lock_node_1 = list_1 != list_0;
    if (
        needs_lock_node_1
            && cds_mutex_lock(&list_1->mutex, mutex_timeout, list_1->mutex_type)
                == CDS_MUTEX_TIMEOUT
    ){
        cds_mutex_unlock(&list_0->mutex);
        return CDS_MUTEX_TIMEOUT;
    }
    struct cds_singly_linked_list_node* const temp_node = prev_node_0->next;
    prev_node_0->next = prev_node_1->next;
    prev_node_1->next = temp_node;
    struct cds_singly_linked_list* const temp_list = node_0->list;
    node_0->list = list_1;
    node_1->list = temp_list;
    struct cds_singly_linked_list_node* const temp_next = node_0->next;
    node_0->next = node_1->next;
    node_1->next = temp_next;
    if (needs_lock_node_1) cds_mutex_unlock(&list_1->mutex);
    cds_mutex_unlock(&list_0->mutex);
    return CDS_SUCCESS;
}

enum cds_status cds_swap_free_and_next_singly_linked_list_nodes_with_timeout(
    struct cds_singly_linked_list_node *restrict const prev_node,
    struct cds_singly_linked_list_node *restrict *restrict const 
        free_node_holder,
    const struct timespec *restrict const mutex_timeout
){
    struct cds_singly_linked_list_node* const free_node = *free_node_holder;
    if (!prev_node || !free_node) return CDS_NULL_ARG;
    struct cds_singly_linked_list* const list = prev_node->list;
    if (!list) return CDS_NULL_ARG;
    if ((free_node && free_node->list)) return CDS_INVALID_ARG;
    if (
        cds_mutex_lock(&list->mutex, mutex_timeout, list->mutex_type)
            == CDS_MUTEX_TIMEOUT
    ) return CDS_MUTEX_TIMEOUT;
    if (!prev_node->next){
        cds_push_next_linked_list_node_core(prev_node, free_node, (void*)0);
        *free_node_holder = (struct cds_singly_linked_list_node*)0;
        cds_mutex_unlock(&list->mutex);
        return CDS_SUCCESS;
    }
    if (!free_node){
        *free_node_holder 
            = cds_pop_next_singly_linked_list_node_core(prev_node);
        prev_node->next = (struct cds_singly_linked_list_node*)0;
        cds_mutex_unlock(&list->mutex);
        return CDS_SUCCESS;
    }
    struct cds_singly_linked_list_node* const popped_node
        = cds_pop_next_singly_linked_list_node_core(prev_node);
    cds_push_next_linked_list_node_core(prev_node, free_node, (void*)0);
    *free_node_holder = popped_node;
    cds_mutex_unlock(&list->mutex);
    return CDS_SUCCESS;
}