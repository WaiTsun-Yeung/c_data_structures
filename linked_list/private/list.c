#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include <omp.h>

#include "utilities.h"
#include "list_type.h"

/// @brief Compute the number of bytes of a cds_list_node with the given
///     bytes_per_element.
/// @param[in] bytes_per_element The number of bytes for the data element.
/// @return The number of bytes of a cds_list_node with the given 
///     bytes_per_element.
static inline size_t cds_compute_list_node_bytes_count(
    const size_t bytes_per_element
){
    return sizeof(struct cds_list_node) + bytes_per_element;
}

static inline struct cds_list_node* cds_malloc_list_node(
    const size_t bytes_per_element
){
    return cds_malloc_buffer(
        cds_compute_list_node_bytes_count(bytes_per_element)
    );
}

struct cds_list_node* cds_create_list_node(
    const size_t bytes_per_element
){
    struct cds_list_node* const node = cds_malloc_list_node(bytes_per_element);
    node->bytes_per_element = bytes_per_element;
    node->lock = (omp_lock_t*)0;
    node->next = (struct cds_list_node*)0;
    node->data = (uint8_t*)node + sizeof(struct cds_list_node);
    return node;
}

struct cds_list* cds_create_list(){
    struct cds_list* const list = cds_malloc_buffer(sizeof(struct cds_list));
    list->front = (struct cds_list_node*)0;
    list->hint = omp_sync_hint_none;
    omp_init_lock(&list->lock);
    return list;
}

struct cds_list* cds_create_list_with_hint(
    const omp_sync_hint_t hint
){
    struct cds_list* const list = cds_malloc_buffer(sizeof(struct cds_list));
    list->front = (struct cds_list_node*)0;
    list->hint = hint;
    omp_init_lock_with_hint(&list->lock, hint);
    return list;
}

/// @brief Copy src node to create a new heap-allocated cds_list_node.
///     The address of the src node's next node 
///     and the address of the src list's mutex lock are also copied.
///     omp_set_lock() and omp_unset_lock() must be called before and after
///     calling this function in a multithreaded environment.
/// @param[in] src The source cds_list_node to copy from.
/// @return The copied node.
struct cds_list_node* cds_copy_and_create_list_node(
    const struct cds_list_node* const src
){
    if (!src) return (struct cds_list_node*)0;
    const size_t node_bytes_count 
        = cds_compute_list_node_bytes_count(src->bytes_per_element);
    struct cds_list_node* const node = cds_malloc_buffer(node_bytes_count);
    memcpy(node, src, node_bytes_count);
    return node;
}

struct cds_list* cds_copy_and_create_list(
    struct cds_list* const src
){
    if (!src) return (struct cds_list*)0;
    struct cds_list* const list = cds_malloc_buffer(sizeof(struct cds_list));
    list->hint = src->hint;
    omp_init_lock_with_hint(&list->lock, list->hint);
    omp_set_lock(&src->lock);
    if (!src->front){
        omp_unset_lock(&src->lock);
        list->front = src->front;
        return list;
    }
    const struct cds_list_node* src_node = src->front;
    struct cds_list_node* dest_node = cds_copy_and_create_list_node(src_node);
    list->front = dest_node;
    while (src_node->next){
        dest_node->lock = &list->lock;
        src_node = src_node->next;
        dest_node->next = cds_copy_and_create_list_node(src_node);
        dest_node = dest_node->next;
    }
    dest_node->lock = &list->lock;
    omp_unset_lock(&src->lock);
    return list;
}

static struct cds_list* cds_destroy_empty_list(
    struct cds_list** const list
){
    omp_destroy_lock(&(*list)->lock);
    free(*list);
    *list = (struct cds_list*)0;
    return *list;
}

struct cds_list* cds_destroy_list(
    struct cds_list** const list
){
    if (!*list) return *list;
    if (!(*list)->front) return cds_destroy_empty_list(list);
    struct cds_list_node* node = (*list)->front;
    while (node->next){
        struct cds_list_node* const next_node = node->next;
        free(node);
        node = next_node;
    }
    free(node);
    return cds_destroy_empty_list(list);
}

/// @brief Push a node to the front of the list.
///     The ownership of the node is transferred to the list, and the node
///     will be destroyed when the list is destroyed.
/// @param[in,out] list The list to push the node to.
/// @param[in] node The node to push to the list.
/// @return The input list.
struct cds_list* cds_list_push_front(
    struct cds_list* const list,
    struct cds_list_node* const node
){
    if (!list || !node) return (struct cds_list*)0;
    omp_set_lock(&list->lock);
    node->lock = &list->lock;
    node->next = list->front;
    list->front = node;
    omp_unset_lock(&list->lock);
    return list;
}

/// @brief Pop the front node of the list.
///     The ownership of the node is transferred to the caller, and the caller
///     is responsible for destroying the node by calling free().
/// @param[in,out] list The list to pop the front node from.
/// @return The front node of the list.
struct cds_list_node* cds_list_pop_front(
    struct cds_list* const list
){
    if (!list) return (struct cds_list_node*)0;
    omp_set_lock(&list->lock);
    if (!list->front){
        omp_unset_lock(&list->lock);
        return list->front;
    }
    struct cds_list_node* const node = list->front;
    node->lock = (omp_lock_t*)0;
    list->front = node->next;
    omp_unset_lock(&list->lock);
    return node;
}

