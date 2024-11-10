#include <stdalign.h>
#include <stdbool.h>

#include "status.h"

#include "singly_linked_list_type.h"
#include "singly_linked_list.h"

int main(){
    for (size_t i = 0; i < 1000000; ++i) {
        enum cds_status return_state;
        struct cds_singly_linked_list* list 
            = cds_create_singly_linked_list(&return_state);
        if (return_state) return return_state;
        struct cds_singly_linked_list_node* last_node 
            = cds_create_singly_linked_list_node(
                sizeof(int), alignof(int), &return_state
            );
        if (return_state) return return_state;
        cds_singly_linked_list_push_front(list, last_node, &return_state);
        if (return_state) return return_state;
        for (size_t j = 0; j < 10; ++j){ 
            struct cds_singly_linked_list_node* const node 
                = cds_create_singly_linked_list_node(
                    sizeof(int), alignof(int), &return_state
                );
            if (return_state) return return_state;
            cds_singly_linked_list_push_front(list, node, &return_state);
            if (return_state) return return_state;
        }
        cds_erase_preceding_singly_linked_list_nodes(
            list, last_node, false, &return_state
        );
        if (return_state) return return_state;
        cds_singly_linked_list_destroy_front(list, &return_state);
        if (return_state) return return_state;
        if (!cds_is_singly_linked_list_empty(list)) return 256;
        cds_destroy_singly_linked_list(&list, &return_state);
        if (return_state) return return_state;
    }
    return 0;
}