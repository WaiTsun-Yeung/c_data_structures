#include <stdalign.h>

#include "alloc.h"
#include "status.h"

#include "doubly_linked_list_type.h"
#include "doubly_linked_list.h"

int main() {
    for (size_t i = 0; i < 1000000; ++i){
        enum cds_status return_state;
        struct cds_doubly_linked_list* list_0 
            = cds_create_doubly_linked_list(&return_state);
        if (return_state) return return_state;
        struct cds_doubly_linked_list* list_1
            = cds_create_doubly_linked_list(&return_state);
        if (return_state) return return_state;
        for (size_t j = 0; j < 10; ++j){
            struct cds_doubly_linked_list_node* const node_0 
                = cds_create_doubly_linked_list_node(
                    sizeof(int), alignof(int), &return_state
                );
            if (return_state) return return_state;
            cds_doubly_linked_list_push_front(list_0, node_0, &return_state);
            if (return_state) return return_state;
            struct cds_doubly_linked_list_node* const node_1 
                = cds_create_doubly_linked_list_node(
                    sizeof(int), alignof(int), &return_state
                );
            if (return_state) return return_state;
            cds_doubly_linked_list_push_front(list_1, node_1, &return_state);
            if (return_state) return return_state;
        }
        struct cds_doubly_linked_list* list_0_copy 
            = cds_copy_and_create_doubly_linked_list(list_0, &return_state);
        if (return_state) return return_state;
        struct cds_doubly_linked_list* list_1_copy
            = cds_copy_and_create_doubly_linked_list(list_1, &return_state);
        if (return_state) return return_state;
        for (
            struct cds_doubly_linked_list_node *node_0 
                    = cds_doubly_linked_list_begin(list_0),
                *node_1 = cds_doubly_linked_list_begin(list_1);
            node_0 != cds_doubly_linked_list_end();
            cds_doubly_linked_list_node_next(&node_0), 
                cds_doubly_linked_list_node_next(&node_1)
        ){
            cds_swap_doubly_linked_list_nodes(&node_0, &node_1, &return_state);
            if (return_state) return return_state;
        }
        for (
            struct cds_doubly_linked_list_node *node_0 
                    = cds_doubly_linked_list_begin(list_0),
                *node_1 = cds_doubly_linked_list_begin(list_1),
                *node_0_copy = cds_doubly_linked_list_begin(list_0_copy),
                *node_1_copy = cds_doubly_linked_list_begin(list_1_copy);
            node_0 != cds_doubly_linked_list_end();
            cds_doubly_linked_list_node_next(&node_0), 
                cds_doubly_linked_list_node_next(&node_1),
                cds_doubly_linked_list_node_next(&node_0_copy), 
                cds_doubly_linked_list_node_next(&node_1_copy)
        ) if (*(int*)cds_data(node_0) != *(int*)cds_data(node_1_copy) ||
            *(int*)cds_data(node_1) != *(int*)cds_data(node_0_copy)) {
            cds_destroy_doubly_linked_list(&list_1_copy, &return_state);
            if (return_state) return return_state;
            cds_destroy_doubly_linked_list(&list_0_copy, &return_state);
            if (return_state) return return_state;
            cds_destroy_doubly_linked_list(&list_1, &return_state);
            if (return_state) return return_state;
            cds_destroy_doubly_linked_list(&list_0, &return_state);
            if (return_state) return return_state;
            return 256;
        }
        cds_destroy_doubly_linked_list(&list_1_copy, &return_state);
        if (return_state) return return_state;
        cds_destroy_doubly_linked_list(&list_0_copy, &return_state);
        if (return_state) return return_state;
        cds_destroy_doubly_linked_list(&list_1, &return_state);
        if (return_state) return return_state;
        cds_destroy_doubly_linked_list(&list_0, &return_state);
        if (return_state) return return_state;
    }
    return 0;
}