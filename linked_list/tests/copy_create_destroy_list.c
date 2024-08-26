#include "list_type.h"
#include "list.h"

int main(){
    for (size_t i = 0; i < 1000000; ++i) {
        struct cds_list* list = cds_create_list();
        for (size_t j = 0; j < 10; ++j)
            cds_list_push_front(list, cds_create_list_node(sizeof(int)));
        struct cds_list* list_copy = cds_copy_and_create_list(list);
        for (
            struct cds_list_node *node = cds_list_begin(list),
                *node_copy = cds_list_begin(list_copy);
            node != cds_list_end(list);
            cds_list_node_next(&node), cds_list_node_next(&node_copy)
        ) if (*(int*)node->data != *(int*)node_copy->data){
            cds_destroy_list(&list_copy);
            cds_destroy_list(&list);
            return 1;
        }
        cds_destroy_list(&list_copy);
        cds_destroy_list(&list);
    }
    return 0;
}