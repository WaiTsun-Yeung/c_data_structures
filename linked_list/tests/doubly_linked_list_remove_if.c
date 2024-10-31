#include <stdalign.h>
#include <stddef.h>
#include <stdbool.h>

#include "doubly_linked_list_type.h"
#include "doubly_linked_list.h"

static bool is_data_size_same_as_int(
    const struct cds_doubly_linked_list_node* const node
){
    return node->bytes_per_element == sizeof(int);
}

int main(){
    for (size_t i = 0; i < 1000000; ++i){
        struct cds_doubly_linked_list* list = cds_create_doubly_linked_list();
        for (size_t j = 0; j < 10; ++j)
            cds_doubly_linked_list_push_front(
                list, 
                cds_create_doubly_linked_list_node(sizeof(int), alignof(int))
            );
        cds_doubly_linked_list_remove_if(list, is_data_size_same_as_int);
        if (!cds_is_doubly_linked_list_empty(list)) return 1;
        cds_destroy_doubly_linked_list(&list);
    }
    return 0;
}