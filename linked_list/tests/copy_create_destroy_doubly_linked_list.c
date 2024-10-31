#include <stdalign.h>

#include "alloc.h"
#include "doubly_linked_list_type.h"
#include "doubly_linked_list.h"

int main(){
    for (size_t i = 0; i < 1000000; ++i) {
        struct cds_doubly_linked_list* list = cds_create_doubly_linked_list();
        for (size_t j = 0; j < 10; ++j)
            cds_doubly_linked_list_push_front(
                list, 
                cds_create_doubly_linked_list_node(sizeof(int), alignof(int))
            );
        struct cds_doubly_linked_list* list_copy 
            = cds_copy_and_create_doubly_linked_list(list);
        for (
            struct cds_doubly_linked_list_node *node 
                = cds_doubly_linked_list_begin(list),
                *node_copy = cds_doubly_linked_list_begin(list_copy);
            node != cds_doubly_linked_list_end(list);
            cds_doubly_linked_list_node_next(&node), 
            cds_doubly_linked_list_node_next(&node_copy)
        ) if (*(int*)cds_data(node) != *(int*)cds_data(node_copy)){
            cds_destroy_doubly_linked_list(&list_copy);
            cds_destroy_doubly_linked_list(&list);
            return 1;
        }
        cds_destroy_doubly_linked_list(&list_copy);
        cds_destroy_doubly_linked_list(&list);
    }
    return 0;
}