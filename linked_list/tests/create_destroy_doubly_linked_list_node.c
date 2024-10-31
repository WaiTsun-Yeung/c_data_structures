#include <stdalign.h>

#include "doubly_linked_list_type.h"
#include "doubly_linked_list.h"

int main() {
    for (size_t i = 0; i < 1000000; ++i){
        struct cds_doubly_linked_list_node* node 
            = cds_create_doubly_linked_list_node(sizeof(int), alignof(int));
        cds_destroy_doubly_linked_list_node(&node);
    }
    return 0;
}