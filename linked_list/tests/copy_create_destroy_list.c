#include "list_type.h"
#include "list.h"

int main(){
    for (size_t i = 0; i < 1000000; ++i) {
        struct cds_list* list = cds_create_list();
        list->front = cds_create_list_node(sizeof(int));
        list->front->next = cds_create_list_node(sizeof(int));
        list->front->next->next = cds_create_list_node(sizeof(int));
        struct cds_list* list_copy = cds_copy_and_create_list(list);
        for (
            struct cds_list_node* node = list->front, 
                *node_copy = list_copy->front;
            node && node_copy;
            node = node->next, node_copy = node_copy->next
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