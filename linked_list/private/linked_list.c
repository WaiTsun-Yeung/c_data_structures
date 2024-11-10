#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <threads.h>
#include <stdlib.h>
#include <limits.h>

#include "alloc.h"
#include "mutex.h"
#include "status.h"

#include "doubly_linked_list_type.h"
#include "linked_list.h"
#include "singly_linked_list.h"
#include "doubly_linked_list.h"

void* cds_create_linked_list_node(
    const size_t bytes_per_node_type, const size_t bytes_per_element,
    const size_t data_align, enum cds_status* const return_state
){
    const size_t data_offset 
        = cds_compute_data_offset(bytes_per_node_type, data_align);
    struct cds_doubly_linked_list_node* const node 
        = malloc(data_offset + bytes_per_element);
    if (!node){
        if (return_state) *return_state = CDS_ALLOC_ERROR;
        return node;
    }
    node->bytes_per_element = bytes_per_element;
    node->data_offset = data_offset;
    node->list = (struct cds_doubly_linked_list*)0;
    node->next = (struct cds_doubly_linked_list_node*)0;
    if (return_state) *return_state = CDS_SUCCESS;
    return node;
}

void* cds_create_linked_list_with_mutex_type(
    const size_t bytes_per_list_type, const int mutex_type,
    enum cds_status* const return_state
){
    struct cds_doubly_linked_list* list = malloc(bytes_per_list_type);
    if (!list){
        if (return_state) *return_state = CDS_ALLOC_ERROR;
        return list;
    }
    list->mutex_type = mutex_type;
    if (mtx_init(&list->mutex, mutex_type) == thrd_error){
        if (return_state) *return_state = CDS_MUTEX_ERROR;
        return cds_destroy_buffer(&list);
    }
    list->front = (struct cds_doubly_linked_list_node*)0;
    if (return_state) *return_state = CDS_SUCCESS;
    return list;
}

void* cds_copy_and_create_linked_list_node(
    const void* restrict const src, enum cds_status* restrict const return_state
){
    if (!src){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (void*)0;
    }
    const size_t node_bytes_count 
        = ((const struct cds_doubly_linked_list_node* const)src)->data_offset
            + ((const struct cds_doubly_linked_list_node* const)src)
                ->bytes_per_element;
    void* node = malloc(node_bytes_count);
    if (!node){
        if (return_state) *return_state = CDS_ALLOC_ERROR;
        return node;
    }
    const errno_t memcpy_error 
        = memcpy_s(node, node_bytes_count, src, node_bytes_count);
    if (memcpy_error){
        cds_print_error_message(
            memcpy_error, __FILE__, __LINE__, __func__, "memcpy_s"
        );
        if (return_state) *return_state = CDS_COPY_ERROR;
        return cds_destroy_buffer(&node);
    }
    if (return_state) *return_state = CDS_SUCCESS;
    return node;
}

static void* cds_destroy_empty_linked_list(void** const list_holder){
    struct cds_doubly_linked_list* const list = *list_holder;
    mtx_destroy(&list->mutex);
    free(list);
    *list_holder = (void*)0;
    return *list_holder;
}

static void* cds_copy_and_create_linked_list_core(
    void* restrict const src_list,
    struct cds_doubly_linked_list *restrict *restrict const dest_list_holder,
    const enum cds_linked_list_type linked_list_type,
    const struct timespec *restrict const mutex_timeout
){
    const struct cds_doubly_linked_list_node* src_node 
        = ((struct cds_doubly_linked_list*)src_list)->front;
    struct cds_doubly_linked_list_node* dest_node 
        = cds_copy_and_create_linked_list_node(src_node);
    if (!dest_node) return cds_destroy_empty_linked_list(dest_list_holder);
    struct cds_doubly_linked_list* const dest_list = *dest_list_holder;
    dest_list->front = dest_node;
    for (
        src_node = src_node->next;
        src_node; 
        src_node = src_node->next, dest_node = dest_node->next
    ){
        dest_node->list = dest_list; 
        struct cds_doubly_linked_list_node* const dest_node_next 
            = dest_node->next;
        dest_node->next = cds_copy_and_create_linked_list_node(src_node);
        if (!dest_node->next)
            return cds_destroy_linked_list_with_timeout(
                dest_list_holder, false, mutex_timeout
            );
        if (linked_list_type == CDS_DOUBLY_LINKED_LIST)
            dest_node_next->prev = dest_node;
    }
    dest_node->list = dest_list;
    if (linked_list_type == CDS_DOUBLY_LINKED_LIST)
        dest_list->back = dest_node;
    return dest_list;
}

