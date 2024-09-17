#include <stdalign.h>

#include "utilities.h"
#include "singly_linked_list_type.h"
#include "singly_linked_list.h"

int main() {
    for (size_t i = 0; i < 1000000; ++i){
        struct cds_singly_linked_list_node* node_0 
            = cds_create_singly_linked_list_node(sizeof(int), alignof(int));
        struct cds_singly_linked_list_node* node_1
            = cds_copy_and_create_singly_linked_list_node(node_0);
        if (
            *(int*)cds_data(node_0) 
                != *(int*)cds_data(node_1)
        ){
            cds_destroy_free_singly_linked_list_node(&node_1);
            cds_destroy_free_singly_linked_list_node(&node_0);
            return 1;
        }
        cds_destroy_free_singly_linked_list_node(&node_1);
        cds_destroy_free_singly_linked_list_node(&node_0);
    }
    return 0;
}