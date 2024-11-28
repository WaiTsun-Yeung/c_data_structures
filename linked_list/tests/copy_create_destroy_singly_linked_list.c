#include <stdalign.h>

#include "status.h"
#include "alloc.h"
#include "singly_linked_list_type.h"
#include "singly_linked_list.h"

int main(){
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
            if (return_state) return return_state;
            cds_singly_linked_list_push_front( list, node, &return_state);
            if (return_state) return return_state;
        }
        struct cds_singly_linked_list* list_copy 
            = cds_copy_and_create_singly_linked_list(list, &return_state);
        if (return_state) return return_state;
        for (
            struct cds_singly_linked_list_node *node 
                = cds_singly_linked_list_begin(list),
                *node_copy = cds_singly_linked_list_begin(list_copy);
            node != cds_singly_linked_list_end();
            cds_singly_linked_list_node_next(&node), 
            cds_singly_linked_list_node_next(&node_copy)
        ) if (*(int*)cds_data(node) != *(int*)cds_data(node_copy)){
            cds_destroy_singly_linked_list(&list_copy, &return_state);
            if (return_state) return return_state;
            cds_destroy_singly_linked_list(&list, &return_state);
            if (return_state) return return_state;
            return 256;
        }
        cds_destroy_singly_linked_list(&list_copy, &return_state);
        if (return_state) return return_state;
        cds_destroy_singly_linked_list(&list, &return_state);
        if (return_state) return return_state;
    }
    return 0;
}