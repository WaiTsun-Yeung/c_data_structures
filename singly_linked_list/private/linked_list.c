#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <threads.h>
#include <setjmp.h>
#include <stdlib.h>

#include "alloc.h"
#include "mutex.h"
#include "status.h"

#include "singly_linked_list_type.h"
#include "linked_list.h"

void* cds_create_linked_list_node(
    const size_t bytes_per_node_type, const size_t bytes_per_element,
    const size_t data_align
){
    const ptrdiff_t data_offset 
        = cds_compute_data_offset(bytes_per_node_type, data_align);
    struct cds_singly_linked_list_node* const node 
        = malloc(data_offset + bytes_per_element);
    if (!node) return node;
    node->bytes_per_element = bytes_per_element;
    node->data_offset = data_offset;
    node->list = (struct cds_singly_linked_list*)0;
    node->next = (struct cds_singly_linked_list_node*)0;
    return node;
}

void* cds_create_linked_list_with_mutex_type(
    const size_t bytes_per_list_type, const int mutex_type
){
    struct cds_singly_linked_list* const list = malloc(bytes_per_list_type);
    if (!list) return list;
    list->mutex_type = mutex_type;
    cds_mutex_init(&list->mutex, mutex_type);
    list->front = (struct cds_singly_linked_list_node*)0;
    return list;
}

void* cds_copy_and_create_linked_list_node(
    const void* const src
){
    if (!src) return (void*)0;
    const size_t node_bytes_count 
        = ((const struct cds_singly_linked_list_node* const)src)->data_offset
            + ((const struct cds_singly_linked_list_node* const)src)
                ->bytes_per_element;
    void* node = malloc(node_bytes_count);
    if (!node) return node;
    const errno_t memcpy_error 
        = memcpy_s(node, node_bytes_count, src, node_bytes_count);
    if (memcpy_error){
        cds_print_error_message(
            memcpy_error, __FILE__, __LINE__, __func__, "memcpy_s"
        );
        return cds_destroy_buffer(&node);
    }
    return node;
}

static void* cds_destroy_empty_linked_list(void** const list){
    mtx_destroy(&((struct cds_singly_linked_list*)*list)->mutex);
    free(*list);
    *list = (void*)0;
    return *list;
}

static void* cds_copy_and_create_linked_list_core(
    void* restrict const src_list,
    struct cds_singly_linked_list *restrict *restrict const dest_list,
    void (*doubly_linked_list_copy_node_callback)(void* const dest_node),
    void (*doubly_linked_list_closing_callback)
        (void* const dest_list, void* const dest_node),
    const struct timespec *restrict const mutex_timeout
){
    const struct cds_singly_linked_list_node* src_node 
        = ((struct cds_singly_linked_list*)src_list)->front;
    struct cds_singly_linked_list_node* dest_node 
        = cds_copy_and_create_linked_list_node(src_node);
    if (!dest_node) return cds_destroy_empty_linked_list(dest_list);
    (*dest_list)->front = dest_node;
    for (
        src_node = src_node->next;
        src_node; 
        src_node = src_node->next, dest_node = dest_node->next
    ){
        dest_node->list = *dest_list; 
        dest_node->next = cds_copy_and_create_linked_list_node(src_node);
        if (!dest_node->next)
            return cds_destroy_linked_list_with_timeout(
                dest_list, false, mutex_timeout
            );
        if (doubly_linked_list_copy_node_callback)
            doubly_linked_list_copy_node_callback(dest_node);
    }
    dest_node->list = *dest_list;
    if (doubly_linked_list_closing_callback)
        doubly_linked_list_closing_callback(*dest_list, dest_node);
    return *dest_list;
}

