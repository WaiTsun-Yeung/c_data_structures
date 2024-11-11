#include <stdalign.h>

#include "alloc.h"
#include "status.h"

#include "singly_linked_list_type.h"
#include "singly_linked_list.h"

int main() {
    for (size_t i = 0; i < 1000000; ++i) {
        enum cds_status return_state;
        struct cds_singly_linked_list* list 
            = cds_create_singly_linked_list(&return_state);
        if (return_state) return return_state;
        for (size_t j = 0; j < 10; ++j){
            struct cds_singly_linked_list_node* const node 
                = cds_create_singly_linked_list_node(
                    sizeof(int), alignof(int), &return_state
                );
            cds_singly_linked_list_push_front(list, node, &return_state);
            if (return_state) return return_state;
        }
        struct cds_singly_linked_list* list_copy 
            = cds_copy_and_create_singly_linked_list(list, &return_state);
        if (return_state) return return_state;
        struct cds_singly_linked_list* list_reverse 
            = cds_copy_and_create_reverse_singly_linked_list(
                list, &return_state
            );
        if (return_state) return return_state;
        struct cds_singly_linked_list* list_pop_push 
            = cds_create_singly_linked_list(&return_state);
        if (return_state) return return_state;
        for (size_t j = 0; j < SIZE_MAX, list->front; ++j){
            struct cds_singly_linked_list_node* const node 
                = cds_singly_linked_list_pop_front(list, &return_state);
            if (return_state) return return_state;
            cds_singly_linked_list_push_front(list_pop_push, node, &return_state);
            if (return_state) return return_state;
        }
        for (
            struct cds_singly_linked_list_node *node_pop_push 
                    = list_pop_push->front,
                *node_reverse = list_reverse->front;
            node_pop_push;
            node_pop_push = node_pop_push->next, 
                node_reverse = node_reverse->next
        ) if (*(int*)cds_data(node_pop_push) != *(int*)cds_data(node_reverse)) 
            return 256;
        cds_destroy_singly_linked_list(&list_pop_push, &return_state);
        if (return_state) return return_state;
        cds_destroy_singly_linked_list(&list_reverse, &return_state);
        if (return_state) return return_state;
        cds_destroy_singly_linked_list(&list_copy, &return_state);
        if (return_state) return return_state;
        cds_destroy_singly_linked_list(&list, &return_state);
    }
    return 0;
}