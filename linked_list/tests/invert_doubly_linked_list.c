#include <stdalign.h>

#include "alloc.h"
#include "status.h"

#include "doubly_linked_list_type.h"
#include "doubly_linked_list.h"

int main(){
    for (size_t i = 0; i < 1000000; ++i){
        enum cds_status return_state;
        struct cds_doubly_linked_list* list 
            = cds_create_doubly_linked_list(&return_state);
        if (return_state) return return_state;
        for (size_t j = 0; j < 10; ++j){
            struct cds_doubly_linked_list_node* const node
                = cds_create_doubly_linked_list_node(
                    sizeof(int), alignof(int), &return_state
                );
            if (return_state) return return_state;
            cds_doubly_linked_list_push_front(list, node, &return_state);
            if (return_state) return return_state;
        }
        struct cds_doubly_linked_list* list_reverse
            = cds_copy_and_create_reverse_doubly_linked_list(
                list, &return_state
            );
        if (return_state) return return_state;
        cds_invert_doubly_linked_list(list, &return_state);
        if (return_state) return return_state;
        size_t j = 0;
        for (
            struct cds_doubly_linked_list_node *node 
                    = cds_doubly_linked_list_begin(list),
                *node_reverse = cds_doubly_linked_list_begin(list_reverse);
            node != cds_doubly_linked_list_end(list);
            cds_doubly_linked_list_node_next(&node), 
                cds_doubly_linked_list_node_next(&node_reverse), ++j
        ) if (*(int*)cds_data(node) != *(int*)cds_data(node_reverse)){
            cds_destroy_doubly_linked_list(&list_reverse, &return_state);
            if (return_state) return return_state;
            cds_destroy_doubly_linked_list(&list, &return_state);
            if (return_state) return return_state;
            return 256;
        }
        cds_destroy_doubly_linked_list(&list_reverse, &return_state);
        if (return_state) return return_state;
        cds_destroy_doubly_linked_list(&list, &return_state);
        if (return_state) return return_state;
    }
    return 0;
}