void* cds_copy_and_create_linked_list_with_timeout(
    const size_t bytes_per_list_type, 
    void* restrict const src_list,
    const enum cds_linked_list_type linked_list_type,
    const struct timespec *restrict const mutex_timeout
){
    if (!src_list) return src_list;
    struct cds_doubly_linked_list* dest_list 
        = malloc(bytes_per_list_type);
    if (!dest_list) return dest_list;
    dest_list->mutex_type 
        = ((struct cds_doubly_linked_list*)src_list)->mutex_type;
    if(mtx_init(&dest_list->mutex, dest_list->mutex_type) == thrd_error)
        return cds_destroy_buffer((void**)&dest_list);
    if(
        cds_mutex_lock(
            &((struct cds_doubly_linked_list*)src_list)->mutex, mutex_timeout, 
            ((struct cds_doubly_linked_list*)src_list)->mutex_type
        ) != thrd_success
    ) return cds_destroy_buffer((void**)&dest_list);
    if (!((struct cds_doubly_linked_list*)src_list)->front){
        (void)mtx_unlock(&((struct cds_doubly_linked_list*)src_list)->mutex);
        dest_list->front = ((struct cds_doubly_linked_list*)src_list)->front;
        if (linked_list_type == CDS_DOUBLY_LINKED_LIST)
            dest_list->back = ((struct cds_doubly_linked_list*)src_list)->back;
        return dest_list;
    }
    (void)cds_copy_and_create_linked_list_core(
        src_list, &dest_list, linked_list_type, mutex_timeout
    );
    (void)mtx_unlock(&((struct cds_doubly_linked_list*)src_list)->mutex);
    return dest_list;
}

static void* cds_set_linked_list_node_after_realloc(
    void* const node, const size_t data_offset,
    const size_t bytes_per_element
){
    ((struct cds_doubly_linked_list_node*)node)->bytes_per_element 
        = bytes_per_element;
    ((struct cds_doubly_linked_list_node*)node)->data_offset = data_offset;
    return node;
}

void* cds_change_linked_list_node_data_type(
    const size_t bytes_per_node_type, 
    void *restrict * restrict const node_holder, 
    const size_t bytes_per_element, const size_t data_align,
    enum cds_status *restrict const return_state
){
    const size_t data_offset 
        = cds_compute_data_offset(bytes_per_node_type, data_align);
    struct cds_doubly_linked_list_node* const node = *node_holder;
    if (!node){
        if (return_state) *return_state = CDS_NULL_ARG;
        return node;
    }
    if (
            node->bytes_per_element == bytes_per_element
                && node->data_offset == data_offset
    ){
        if (return_state) *return_state = CDS_SUCCESS;
        return (void*)0;
    }
    void* realloced_node = realloc(node, data_offset + bytes_per_element);
    if (!realloced_node){
        if (return_state) *return_state = CDS_ALLOC_ERROR;
        return realloced_node;
    } 
    else *node_holder = realloced_node;
    if (return_state) *return_state = CDS_SUCCESS;
    return cds_set_linked_list_node_after_realloc(
        *node_holder, data_offset, bytes_per_element
    );
}

