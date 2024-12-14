#include <stdalign.h>

#include "alloc.h"
#include "status.h"

#include "doubly_linked_list_type.h"
#include "doubly_linked_list.h"

int main() {
    for (size_t i = 0; i < 1000000; ++i) {
        enum cds_status return_state;
        struct cds_doubly_linked_list* list_0 
            = cds_create_doubly_linked_list(&return_state);
        if (return_state) return return_state;
        struct cds_doubly_linked_list* list_1 
            = cds_create_doubly_linked_list(&return_state);
        if (return_state) return return_state;
        for (size_t j = 0; j < 10; ++j){
            struct cds_doubly_linked_list_node* node_0
                = cds_create_doubly_linked_list_node(
                    sizeof(int), alignof(int), &return_state
                );
            if (return_state) return return_state;
            cds_doubly_linked_list_push_front(list_0, node_0, &return_state);
            if (return_state) return return_state;
            struct cds_doubly_linked_list_node* node_1
                = cds_create_doubly_linked_list_node(
                    sizeof(int), alignof(int), &return_state
                );
            if (return_state) return return_state;
            cds_doubly_linked_list_push_front(list_1, node_1, &return_state);
            if (return_state) return return_state;
        }
        struct cds_doubly_linked_list_node* list_0_front 
            = cds_doubly_linked_list_begin(list_0);
        struct cds_doubly_linked_list_node* list_1_front 
            = cds_doubly_linked_list_begin(list_1);
        cds_append_doubly_linked_list_with_range(
            list_0, list_1_front, list_1_front->next, &return_state
        );
        if (return_state) return return_state;
        const struct cds_doubly_linked_list_node* node_0 
            = cds_doubly_linked_list_begin(list_0);
        if (node_0 != list_0_front) return 256;
        for (
            size_t j = 0; j < 11; ++j, node_0 = node_0->next
        );
        const struct cds_doubly_linked_list_node* node_1 
            = cds_doubly_linked_list_begin(list_1);
        if (node_1 == list_1_front) return 256;
        for (
            size_t j = 0; j < 9; ++j, node_1 = node_1->next
        );
        cds_destroy_doubly_linked_list(&list_1, &return_state);
        if (return_state) return return_state;
        cds_destroy_doubly_linked_list(&list_0, &return_state);
        if (return_state) return return_state;
    }
    return 0;
}