#include <stdalign.h>

#include "singly_linked_list_type.h"
#include "singly_linked_list.h"

int main(){
    for (size_t i = 0; i < 1000000; ++i) {
        struct cds_singly_linked_list* list = cds_create_singly_linked_list();
        for (size_t j = 0; j < 10; ++j)
            cds_singly_linked_list_push_front(
                list, cds_create_singly_linked_list_node(
                    sizeof(int), alignof(int)
                )
            );
        struct cds_singly_linked_list* list_copy 
            = cds_copy_and_create_singly_linked_list(list);
        for (
            struct cds_singly_linked_list_node *node 
                = cds_singly_linked_list_begin(list),
                *node_copy = cds_singly_linked_list_begin(list_copy);
            node != cds_singly_linked_list_end(list);
            cds_singly_linked_list_node_next(&node), 
            cds_singly_linked_list_node_next(&node_copy)
        ) if (*(int*)node->data != *(int*)node_copy->data){
            cds_destroy_singly_linked_list(&list_copy);
            cds_destroy_singly_linked_list(&list);
            return 1;
        }
        cds_destroy_singly_linked_list(&list_copy);
        cds_destroy_singly_linked_list(&list);
    }
    return 0;
}