void* cds_copy_linked_list_node(
    void **restrict const dest_holder, const void* const src,
    enum cds_status *restrict const return_state
){
    if (!*dest_holder || !src){
        if (return_state) *return_state = CDS_NULL_ARG;
        return (void*)0;
    }
    if (*dest_holder == src){
        if (return_state) *return_state = CDS_INVALID_ARG;
        return *dest_holder;
    }
    struct cds_doubly_linked_list_node* const dest = *dest_holder;
    if (
        dest->bytes_per_element 
                != ((const struct cds_doubly_linked_list_node* const)src)
                    ->bytes_per_element
            || dest->data_offset
                != ((const struct cds_doubly_linked_list_node* const)src)
                    ->data_offset
    ){ 
        struct cds_doubly_linked_list_node* const realloced_dest = realloc(
            dest, 
            ((const struct cds_doubly_linked_list_node* const)src)->data_offset 
                + ((const struct cds_doubly_linked_list_node* const)src)
                    ->bytes_per_element
        );
        if (!realloced_dest){
            if (return_state) *return_state = CDS_ALLOC_ERROR;
            return realloced_dest;
        } 
        *dest_holder = cds_set_linked_list_node_after_realloc(
            realloced_dest, 
            ((const struct cds_doubly_linked_list_node* const)src)->data_offset, 
            ((const struct cds_doubly_linked_list_node* const)src)
                ->bytes_per_element
        );
    }
    struct cds_doubly_linked_list_node* const new_dest 
        = (struct cds_doubly_linked_list_node*)*dest_holder;
    const errno_t memcpy_error = memcpy_s(
        cds_data(new_dest), 
        new_dest->bytes_per_element,
        cds_data(src), 
        ((const struct cds_doubly_linked_list_node* const)src)
            ->bytes_per_element
    );
    if (memcpy_error){
        cds_print_error_message(
            memcpy_error, __FILE__, __LINE__, __func__, "memcpy_s"
        );
        if (return_state) *return_state = CDS_COPY_ERROR;
        return cds_destroy_buffer(dest_holder);
    }
    if (return_state) *return_state = CDS_SUCCESS;
    return new_dest;
}

void* cds_empty_linked_list_with_timeout(
    void* restrict const list, 
    const enum cds_linked_list_type linked_list_type,
    const bool toggle_guards_and_cleanups, 
    const struct timespec *restrict const mutex_timeout
){
    if (toggle_guards_and_cleanups){
        if (!list) return list;
        if (
            cds_mutex_lock(
                &((struct cds_doubly_linked_list*)list)->mutex, 
                mutex_timeout, 
                ((struct cds_doubly_linked_list*)list)->mutex_type
            ) != thrd_success
        ) return (void*)0;
        if (!((struct cds_doubly_linked_list*)list)->front){
            (void)mtx_unlock(&((struct cds_doubly_linked_list*)list)->mutex);
            return list;
        }
    }
    struct cds_doubly_linked_list_node* node 
        = ((struct cds_doubly_linked_list*)list)->front;
    for (
        struct cds_doubly_linked_list_node* next_node = node->next; 
        next_node; 
        next_node = node->next
    ){
        free(node);
        node = next_node;
    }
    if (toggle_guards_and_cleanups){
        ((struct cds_doubly_linked_list*)list)->front 
            = (struct cds_doubly_linked_list_node*)0;
        if (linked_list_type == CDS_DOUBLY_LINKED_LIST) 
            ((struct cds_doubly_linked_list*)list)->back 
                = (struct cds_doubly_linked_list_node*)0;
        (void)mtx_unlock(&((struct cds_doubly_linked_list*)list)->mutex);
    }
    free(node);
    return list;
}

void* cds_destroy_linked_list_with_timeout(
    void *restrict *restrict const list_holder, 
    const bool uses_mutex_lock,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    struct cds_doubly_linked_list* const list = *list_holder;
    if (!list){
        if (return_state) *return_state = CDS_SUCCESS;
        return list;
    }
    if (
        uses_mutex_lock && cds_mutex_lock(
            &list->mutex, mutex_timeout, list->mutex_type, return_state
        )
    ) return list;
    enum cds_status local_return_state;
    if (list->front){ 
        (void)cds_empty_linked_list_with_timeout(
            list, CDS_SINGLY_LINKED_LIST, false, mutex_timeout, 
            &local_return_state
        );
        if (local_return_state){
            if (uses_mutex_lock) (void)mtx_unlock(&list->mutex);
            if (return_state) *return_state = local_return_state;
            return list;
        }
    }
    if (uses_mutex_lock) (void)mtx_unlock(&list->mutex);
    if (return_state) *return_state = CDS_SUCCESS;
    return cds_destroy_empty_linked_list(list_holder);
}

