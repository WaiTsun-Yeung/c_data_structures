#include "list_type.h"
#include "list.h"

#include <stdio.h>

int main() {
    for (size_t i = 0; i < 1000000; ++i) {
        struct cds_list* list = cds_create_list();
        for (size_t j = 0; j < 10; ++j)
            cds_list_push_front(list, cds_create_list_node(sizeof(int)));
        struct cds_list* list_copy = cds_copy_and_create_list(list);
        struct cds_list* list_reverse = cds_copy_and_create_reverse_list(list);
        struct cds_list* list_pop_push = cds_create_list();
        for (
            struct cds_list_node* node = cds_list_begin(list);
            node != cds_list_end(list);
            cds_list_node_next(&node)
        ) cds_list_push_front(list_pop_push, cds_list_pop_front(list));
        for (
            struct cds_list_node *node_pop_push = cds_list_begin(list_pop_push),
                *node_reverse = cds_list_begin(list_reverse);
            node_pop_push != cds_list_end(list_pop_push);
            cds_list_node_next(&node_pop_push), 
                cds_list_node_next(&node_reverse)
        )if (*(int*)node_pop_push->data != *(int*)node_reverse->data) return 1;
        cds_destroy_list(&list_pop_push);
        cds_destroy_list(&list_reverse);
        cds_destroy_list(&list_copy);
        cds_destroy_list(&list);
    }
    return 0;
}