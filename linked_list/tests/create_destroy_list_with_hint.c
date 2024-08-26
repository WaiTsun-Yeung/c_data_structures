#include <omp.h>

#include "list_type.h"
#include "list.h"

int main() {
    for (size_t i = 0; i < 1000000; ++i){
        struct cds_list* list 
            = cds_create_list_with_hint(omp_sync_hint_uncontended);
        for (size_t j = 0; j < 10; ++j)
            cds_list_push_front(list, cds_create_list_node(sizeof(int)));
        cds_destroy_list(&list);
    }
    return 0;
}