void* cds_push_next_linked_list_node_core(
    void *restrict const node, void *restrict const new_node,
    const enum cds_linked_list_type linked_list_type
){
    struct cds_doubly_linked_list* const list 
        = ((struct cds_doubly_linked_list_node*)node)->list;
    if (linked_list_type == CDS_DOUBLY_LINKED_LIST){ 
        struct cds_doubly_linked_list_node* const this_next 
            = ((struct cds_doubly_linked_list_node*)node)->next;
        if (this_next) this_next->prev = new_node; else list->back = new_node;
        ((struct cds_doubly_linked_list_node*)new_node)->prev 
            = (struct cds_doubly_linked_list_node*)node;
    }
    ((struct cds_doubly_linked_list_node*)new_node)->list = list;
    ((struct cds_doubly_linked_list_node*)new_node)->next 
        = ((struct cds_doubly_linked_list_node*)node)->next;
    ((struct cds_doubly_linked_list_node*)node)->next = new_node;
    return new_node;
}

void* cds_push_next_linked_list_node_with_timeout(
    void *restrict const node,
    void *restrict const new_node,
    const enum cds_linked_list_type linked_list_type,
    const struct timespec *restrict const mutex_timeout
){
    if (!node || !new_node) return (void*)0;
    struct cds_doubly_linked_list* const list 
        = ((struct cds_doubly_linked_list_node*)node)->list;
    if (
        !list 
            || cds_mutex_lock(&list->mutex, mutex_timeout, list->mutex_type)
                != thrd_success
    ) return (void*)0;
    (void)cds_push_next_linked_list_node_core(
        node, new_node, linked_list_type
    );
    (void)mtx_unlock(&list->mutex);
    return new_node;
}

void* cds_copy_and_create_reverse_linked_list_with_timeout(
    const size_t bytes_per_list_type, 
    void *restrict const src_list,
    const enum cds_linked_list_type linked_list_type,
    const struct timespec *restrict const mutex_timeout
){
    if (!src_list) return src_list;
    struct cds_doubly_linked_list* const dest_list 
        = malloc(bytes_per_list_type);
    if (!dest_list) return dest_list;
    dest_list->mutex_type 
        = ((struct cds_doubly_linked_list*)src_list)->mutex_type;
    if(mtx_init(&dest_list->mutex, dest_list->mutex_type) == thrd_error)
        return cds_destroy_buffer((void**)&dest_list);
    dest_list->front = (struct cds_doubly_linked_list_node*)0;
    if (
        cds_mutex_lock(
            &((struct cds_doubly_linked_list*)src_list)->mutex, mutex_timeout, 
            ((struct cds_doubly_linked_list*)src_list)->mutex_type
        ) != thrd_success
    ) return cds_destroy_buffer((void**)&dest_list);
    if (!((struct cds_doubly_linked_list*)src_list)->front){
        (void)mtx_unlock(&((struct cds_doubly_linked_list*)src_list)->mutex);
        if (linked_list_type == CDS_DOUBLY_LINKED_LIST) 
            dest_list->back = ((struct cds_doubly_linked_list*)src_list)->back;
        return dest_list;
    }
    for (
        const struct cds_doubly_linked_list_node* src_node 
            = ((struct cds_doubly_linked_list*)src_list)->front;
        src_node;
        src_node = src_node->next
    ){
        struct cds_doubly_linked_list_node* dest_node 
            = cds_copy_and_create_linked_list_node(src_node);
        if (!dest_node){
            (void)mtx_unlock(
                &((struct cds_doubly_linked_list*)src_list)->mutex
            );
            return cds_destroy_linked_list_with_timeout(
                (void**)&dest_list, false, mutex_timeout
            );
        }
        if (linked_list_type == CDS_DOUBLY_LINKED_LIST)
            (void)cds_doubly_linked_list_push_front_with_toggle_with_timeout(
                dest_list, dest_node, false, mutex_timeout
            );
        else (void)cds_singly_linked_list_push_front_core(
            (struct cds_singly_linked_list*)dest_list, 
            (struct cds_singly_linked_list_node*)dest_node
        );
    }
    (void)mtx_unlock(&((struct cds_doubly_linked_list*)src_list)->mutex);
    return dest_list;
}

