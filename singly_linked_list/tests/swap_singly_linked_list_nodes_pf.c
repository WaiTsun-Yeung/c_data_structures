#include <stdalign.h>

#include "alloc.h"
#include "singly_linked_list_type.h"
#include "singly_linked_list.h"

int main() {
    for (size_t i = 0; i < 1000000; ++i) {
        struct cds_singly_linked_list* list = cds_create_singly_linked_list();
        for (size_t j = 0; j < 10; ++j) {
            cds_singly_linked_list_push_front(
                list, 
                cds_create_singly_linked_list_node(sizeof(int), alignof(int))
            );
        }
        struct cds_singly_linked_list* list_copy 
            = cds_copy_and_create_singly_linked_list(list);
        struct cds_singly_linked_list_node *free_node
            = cds_create_singly_linked_list_node(sizeof(int), alignof(int));
        struct cds_singly_linked_list_node *free_node_copy
            = cds_create_singly_linked_list_node(sizeof(int), alignof(int));
        cds_swap_singly_linked_list_nodes_pf(list->front, &free_node);
        cds_swap_singly_linked_list_nodes_pf(list_copy->front, &free_node_copy);
        for (
            struct cds_singly_linked_list_node *node 
                    = cds_singly_linked_list_begin(list),
                *node_copy = cds_singly_linked_list_begin(list_copy);
            node != cds_singly_linked_list_end(list);
            cds_singly_linked_list_node_next(&node), 
                cds_singly_linked_list_node_next(&node_copy)
        ) if (*(int*)cds_data(node) != *(int*)cds_data(node_copy)) {
            cds_destroy_singly_linked_list(&list_copy);
            cds_destroy_singly_linked_list(&list);
            return 1;
        }
        cds_destroy_free_singly_linked_list_node(&free_node_copy);
        cds_destroy_free_singly_linked_list_node(&free_node);
        cds_destroy_singly_linked_list(&list_copy);
        cds_destroy_singly_linked_list(&list);
    }
    return 0;
}