void* cds_copy_and_create_linked_list_with_timeout(
    const size_t bytes_per_list_type, 
    void* restrict const src_list,
    void* (*doubly_linked_list_null_front_callback)(void* const dest_list),
    void (*doubly_linked_list_copy_node_callback)(void* const dest_node),
    void (*doubly_linked_list_closing_callback)
        (void* const dest_list, void* const dest_node), 
    const struct timespec *restrict const mutex_timeout
){
    if (!src_list) return src_list;
    struct cds_singly_linked_list* dest_list 
        = malloc(bytes_per_list_type);
    if (!dest_list) return dest_list;
    dest_list->mutex_type 
        = ((struct cds_singly_linked_list*)src_list)->mutex_type;
    cds_mutex_init(&dest_list->mutex, dest_list->mutex_type);
    if(
        cds_mutex_lock(
            &((struct cds_singly_linked_list*)src_list)->mutex, mutex_timeout, 
            ((struct cds_singly_linked_list*)src_list)->mutex_type
        ) != thrd_success
    ) return cds_destroy_buffer((void**)&dest_list);
    if (!((struct cds_singly_linked_list*)src_list)->front){
        cds_mutex_unlock(&((struct cds_singly_linked_list*)src_list)->mutex);
        dest_list->front = ((struct cds_singly_linked_list*)src_list)->front;
        return doubly_linked_list_null_front_callback
            ? doubly_linked_list_null_front_callback(dest_list)
            : dest_list;
    }
    (void)cds_copy_and_create_linked_list_core(
        src_list, &dest_list, doubly_linked_list_copy_node_callback,
        doubly_linked_list_closing_callback, mutex_timeout
    );
    cds_mutex_unlock(&((struct cds_singly_linked_list*)src_list)->mutex);
    return dest_list;
}

static void* cds_set_linked_list_node_after_realloc(
    void** const node, const ptrdiff_t data_offset,
    const size_t bytes_per_element
){
    ((struct cds_singly_linked_list_node*)*node)->bytes_per_element 
        = bytes_per_element;
    ((struct cds_singly_linked_list_node*)*node)->data_offset = data_offset;
    return *node;
}

void* cds_change_linked_list_node_data_type(
    const size_t bytes_per_node_type, void** const node, 
    const size_t bytes_per_element, const size_t data_align
){
    const ptrdiff_t data_offset 
        = cds_compute_data_offset(bytes_per_node_type, data_align);
    if (
        !*node || (
            ((struct cds_singly_linked_list_node*)*node)->bytes_per_element 
                    == bytes_per_element
                && ((struct cds_singly_linked_list_node*)*node)->data_offset
                    == data_offset
        )
    ) return node;
    cds_realloc_buffer(node, data_offset + bytes_per_element);
    return cds_set_linked_list_node_after_realloc(
        node, data_offset, bytes_per_element
    );
}

void* cds_copy_linked_list_node(
    void **restrict const dest, const void* const src
){
    if (!*dest || !src) (void*)0;
    if (*dest == src) return *dest;
    if (
        ((struct cds_singly_linked_list_node*)*dest)->bytes_per_element 
                != ((const struct cds_singly_linked_list_node* const)src)
                    ->bytes_per_element
            || ((struct cds_singly_linked_list_node*)*dest)->data_offset
                != ((const struct cds_singly_linked_list_node* const)src)
                    ->data_offset
    ){ 
        cds_realloc_buffer(
            dest, 
            ((const struct cds_singly_linked_list_node* const)src)->data_offset 
                + ((const struct cds_singly_linked_list_node* const)src)
                    ->bytes_per_element
        );
        cds_set_linked_list_node_after_realloc(
            dest, 
            ((const struct cds_singly_linked_list_node* const)src)->data_offset, 
            ((const struct cds_singly_linked_list_node* const)src)
                ->bytes_per_element
        );
    }
    const errno_t memcpy_error = memcpy_s(
        cds_data(*dest), 
        ((const struct cds_singly_linked_list_node* const)*dest)
            ->bytes_per_element,
        cds_data(src), 
        ((const struct cds_singly_linked_list_node* const)src)
            ->bytes_per_element
    );
    if (memcpy_error){
        cds_print_error_message(
            memcpy_error, __FILE__, __LINE__, __func__, "memcpy_s"
        );
        return cds_destroy_buffer(dest);
    }
    return *dest;
}

