#ifndef LIST_TYPE_H
#define LIST_TYPE_H

    #include <stddef.h>

    #include <omp.h>

    struct cds_list_node{
        size_t bytes_per_element;
        struct cds_list_node* next;
        void* data;
    };

    struct cds_list{
        omp_lock_t lock;
        omp_sync_hint_t hint;
        struct cds_list_node* front;
    };

#endif // LIST_TYPE_H