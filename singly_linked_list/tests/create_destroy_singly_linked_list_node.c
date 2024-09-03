#include "singly_linked_list_type.h"
#include "singly_linked_list.h"

int main() {
    for (size_t i = 0; i < 1000000; ++i){
        struct cds_singly_linked_list_node* node 
            = cds_create_singly_linked_list_node(sizeof(int));
        cds_destroy_free_singly_linked_list_node(&node);
    }
    return 0;
}