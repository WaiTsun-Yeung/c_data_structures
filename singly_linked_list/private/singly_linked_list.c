#include <stddef.h>
#include <stdbool.h>

#include <omp.h>

#include "utilities.h"
#include "singly_linked_list_type.h"
#include "singly_linked_list.h"

/// @brief Destroy the input list node.
///     If the input node is part of a list, the node will not be destroyed.
/// @param[in,out] node The node to destroy.
/// @return If the input node is part of a list, the function will return the
///     input node. Otherwise, the function will return nullptr.
struct cds_singly_linked_list_node* cds_destroy_free_singly_linked_list_node(
    struct cds_singly_linked_list_node** const node
){
    if (!*node || (*node)->list) return *node;
    free(*node);
    *node = (struct cds_singly_linked_list_node*)0;
    return *node;
}

struct cds_singly_linked_list* 
cds_singly_linked_list_push_front_core(
    struct cds_singly_linked_list* const list,
    struct cds_singly_linked_list_node* const node
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
struct cds_singly_linked_list* cds_singly_linked_list_push_front(
    struct cds_singly_linked_list* const list,
    struct cds_singly_linked_list_node* const node
){
    if (!list || !node) return (struct cds_singly_linked_list*)0;
    omp_set_lock(&list->lock);
    cds_singly_linked_list_push_front_core(list, node);
    omp_unset_lock(&list->lock);
    return list;
}

/// @brief Pop the node after the input node.
///     The ownership of the node is transferred to the caller, and the caller
///     is responsible for destroying the node by calling free().
/// @param[in,out] prev The node to pop the next node from.
/// @param[in] lock The mutex lock of the list.
/// @return The next node of the input node if successful; otherwise, nullptr.
struct cds_singly_linked_list_node* cds_pop_next_singly_linked_list_node(
    struct cds_singly_linked_list_node* const prev
){
    if (!prev || !prev->list) return (struct cds_singly_linked_list_node*)0;
    omp_set_lock(&prev->list->lock);
    if (!prev->next){
        omp_unset_lock(&prev->list->lock);
        return prev->next;
    }
    struct cds_singly_linked_list_node* const node = prev->next;
    node->list = (struct cds_singly_linked_list*)0;
    prev->next = node->next;
    omp_unset_lock(&prev->list->lock);
    return node;
}

/// @brief Destroy the following node of the input node.
///     Iterating through the list while calling this function in a
///     multithreaded environment will result in undefined behaviour.
/// @param[in] prev The node whose next node is to be destroyed.
void cds_destroy_next_singly_linked_list_node(
    struct cds_singly_linked_list_node* const prev
){
    if (!prev || !prev->list) return;
    omp_set_lock(&prev->list->lock);
    if (!prev->next){
        omp_unset_lock(&prev->list->lock);
        return;
    }
    struct cds_singly_linked_list_node* const next_node = prev->next;
    prev->next = next_node->next;
    omp_unset_lock(&prev->list->lock);
    free(next_node);
    return;
}

/// @brief Invert the input list in place.
/// @param[in,out] list The list to invert.
/// @return The input list.
struct cds_singly_linked_list* cds_invert_singly_linked_list(
    struct cds_singly_linked_list* const list
){
    if (!list) return (struct cds_singly_linked_list*)0;
    omp_set_lock(&list->lock);
    if (!list->front){
        omp_unset_lock(&list->lock);
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
    omp_unset_lock(&list->lock);
    return list;
}

void cds_erase_following_singly_linked_list_nodes(
    struct cds_singly_linked_list_node* const prev
){
    if (!prev || !prev->list) return;
    omp_set_lock(&prev->list->lock);
    if (!prev->next){
        omp_unset_lock(&prev->list->lock);
        return;
    }
    cds_erase_following_linked_list_nodes_core(prev);
    prev->next = (struct cds_singly_linked_list_node*)0;
    omp_unset_lock(&prev->list->lock);
    return;
}

struct cds_singly_linked_list* cds_erase_preceding_singly_linked_list_nodes(
    struct cds_singly_linked_list* const list,
    struct cds_singly_linked_list_node* const node,
    const bool is_inclusive
){
    if (!list) return list;
    if (!node) return cds_empty_singly_linked_list(list);
    if (!node->list || node->list != list) return list;
    omp_set_lock(&list->lock);
    if (!list->front || list->front == node){ 
        omp_unset_lock(&list->lock);
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
    omp_unset_lock(&list->lock);
    return list;
}

struct cds_singly_linked_list* cds_singly_linked_list_remove_if(
    struct cds_singly_linked_list* const list,
    bool (*predicate)(const struct cds_singly_linked_list_node* const node)
){
    if (!list || !predicate) return list;
    omp_set_lock(&list->lock);
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
    omp_unset_lock(&list->lock);
    return list;
}