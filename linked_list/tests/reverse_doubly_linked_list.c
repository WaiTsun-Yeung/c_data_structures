#include <stdalign.h>

#include "alloc.h"
#include "status.h"

#include "doubly_linked_list_type.h"
#include "doubly_linked_list.h"

int main() {
    for (size_t i = 0; i < 1000000; ++i){
        enum cds_status return_state;
        struct cds_doubly_linked_list* list 
            = cds_create_doubly_linked_list(&return_state);
        for (size_t j = 0; j < 10; ++j){
            struct cds_doubly_linked_list_node* const node 
                = cds_create_doubly_linked_list_node(
                    sizeof(int), alignof(int), &return_state
                );
            if (return_state) return return_state;
            cds_doubly_linked_list_push_front(list, node, &return_state);
            if (return_state) return return_state;
        }
        struct cds_doubly_linked_list* list_copy 
            = cds_copy_and_create_doubly_linked_list(list, &return_state);
        if (return_state) return return_state;
        struct cds_doubly_linked_list* list_reverse
            = cds_copy_and_create_reverse_doubly_linked_list(
                list, &return_state
            );
        if (return_state) return return_state;
        struct cds_doubly_linked_list* list_pop_push
            = cds_create_doubly_linked_list(&return_state);
        if (return_state) return return_state;
        for (
            struct cds_doubly_linked_list_node* node 
                = cds_doubly_linked_list_begin(list);
            node != cds_doubly_linked_list_end();
            cds_doubly_linked_list_node_next(&node)
        ){
            struct cds_doubly_linked_list_node* const popped_node
                = cds_doubly_linked_list_pop_front(list, &return_state);
            if (return_state) return return_state;
            cds_doubly_linked_list_push_front(
                list_pop_push, popped_node, &return_state
            );
            if (return_state) return return_state;
        }
        for (
            struct cds_doubly_linked_list_node *node_pop_push 
                    = cds_doubly_linked_list_begin(list_pop_push),
                *node_reverse = cds_doubly_linked_list_begin(list_reverse);
            node_pop_push != cds_doubly_linked_list_end();
            cds_doubly_linked_list_node_next(&node_pop_push), 
                cds_doubly_linked_list_node_next(&node_reverse)
        ) if (*(int*)cds_data(node_pop_push) != *(int*)cds_data(node_reverse)) 
            return 256;
        cds_destroy_doubly_linked_list(&list_pop_push, &return_state);
        cds_destroy_doubly_linked_list(&list_reverse, &return_state);
        cds_destroy_doubly_linked_list(&list_copy, &return_state);
        cds_destroy_doubly_linked_list(&list, &return_state);
    }
    return 0;
}