#include <stdalign.h>

#include "doubly_linked_list_type.h"
#include "doubly_linked_list.h"

int main(){
    for (size_t i = 0; i < 1000000; ++i){
        struct cds_doubly_linked_list* list = cds_create_doubly_linked_list();
        for (size_t j = 0; j < 10; ++j)
            cds_doubly_linked_list_push_front(
                list, 
                cds_create_doubly_linked_list_node(sizeof(int), alignof(int))
            );
        for (
            struct cds_doubly_linked_list_node* node 
                = cds_doubly_linked_list_rbegin(list);
            node != cds_doubly_linked_list_rend(list);
            cds_doubly_linked_list_node_prev(&node)
        );
        for(size_t j = 0; j < 10; ++j) 
            cds_doubly_linked_list_destroy_front(list);
        cds_destroy_doubly_linked_list(&list);
    }
    return 0;
}