void cds_list_destroy_front(struct cds_list* const list){
    if (!list) return;
    if (!list->front) return;
    omp_set_lock(&list->lock);
    struct cds_list_node* const node = list->front;
    list->front = node->next;
    omp_unset_lock(&list->lock);
    free(node);
    return;
}

/// @brief Insert a node after the input node.
///     The ownership of the node is transferred to the list, and the node
///     will be destroyed when the list is destroyed.
/// @param[in] prev The node to insert the new node after.
/// @param[in] node The node to insert.
/// @param[in] lock The mutex lock of the list.
void cds_push_next_list_node(
    struct cds_list_node* const prev,
    struct cds_list_node* const node
){
    if (!prev || !node) return;
    if (!prev->lock) return;
    omp_set_lock(prev->lock);
    node->lock = prev->lock;
    node->next = prev->next;
    prev->next = node;
    omp_unset_lock(prev->lock);
    return;
}

/// @brief Pop the node after the input node.
///     The ownership of the node is transferred to the caller, and the caller
///     is responsible for destroying the node by calling free().
/// @param[in] prev The node to pop the next node from.
/// @param[in] lock The mutex lock of the list.
/// @return The next node of the input node.
struct cds_list_node* cds_pop_next_list_node(
    struct cds_list_node* const prev
){
    if (!prev) return (struct cds_list_node*)0;
    if (!prev->lock) return (struct cds_list_node*)0;
    omp_set_lock(prev->lock);
    if (!prev->next){
        omp_unset_lock(prev->lock);
        return prev->next;
    }
    struct cds_list_node* const node = prev->next;
    node->lock = (omp_lock_t*)0;
    prev->next = node->next;
    omp_unset_lock(prev->lock);
    return node;
}

/// @brief Destroy the following node of the input node.
///     Iterating through the list while calling this function in a
///     multithreaded environment will result in undefined behaviour.
/// @param[in] node The node whose next node is to be destroyed.
/// @return The input node.
void cds_destroy_next_list_node(
    struct cds_list_node* const prev
){
    if (!prev) return;
    if (!prev->next) return;
    if (!prev->lock) return;
    omp_set_lock(prev->lock);
    struct cds_list_node* const next_node = prev->next;
    prev->next = next_node->next;
    omp_unset_lock(prev->lock);
    free(next_node);
    return;
}

struct cds_list* cds_copy_and_create_reverse_list(
    struct cds_list* const src
){
    if (!src) return (struct cds_list*)0;
    struct cds_list* const list = cds_malloc_buffer(sizeof(struct cds_list));
    list->hint = src->hint;
    omp_init_lock_with_hint(&list->lock, list->hint);
    list->front = (struct cds_list_node*)0;
    omp_set_lock(&src->lock);
    if (!src->front){
        omp_unset_lock(&src->lock);
        return list;
    }
    const struct cds_list_node* src_node = src->front;
    do {
        struct cds_list_node* const dest_node 
            = cds_copy_and_create_list_node(src_node);
        cds_list_push_front(list, dest_node);
        src_node = src_node->next;
    } while (src_node);
    omp_unset_lock(&src->lock);
    return list;
}

void cds_erase_following_list_nodes(
    struct cds_list_node* const prev
){
    if (!prev) return;
    if (!prev->next) return;
    omp_set_lock(prev->lock);
    struct cds_list_node* node = prev->next;
    while (node){
        struct cds_list_node* const next_node = node->next;
        free(node);
        node = next_node;
    }
    prev->next = (struct cds_list_node*)0;
    omp_unset_lock(prev->lock);
    return;
}

/// @brief Swap the nodes of the input list.
///     If one of the nodes is a nullptr, the function will return immediately.
/// @param[in] node_0 The first node to swap.
/// @param[in] node_1 The second node to swap.
void cds_swap_list_nodes(
    struct cds_list_node** const node_0,
    struct cds_list_node** const node_1
){
    if (!*node_0 || !*node_1) return;
    if ((*node_0)->lock) omp_set_lock((*node_0)->lock);
    const bool needs_lock_node_1 
        = (*node_1)->lock && (*node_1)->lock != (*node_0)->lock;
    if (needs_lock_node_1)
        omp_set_lock((*node_1)->lock);
    struct cds_list_node* const temp = *node_0;
    *node_0 = *node_1;
    *node_1 = temp;
    omp_lock_t* const temp_lock = (*node_0)->lock;
    (*node_0)->lock = (*node_1)->lock;
    (*node_1)->lock = temp_lock;
    struct cds_list_node* const temp_next = (*node_0)->next;
    (*node_0)->next = (*node_1)->next;
    (*node_1)->next = temp_next;
    if ((*node_0)->lock) omp_unset_lock((*node_0)->lock);
    if (needs_lock_node_1) omp_unset_lock((*node_1)->lock);
    return;
}

struct cds_list_node* cds_list_node_next(
    struct cds_list_node** const node
){
    if (!*node) return (struct cds_list_node*)0;
    *node = (*node)->next;
    return *node;
}