#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include <omp.h>

#include "utilities.h"

#include "singly_linked_list_type.h"

void* cds_create_linked_list_node(
    const size_t bytes_per_node_type, const size_t bytes_per_element,
    const size_t data_align
){
    const size_t data_offset 
        = cds_compute_data_offset(bytes_per_node_type, data_align);
    struct cds_singly_linked_list_node* const node 
        = cds_malloc_buffer(data_offset + bytes_per_element);
    node->data = (uint8_t*)node + data_offset;
    node->bytes_per_element = bytes_per_element;
    node->data_align = data_align;
    node->lock = (omp_lock_t*)0;
    node->next = (struct cds_singly_linked_list_node*)0;
    return node;
}

void* cds_create_linked_list_with_hint(
    const size_t bytes_per_list_type, const omp_sync_hint_t hint
){
    struct cds_singly_linked_list* const list 
        = cds_malloc_buffer(bytes_per_list_type);
    list->hint = hint;
    omp_init_lock_with_hint(&list->lock, hint);
    list->front = (struct cds_singly_linked_list_node*)0;
    return list;
}

void* cds_copy_and_create_linked_list_node(
    const size_t bytes_per_node_type, const void* const src
){
    if (!src) return (void*)0;
    const size_t data_offset 
        = (uint8_t*)((const struct cds_singly_linked_list_node* const)src)->data 
            - (uint8_t*)src;
    const size_t node_bytes_count 
        = data_offset 
            + ((const struct cds_singly_linked_list_node* const)src)
                ->bytes_per_element;
    void* const node = cds_malloc_buffer(node_bytes_count);
    memcpy(node, src, node_bytes_count);
    ((struct cds_singly_linked_list_node*)node)->data 
        = (uint8_t*)node + data_offset;
    return node;
}

void* cds_copy_and_create_linked_list(
    const size_t bytes_per_list_type, const size_t bytes_per_node_type, 
    void* const src_list, 
    void* (*doubly_linked_list_null_front_callback)
        (void* const dest_list),
    void (*doubly_linked_list_copy_node_callback)(void* const dest_node),
    void (*doubly_linked_list_closing_callback)
        (void* const dest_list, void* const dest_node)
){
    if (!src_list) return (struct cds_singly_linked_list*)0;
    struct cds_singly_linked_list* const dest_list 
        = cds_malloc_buffer(bytes_per_list_type);
    dest_list->hint = ((struct cds_singly_linked_list*)src_list)->hint;
    omp_init_lock_with_hint(&dest_list->lock, dest_list->hint);
    omp_set_lock(&((struct cds_singly_linked_list*)src_list)->lock);
    if (!((struct cds_singly_linked_list*)src_list)->front){
        omp_unset_lock(&((struct cds_singly_linked_list*)src_list)->lock);
        dest_list->front = ((struct cds_singly_linked_list*)src_list)->front;
        return doubly_linked_list_null_front_callback
            ? doubly_linked_list_null_front_callback(dest_list)
            : (void*)0;
    }
    const struct cds_singly_linked_list_node* src_node 
        = ((struct cds_singly_linked_list*)src_list)->front;
    void* dest_node 
        = cds_copy_and_create_linked_list_node(bytes_per_node_type, src_node);
    dest_list->front = dest_node;
    for (
        src_node = src_node->next; 
        src_node; 
        src_node = src_node->next,
        dest_node = ((struct cds_singly_linked_list_node*)dest_node)->next
    ){
        ((struct cds_singly_linked_list_node*)dest_node)->lock 
            = &dest_list->lock;
        ((struct cds_singly_linked_list_node*)dest_node)->next 
            = cds_copy_and_create_linked_list_node(
                bytes_per_node_type, src_node
            );
        if (doubly_linked_list_copy_node_callback)
            doubly_linked_list_copy_node_callback(dest_node);
    }
    ((struct cds_singly_linked_list_node*)dest_node)->lock = &dest_list->lock;
    if (doubly_linked_list_closing_callback)
        doubly_linked_list_closing_callback(dest_list, dest_node);
    omp_unset_lock(&((struct cds_singly_linked_list*)src_list)->lock);
    return dest_list;
}

static void* cds_set_linked_list_node_after_realloc(
    void** const node, const size_t data_offset,
    const size_t bytes_per_element, const size_t data_align
){
    ((struct cds_singly_linked_list_node*)*node)->data 
        = (uint8_t*)*node + data_offset;
    ((struct cds_singly_linked_list_node*)*node)->bytes_per_element 
        = bytes_per_element;
    ((struct cds_singly_linked_list_node*)*node)->data_align = data_align;
    return *node;
}

void* cds_change_linked_list_node_data_type(
    const size_t bytes_per_node_type, void** const node, 
    const size_t bytes_per_element, const size_t data_align
){
    if (
        !*node || (
            ((struct cds_singly_linked_list_node*)*node)->bytes_per_element 
                    == bytes_per_element
                && ((struct cds_singly_linked_list_node*)*node)->data_align 
                    == data_align
        )
    ) return node;
    const size_t data_offset 
        = cds_compute_data_offset(bytes_per_node_type, data_align);
    cds_realloc_buffer(node, data_offset + bytes_per_element);
    return cds_set_linked_list_node_after_realloc(
        node, data_offset, bytes_per_element, data_align
    );
}

