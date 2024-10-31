#include <stdalign.h>
#include <stdlib.h>

#include "doubly_linked_list_type.h"
#include "doubly_linked_list.h"

int main() {
    for (size_t i = 0; i < 1000000; ++i){
        struct cds_doubly_linked_list* list 
            = cds_create_doubly_linked_list();
        for (size_t j = 0; j < 10; ++j) 
            cds_push_next_doubly_linked_list_node(
                list->front, 
                cds_create_doubly_linked_list_node(sizeof(int), alignof(int))
            );
        for (size_t j = 0; j < 10; ++j){
            struct cds_doubly_linked_list_node* node 
                = cds_pop_doubly_linked_list_node(list->front);
            cds_destroy_doubly_linked_list_node(&node);
        }
        if (!cds_is_doubly_linked_list_empty(list)) return 1;
        cds_destroy_doubly_linked_list(&list);
    }
    return 0;
}