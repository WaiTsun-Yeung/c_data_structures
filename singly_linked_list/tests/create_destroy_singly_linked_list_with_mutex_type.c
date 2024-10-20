#include <stdalign.h>

#include "singly_linked_list_type.h"
#include "singly_linked_list.h"

int main() {
    for (size_t i = 0; i < 1000000; ++i){
        struct cds_singly_linked_list* list 
            = cds_create_singly_linked_list_with_mutex_type(mtx_timed);
        for (size_t j = 0; j < 10; ++j)
            cds_singly_linked_list_push_front(
                list, 
                cds_create_singly_linked_list_node(sizeof(int), alignof(int))
            );
        if (cds_is_singly_linked_list_empty(list))
            return 1;
        cds_destroy_singly_linked_list(&list);
    }
    return 0;
}