#include <stdalign.h>

#include "status.h"

#include "singly_linked_list_type.h"
#include "singly_linked_list.h"

int main() {
    for (size_t i = 0; i < 1000000; ++i){
        enum cds_status return_state;
        struct cds_singly_linked_list_node* node 
            = cds_create_singly_linked_list_node(
                sizeof(int), alignof(int), &return_state
            );
        if (return_state) return return_state;
        cds_destroy_free_singly_linked_list_node(&node, &return_state);
    }
    return 0;
}