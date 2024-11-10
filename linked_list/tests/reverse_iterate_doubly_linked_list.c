#include <stdalign.h>

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
                    sizeof(size_t), alignof(size_t), &return_state
                );
            if (return_state) return return_state;
            cds_doubly_linked_list_push_front(list, node, &return_state);
            if (return_state) return return_state;
        }
        for (
            struct cds_doubly_linked_list_node* node 
                = cds_doubly_linked_list_rbegin(list);
            node != cds_doubly_linked_list_rend(list);
            cds_doubly_linked_list_node_prev(&node)
        );
        for(size_t j = 0; j < 10; ++j){ 
            cds_doubly_linked_list_destroy_front(list, &return_state);
            if (return_state) return return_state;
        }
        cds_destroy_doubly_linked_list(&list, &return_state);
        if (return_state) return return_state;
    }
    return 0;
}