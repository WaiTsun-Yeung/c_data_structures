#ifndef LIST_TYPE_H
#define LIST_TYPE_H

    #include <stddef.h>

    #include <omp.h>

    struct cds_singly_linked_list_node{
        void* data;
        size_t bytes_per_element;
        omp_lock_t* lock;
        struct cds_singly_linked_list_node* next;
    };

    struct cds_singly_linked_list{
        omp_lock_t lock;
        omp_sync_hint_t hint;
        struct cds_singly_linked_list_node* front;
    };

#endif // LIST_TYPE_H