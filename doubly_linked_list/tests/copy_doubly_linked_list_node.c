#include <stdalign.h>

#include "alloc.h"
#include "doubly_linked_list_type.h"
#include "doubly_linked_list.h"

int main() {
    for (size_t i = 0; i < 1000000; ++i){
        struct cds_doubly_linked_list_node* node_0 
            = cds_create_doubly_linked_list_node(sizeof(int), alignof(int));
        struct cds_doubly_linked_list_node* node_1
            = cds_create_doubly_linked_list_node(
                sizeof(unsigned long long), alignof(unsigned long long)
            );
        cds_copy_doubly_linked_list_node(&node_0, node_1);
        if (
            *(unsigned long long*)cds_data(node_0) 
                != *(unsigned long long*)cds_data(node_1)
        ){
            cds_destroy_doubly_linked_list_node(&node_1);
            cds_destroy_doubly_linked_list_node(&node_0);
            return 1;
        }
        cds_destroy_doubly_linked_list_node(&node_1);
        cds_destroy_doubly_linked_list_node(&node_0);
    }
    return 0;
}