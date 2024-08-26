#include <omp.h>

#include "list_type.h"
#include "list.h"

int main() {
    for (size_t i = 0; i < 1000000; ++i){
        struct cds_list* list 
            = cds_create_list_with_hint(omp_sync_hint_uncontended);
        list->front = cds_create_list_node(sizeof(int));
        list->front->next = cds_create_list_node(sizeof(int));
        list->front->next->next = cds_create_list_node(sizeof(int));
        cds_destroy_list(&list);
    }
    return 0;
}