void* cds_empty_linked_list_with_timeout(
    void* restrict const list, 
    void (*doubly_linked_list_callback)(void* const list),
    const bool toggle_guards_and_cleanups, 
    const struct timespec *restrict const mutex_timeout
){
    if (toggle_guards_and_cleanups){
        if (!list) return list;
        if (
            cds_mutex_lock(
                &((struct cds_singly_linked_list*)list)->mutex, 
                mutex_timeout, 
                ((struct cds_singly_linked_list*)list)->mutex_type
            ) != thrd_success
        ) return (void*)0;
        if (!((struct cds_singly_linked_list*)list)->front){
            cds_mutex_unlock(&((struct cds_singly_linked_list*)list)->mutex);
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
        cds_mutex_unlock(&((struct cds_singly_linked_list*)list)->mutex);
    }
    free(node);
    return list;
}

void* cds_destroy_linked_list_with_timeout(
    void *restrict *restrict const list, 
    const bool uses_mutex_lock,
    const struct timespec *restrict const mutex_timeout
){
    if (!list) return list;
    if (
        uses_mutex_lock
            && cds_mutex_lock(
                &((struct cds_singly_linked_list*)*list)->mutex, 
                mutex_timeout, 
                ((struct cds_singly_linked_list*)*list)->mutex_type
            ) != thrd_success
    ) return (void*)0;
    if (((struct cds_singly_linked_list*)*list)->front) 
        cds_empty_linked_list(*list, (void*)0, false);
    if (uses_mutex_lock)
        cds_mutex_unlock(&((struct cds_singly_linked_list*)*list)->mutex);
    return cds_destroy_empty_linked_list(list);
}

void* cds_push_next_linked_list_node_core(
    void *restrict const node, void *restrict const new_node,
    void (*doubly_linked_list_callback)(void* const node, void* const new_node)
){
    if (doubly_linked_list_callback) 
        doubly_linked_list_callback(node, new_node);
    ((struct cds_singly_linked_list_node*)new_node)->list 
        = ((struct cds_singly_linked_list_node*)node)->list;
    ((struct cds_singly_linked_list_node*)new_node)->next 
        = ((struct cds_singly_linked_list_node*)node)->next;
    ((struct cds_singly_linked_list_node*)node)->next = new_node;
    return new_node;
}

void* cds_push_next_linked_list_node_with_timeout(
    void *restrict const node,
    void *restrict const new_node,
    void (*doubly_linked_list_callback)(void* const node, void* const new_node),
    const struct timespec *restrict const mutex_timeout
){
    if (!node || !new_node) return (void*)0;
    struct cds_singly_linked_list* const list 
        = ((struct cds_singly_linked_list_node*)node)->list;
    if (!list || cds_mutex_lock(&list->mutex, mutex_timeout, list->mutex_type)
        != thrd_success
    ) return (void*)0;
    cds_push_next_linked_list_node_core(
        node, new_node, doubly_linked_list_callback
    );
    cds_mutex_unlock(&list->mutex);
    return new_node;
}

void* cds_copy_and_create_reverse_linked_list_with_timeout(
    const size_t bytes_per_list_type, 
    void *restrict const src_list,
    void* (*doubly_linked_list_null_front_callback)(void* const dest_list),
    void* (*push_list_front_callback)(
        void* const dest_list, void* const dest_node, 
        const bool toggle_safety_guards
    ),
    const struct timespec *restrict const mutex_timeout
){
    if (!src_list) return src_list;
    struct cds_singly_linked_list* const dest_list 
        = malloc(bytes_per_list_type);
    if (!dest_list) return dest_list;
    dest_list->mutex_type 
        = ((struct cds_singly_linked_list*)src_list)->mutex_type;
    cds_mutex_init(&dest_list->mutex, dest_list->mutex_type);
    dest_list->front = (struct cds_singly_linked_list_node*)0;
    if (
        cds_mutex_lock(
            &((struct cds_singly_linked_list*)src_list)->mutex, mutex_timeout, 
            ((struct cds_singly_linked_list*)src_list)->mutex_type
        ) != thrd_success
    ) return cds_destroy_buffer((void**)&dest_list);
    if (!((struct cds_singly_linked_list*)src_list)->front){
        cds_mutex_unlock(&((struct cds_singly_linked_list*)src_list)->mutex);
        return doubly_linked_list_null_front_callback 
            ? doubly_linked_list_null_front_callback(dest_list) : dest_list;
    }
    for (
        const struct cds_singly_linked_list_node* src_node 
            = ((struct cds_singly_linked_list*)src_list)->front;
        src_node;
        src_node = src_node->next
    ){
        struct cds_singly_linked_list_node* dest_node 
            = cds_copy_and_create_linked_list_node(src_node);
        if (!dest_node){
            cds_mutex_unlock(
                &((struct cds_singly_linked_list*)src_list)->mutex
            );
            return cds_destroy_linked_list_with_timeout(
                (void**)&dest_list, false, mutex_timeout
            );
        }
        push_list_front_callback(dest_list, dest_node, false);
    }
    cds_mutex_unlock(&((struct cds_singly_linked_list*)src_list)->mutex);
    return dest_list;
}

void* cds_linked_list_pop_front_with_timeout(
    void *restrict const list, 
    void (*doubly_linked_list_callback)(void* list, void* const node),
    const struct timespec *restrict const mutex_timeout
){
    if (
        !list
            || cds_mutex_lock(
                &((struct cds_singly_linked_list*)list)->mutex, 
                mutex_timeout, 
                ((struct cds_singly_linked_list*)list)->mutex_type
            ) != thrd_success
    ) return (void*)0;
    if (!((struct cds_singly_linked_list*)list)->front){
        cds_mutex_unlock(&((struct cds_singly_linked_list*)list)->mutex);
        return ((struct cds_singly_linked_list*)list)->front;
    }
    struct cds_singly_linked_list_node* const node 
        = ((struct cds_singly_linked_list*)list)->front;
    if (doubly_linked_list_callback) 
        doubly_linked_list_callback(list, node);
    ((struct cds_singly_linked_list*)list)->front = node->next;
    cds_mutex_unlock(&((struct cds_singly_linked_list*)list)->mutex);
    node->list = (struct cds_singly_linked_list*)0;
    node->next = (struct cds_singly_linked_list_node*)0;
    return node;
}

enum cds_status cds_linked_list_destroy_front_with_timeout(
    void *restrict const list, 
    void (*doubly_linked_list_callback)
        (void* const list, const void* const node),
    const struct timespec *restrict const mutex_timeout
){
    if (!list) return CDS_NULL_ARG;
    switch(
        cds_mutex_lock(
            &((struct cds_singly_linked_list*)list)->mutex, 
            mutex_timeout, 
            ((struct cds_singly_linked_list*)list)->mutex_type
        )
    ){
        case thrd_timedout: return CDS_MUTEX_TIMEOUT;
        case thrd_error: return CDS_MUTEX_ERROR;
        default: break;
    }
    if (!((struct cds_singly_linked_list*)list)->front){
        cds_mutex_unlock(&((struct cds_singly_linked_list*)list)->mutex);
        return CDS_SUCCESS;
    }
    struct cds_singly_linked_list_node* const node 
        = ((struct cds_singly_linked_list*)list)->front;
    ((struct cds_singly_linked_list*)list)->front = node->next;
    if (doubly_linked_list_callback) 
        doubly_linked_list_callback(list, node);
    cds_mutex_unlock(&((struct cds_singly_linked_list*)list)->mutex);
    free(node);
    return CDS_SUCCESS;
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

void* cds_linked_list_node_next(void** const node){
    *node = ((struct cds_singly_linked_list_node*)*node)->next;
    return *node;
}