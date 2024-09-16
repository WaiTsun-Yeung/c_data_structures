#include <stdalign.h>

#include "utilities.h"
#include "singly_linked_list_type.h"
#include "singly_linked_list.h"

int main() {
    for (size_t i = 0; i < 1000000; ++i) {
        struct cds_singly_linked_list* list = cds_create_singly_linked_list();
        for (size_t j = 0; j < 10; ++j)
            cds_singly_linked_list_push_front(
                list, 
                cds_create_singly_linked_list_node(sizeof(int), alignof(int))
            );
        struct cds_singly_linked_list* list_copy 
            = cds_copy_and_create_singly_linked_list(list);
        struct cds_singly_linked_list* list_reverse 
            = cds_copy_and_create_reverse_singly_linked_list(list);
        struct cds_singly_linked_list* list_pop_push 
            = cds_create_singly_linked_list();
        for (
            struct cds_singly_linked_list_node* node 
                = cds_singly_linked_list_begin(list);
            node != cds_singly_linked_list_end(list);
            cds_singly_linked_list_node_next(&node)
        ) cds_singly_linked_list_push_front(
            list_pop_push, cds_singly_linked_list_pop_front(list)
        );
        for (
            struct cds_singly_linked_list_node *node_pop_push 
                    = cds_singly_linked_list_begin(list_pop_push),
                *node_reverse = cds_singly_linked_list_begin(list_reverse);
            node_pop_push != cds_singly_linked_list_end(list_pop_push);
            cds_singly_linked_list_node_next(&node_pop_push), 
                cds_singly_linked_list_node_next(&node_reverse)
        ) if (*(int*)cds_data(node_pop_push) != *(int*)cds_data(node_reverse)) 
            return 1;
        cds_destroy_singly_linked_list(&list_pop_push);
        cds_destroy_singly_linked_list(&list_reverse);
        cds_destroy_singly_linked_list(&list_copy);
        cds_destroy_singly_linked_list(&list);
    }
    return 0;
}