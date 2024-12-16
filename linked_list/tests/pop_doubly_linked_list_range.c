#include <stdalign.h>

#include "alloc.h"
#include "status.h"

#include "doubly_linked_list_type.h"
#include "doubly_linked_list.h"

int main() {
    for (size_t i = 0; i < 1000000; ++i) {
        enum cds_status return_state;
        struct cds_doubly_linked_list* list 
            = cds_create_doubly_linked_list(&return_state);
        if (return_state) return return_state;
        for (size_t j = 0; j < 10; ++j){
            struct cds_doubly_linked_list_node* node
                = cds_create_doubly_linked_list_node(
                    sizeof(int), alignof(int), &return_state
                );
            if (return_state) return return_state;
            cds_doubly_linked_list_push_front(list, node, &return_state);
            if (return_state) return return_state;
        }
        struct cds_doubly_linked_list_node* list_front 
            = cds_doubly_linked_list_begin(list);
        struct cds_doubly_linked_list* popped_list
            = cds_pop_doubly_linked_list_range(
                list_front, list_front->next, &return_state
            );
        if (return_state) return return_state;
        const struct cds_doubly_linked_list_node* node
            = cds_doubly_linked_list_begin(list);
        if (node == list_front) return 256;
        for (
            size_t j = 0; j < 9; ++j, node = node->next
        );
        const struct cds_doubly_linked_list_node* popped_node
            = cds_doubly_linked_list_begin(popped_list);
        if (popped_node != list_front) return 256;
        for (
            size_t j = 0; j < 1; ++j, popped_node = popped_node->next
        );
        cds_destroy_doubly_linked_list(&popped_list, &return_state);
        if (return_state) return return_state;
        cds_destroy_doubly_linked_list(&list, &return_state);
        if (return_state) return return_state;
    }
}