void* cds_copy_linked_list_node(
    void** const dest, const void* const src
){
    if (!*dest || !src) exit(1);
    if (*dest == src) return *dest;
    const size_t data_offset 
        = (uint8_t*)((const struct cds_singly_linked_list_node* const)src)->data 
            - (uint8_t*)src;
    if (
        ((struct cds_singly_linked_list_node*)*dest)->bytes_per_element 
                != ((const struct cds_singly_linked_list_node* const)src)
                    ->bytes_per_element
            || ((struct cds_singly_linked_list_node*)*dest)->data_align 
                != ((const struct cds_singly_linked_list_node* const)src)
                    ->data_align
    ){ 
        cds_realloc_buffer(
            dest, 
            data_offset 
                + ((const struct cds_singly_linked_list_node* const)src)
                    ->bytes_per_element
        );
        cds_set_linked_list_node_after_realloc(
            dest, data_offset, 
            ((const struct cds_singly_linked_list_node* const)src)
                ->bytes_per_element, 
            ((const struct cds_singly_linked_list_node* const)src)->data_align
        );
    }
    memcpy(
        ((struct cds_singly_linked_list_node*)*dest)->data, 
        ((const struct cds_singly_linked_list_node* const)src)->data, 
        ((const struct cds_singly_linked_list_node* const)src)
            ->bytes_per_element
    );
    return *dest;
}

static void* cds_destroy_empty_linked_list(void** const list){
    omp_destroy_lock(&((struct cds_singly_linked_list*)*list)->lock);
    free(*list);
    *list = (void*)0;
    return *list;
}

void* cds_empty_linked_list(
    void* const list, void (*doubly_linked_list_callback)(void* const list),
    const bool toggle_guards_and_cleanups
){
    if (toggle_guards_and_cleanups){
        if (!list) return list;
        omp_set_lock(&((struct cds_singly_linked_list*)list)->lock);
        if (!((struct cds_singly_linked_list*)list)->front){
            omp_unset_lock(&((struct cds_singly_linked_list*)list)->lock);
            return list;
        }
    }
    struct cds_singly_linked_list_node* node 
        = ((struct cds_singly_linked_list*)list)->front;
    for (
        struct cds_singly_linked_list_node* next_node = node->next; 
        next_node; 
        next_node = node->next
    ){
        free(node);
        node = next_node;
    }
    if (toggle_guards_and_cleanups){
        ((struct cds_singly_linked_list*)list)->front 
            = (struct cds_singly_linked_list_node*)0;
        if (doubly_linked_list_callback) doubly_linked_list_callback(list);
        omp_unset_lock(&((struct cds_singly_linked_list*)list)->lock);
    }
    free(node);
    return list;
}

void* cds_destroy_linked_list(void** const list){
    if (!*list) return *list;
    omp_set_lock(&((struct cds_singly_linked_list*)*list)->lock);
    if (!((struct cds_singly_linked_list*)*list)->front) {
        omp_unset_lock(&((struct cds_singly_linked_list*)*list)->lock);
        return cds_destroy_empty_linked_list(list);
    }
    cds_empty_linked_list(*list, (void*)0, false);
    return cds_destroy_empty_linked_list(list);
}

void* cds_push_next_linked_list_node(
    void* const node,
    void* const new_node,
    void (*doubly_linked_list_callback)(void* const node, void* const new_node)
){
    if (
        !node || !new_node 
            || !((struct cds_singly_linked_list_node*)node)->lock
    ) return (void*)0;
    omp_set_lock(((struct cds_singly_linked_list_node*)node)->lock);
    if (doubly_linked_list_callback) 
        doubly_linked_list_callback(node, new_node);
    ((struct cds_singly_linked_list_node*)new_node)->lock 
        = ((struct cds_singly_linked_list_node*)node)->lock;
    ((struct cds_singly_linked_list_node*)new_node)->next 
        = ((struct cds_singly_linked_list_node*)node)->next;
    ((struct cds_singly_linked_list_node*)node)->next = new_node;
    omp_unset_lock(((struct cds_singly_linked_list_node*)node)->lock);
    return new_node;
}