void* cds_linked_list_pop_front_with_timeout(
    void *restrict const list, 
    const enum cds_linked_list_type linked_list_type,
    const struct timespec *restrict const mutex_timeout
){
    if (
        !list
            || cds_mutex_lock(
                &((struct cds_doubly_linked_list*)list)->mutex, 
                mutex_timeout, 
                ((struct cds_doubly_linked_list*)list)->mutex_type
            ) != thrd_success
    ) return (void*)0;
    if (!((struct cds_doubly_linked_list*)list)->front){
        (void)mtx_unlock(&((struct cds_doubly_linked_list*)list)->mutex);
        return ((struct cds_doubly_linked_list*)list)->front;
    }
    struct cds_doubly_linked_list_node* const node 
        = ((struct cds_doubly_linked_list*)list)->front;
    if (linked_list_type == CDS_DOUBLY_LINKED_LIST){
        struct cds_doubly_linked_list_node* const next_node = node->next;
        if (next_node) next_node->prev = (struct cds_doubly_linked_list_node*)0;
        else ((struct cds_doubly_linked_list*)list)->back 
            = (struct cds_doubly_linked_list_node*)0;
    }
    ((struct cds_doubly_linked_list*)list)->front = node->next;
    (void)mtx_unlock(&((struct cds_doubly_linked_list*)list)->mutex);
    node->list = (struct cds_doubly_linked_list*)0;
    node->next = (struct cds_doubly_linked_list_node*)0;
    return node;
}

void* cds_linked_list_destroy_front_core(
    void* const list, const enum cds_linked_list_type linked_list_type
){
    struct cds_doubly_linked_list_node* const node 
        = ((struct cds_doubly_linked_list*)list)->front;
    ((struct cds_doubly_linked_list*)list)->front = node->next;
    if (linked_list_type == CDS_DOUBLY_LINKED_LIST){
        if (((struct cds_doubly_linked_list*)list)->back == node)
            ((struct cds_doubly_linked_list*)list)->back 
                = (struct cds_doubly_linked_list_node*)0;
        struct cds_doubly_linked_list_node* const front_node
            = ((struct cds_doubly_linked_list*)list)->front;
        if (front_node) front_node->prev 
            = (struct cds_doubly_linked_list_node*)0;
    }
    free(node);
    return list;
}

enum cds_status cds_linked_list_destroy_front_with_timeout(
    void *restrict const list, 
    const enum cds_linked_list_type linked_list_type,
    const struct timespec *restrict const mutex_timeout,
    enum cds_status *restrict const return_state
){
    if (!list){
        if (return_state) *return_state = CDS_NULL_ARG;
        return CDS_NULL_ARG;
    }
    const enum cds_status mutex_lock_error = cds_mutex_lock(
        &((struct cds_doubly_linked_list*)list)->mutex, mutex_timeout, 
        ((struct cds_doubly_linked_list*)list)->mutex_type, return_state
    );
    if (mutex_lock_error) return mutex_lock_error;
    if (!((struct cds_doubly_linked_list*)list)->front){
        (void)mtx_unlock(&((struct cds_doubly_linked_list*)list)->mutex);
        if (return_state) *return_state = CDS_SUCCESS;
        return CDS_SUCCESS;
    }
    (void)cds_linked_list_destroy_front_core(list, linked_list_type);
    (void)mtx_unlock(&((struct cds_doubly_linked_list*)list)->mutex);
    if (return_state) *return_state = CDS_SUCCESS;
    return CDS_SUCCESS;
}

void cds_erase_following_linked_list_nodes_core(void* const node){
    struct cds_doubly_linked_list_node* node_iter 
        = ((struct cds_doubly_linked_list_node*)node)->next;
    for (size_t i = 0; i < SIZE_MAX, node_iter; ++i){
        struct cds_doubly_linked_list_node* const node_iter_next 
            = node_iter->next;
        free(node_iter);
        node_iter = node_iter_next;
    }
}

void* cds_linked_list_node_next(void** const node_holder){
    struct cds_doubly_linked_list_node* const node = *node_holder;
    *node_holder = node->next;
    return *node_holder;
}