#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <omp.h>

#include "utilities.h"
#include "list_type.h"

/// @brief Compute the number of bytes of a cds_list_node with the given
///     bytes_per_element.
/// @param[in] bytes_per_element The number of bytes for the data element.
/// @return The number of bytes of a cds_list_node with the given bytes_per_element.
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
///     The address of the src node's next node is also copied.
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
        list->front = (struct cds_list_node*)0;
        return list;
    }
    const struct cds_list_node* src_node = src->front;
    struct cds_list_node* dest_node = cds_copy_and_create_list_node(src_node);
    list->front = dest_node;
    while (src_node->next){
        src_node = src_node->next;
        dest_node->next = cds_copy_and_create_list_node(src_node);
        dest_node = dest_node->next;
    }
    omp_unset_lock(&src->lock);
    return list;
}

/// @brief Destroy the following node of the input node.
///     Iterating through the list while calling this function in a
///     multithreaded environment will result in undefined behaviour.
/// @param[in] node The node whose next node is to be destroyed.
/// @return The input node.
void cds_destroy_next_list_node(
    struct cds_list_node* const prev, omp_lock_t* const lock
){
    if (!prev) return;
    if (!prev->next) return;
    omp_set_lock(lock);
    struct cds_list_node* const next_node = prev->next;
    prev->next = next_node->next;
    omp_unset_lock(lock);
    free(next_node);
    return;
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