#include "list_type.h"
#include "list.h"

int main() {
    for (size_t i = 0; i < 1000000; ++i) {
        struct cds_list* list_0 = cds_create_list();
        struct cds_list* list_1 = cds_create_list();
        for (size_t j = 0; j < 10; ++j) {
            cds_list_push_front(list_0, cds_create_list_node(sizeof(int)));
            cds_list_push_front(list_1, cds_create_list_node(sizeof(int)));
        }
        struct cds_list* list_0_copy = cds_copy_and_create_list(list_0);
        struct cds_list* list_1_copy = cds_copy_and_create_list(list_1);
        for (
            struct cds_list_node *node_0 = cds_list_begin(list_0),
                *node_1 = cds_list_begin(list_1);
            node_0 != cds_list_end(list_0);
            cds_list_node_next(&node_0), cds_list_node_next(&node_1)
        ) cds_swap_list_nodes(&node_0, &node_1);
        for (
            struct cds_list_node *node_0 = cds_list_begin(list_0),
                *node_1 = cds_list_begin(list_1),
                *node_0_copy = cds_list_begin(list_0_copy),
                *node_1_copy = cds_list_begin(list_1_copy);
            node_0 != cds_list_end(list_0);
            cds_list_node_next(&node_0), 
                cds_list_node_next(&node_1),
                cds_list_node_next(&node_0_copy), 
                cds_list_node_next(&node_1_copy)
        ) if (*(int*)node_0->data != *(int*)node_1_copy->data ||
            *(int*)node_1->data != *(int*)node_0_copy->data) {
            cds_destroy_list(&list_1_copy);
            cds_destroy_list(&list_0_copy);
            cds_destroy_list(&list_1);
            cds_destroy_list(&list_0);
            return 1;
        }
        cds_destroy_list(&list_1_copy);
        cds_destroy_list(&list_0_copy);
        cds_destroy_list(&list_1);
        cds_destroy_list(&list_0);
    }
    return 0;
}