void* cds_copy_and_create_reverse_linked_list(
    const size_t bytes_per_list_type, const size_t bytes_per_node_type, 
    void* const src_list,
    void* (*doubly_linked_list_null_front_callback)(void* const dest_list),
    void* (*push_list_front_callback)(
        void* const dest_list, void* const dest_node, 
        const bool toggle_safety_guards
    )
){
    if (!src_list) return src_list;
    struct cds_singly_linked_list* const dest_list 
        = cds_malloc_buffer(bytes_per_list_type);
    dest_list->hint = ((struct cds_singly_linked_list*)src_list)->hint;
    omp_init_lock_with_hint(&dest_list->lock, dest_list->hint);
    dest_list->front = (struct cds_singly_linked_list_node*)0;
    omp_set_lock(&((struct cds_singly_linked_list*)src_list)->lock);
    if (!((struct cds_singly_linked_list*)src_list)->front){
        omp_unset_lock(&((struct cds_singly_linked_list*)src_list)->lock);
        return doubly_linked_list_null_front_callback 
            ? doubly_linked_list_null_front_callback(dest_list) : dest_list;
    }
    for (
        const struct cds_singly_linked_list_node* src_node 
            = ((struct cds_singly_linked_list*)src_list)->front;
        src_node;
        src_node = src_node->next
    ) push_list_front_callback(
        dest_list, 
        cds_copy_and_create_linked_list_node(
            bytes_per_node_type, src_node
        ),
        false
    );
    omp_unset_lock(&((struct cds_singly_linked_list*)src_list)->lock);
    return dest_list;
}

void* cds_linked_list_pop_front(
    void* const list, 
    void (*doubly_linked_list_callback)(void* list, void* const node)
){
    if (!list) return (void*)0;
    omp_set_lock(&((struct cds_singly_linked_list*)list)->lock);
    if (!((struct cds_singly_linked_list*)list)->front){
        omp_unset_lock(&((struct cds_singly_linked_list*)list)->lock);
        return ((struct cds_singly_linked_list*)list)->front;
    }
    struct cds_singly_linked_list_node* const node 
        = ((struct cds_singly_linked_list*)list)->front;
    if (doubly_linked_list_callback) 
        doubly_linked_list_callback(list, node);
    ((struct cds_singly_linked_list*)list)->front = node->next;
    omp_unset_lock(&((struct cds_singly_linked_list*)list)->lock);
    node->lock = (omp_lock_t*)0;
    node->next = (struct cds_singly_linked_list_node*)0;
    return node;
}

void cds_linked_list_destroy_front(
    void* const list, 
    void (*doubly_linked_list_callback)
        (void* const list, const void* const node)
){
    if (!list) return;
    omp_set_lock(&((struct cds_singly_linked_list*)list)->lock);
    if (!((struct cds_singly_linked_list*)list)->front){
        omp_unset_lock(&((struct cds_singly_linked_list*)list)->lock);
        return;
    }
    struct cds_singly_linked_list_node* const node 
        = ((struct cds_singly_linked_list*)list)->front;
    ((struct cds_singly_linked_list*)list)->front = node->next;
    if (doubly_linked_list_callback) 
        doubly_linked_list_callback(list, node);
    omp_unset_lock(&((struct cds_singly_linked_list*)list)->lock);
    free(node);
    return;
}

void cds_erase_following_linked_list_nodes_core(void* const node){
    struct cds_singly_linked_list_node* node_iter 
        = ((struct cds_singly_linked_list_node*)node)->next;
    while (node_iter){
        struct cds_singly_linked_list_node* const node_iter_next 
            = node_iter->next;
        free(node_iter);
        node_iter = node_iter_next;
    }
}

void cds_swap_linked_list_nodes(
    void** const node_0, void** const node_1,
    void (*doubly_linked_list_callback)(void* const node_0, void* const node_1)
){
    if (!*node_0 || !*node_1) return;
    if (((struct cds_singly_linked_list_node*)*node_0)->lock) 
        omp_set_lock(((struct cds_singly_linked_list_node*)*node_0)->lock);
    const bool needs_lock_node_1 
        = ((struct cds_singly_linked_list_node*)*node_1)->lock 
            && ((struct cds_singly_linked_list_node*)*node_1)->lock 
                != ((struct cds_singly_linked_list_node*)*node_0)->lock;
    if (needs_lock_node_1) 
        omp_set_lock(((struct cds_singly_linked_list_node*)*node_1)->lock);
    struct cds_singly_linked_list_node* const temp_node = *node_0;
    *node_0 = *node_1;
    *node_1 = temp_node;
    omp_lock_t* const temp_lock 
        = ((struct cds_singly_linked_list_node*)*node_0)->lock;
    ((struct cds_singly_linked_list_node*)*node_0)->lock 
        = ((struct cds_singly_linked_list_node*)*node_1)->lock;
    ((struct cds_singly_linked_list_node*)*node_1)->lock = temp_lock;
    if (doubly_linked_list_callback) 
        doubly_linked_list_callback(*node_0, *node_1);
    struct cds_singly_linked_list_node* const temp_next 
        = ((struct cds_singly_linked_list_node*)*node_0)->next;
    ((struct cds_singly_linked_list_node*)*node_0)->next 
        = ((struct cds_singly_linked_list_node*)*node_1)->next;
    ((struct cds_singly_linked_list_node*)*node_1)->next = temp_next;
    if (needs_lock_node_1) 
        omp_unset_lock(((struct cds_singly_linked_list_node*)*node_1)->lock);
    if (((struct cds_singly_linked_list_node*)*node_0)->lock) 
        omp_unset_lock(((struct cds_singly_linked_list_node*)*node_0)->lock);
    return;
};

void* cds_linked_list_node_next(void** const node){
    if (!*node) return (void*)0;
    *node = ((struct cds_singly_linked_list_node*)*node)->next;
    return *node;
}