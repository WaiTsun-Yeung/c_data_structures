#include <stdlib.h>

#include "list_type.h"
#include "list.h"

int main(){
    for (size_t i = 0; i < 1000000; ++i) {
        struct cds_list* list = cds_create_list();
        cds_list_push_front(list, cds_create_list_node(sizeof(int)));
        for (size_t j = 0; j < 10; ++j) 
            cds_push_next_list_node(
                list->front, cds_create_list_node(sizeof(int))
            );
        for (size_t j = 0; j < 10; ++j) 
            free(cds_pop_next_list_node(list->front));
        cds_list_destroy_front(list);
        cds_destroy_list(&list);
    }
    return 0;
}