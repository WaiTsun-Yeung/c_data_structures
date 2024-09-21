#ifndef CDS_SINGLY_LINKED_LIST_TYPE_H
#define CDS_SINGLY_LINKED_LIST_TYPE_H

    #include <stddef.h>

    #include <omp.h>

    struct cds_singly_linked_list{
        omp_lock_t lock;
        omp_sync_hint_t hint;
        struct cds_singly_linked_list_node* front;
    };

    struct cds_singly_linked_list_node{
        ptrdiff_t data_offset;
        size_t bytes_per_element;
        struct cds_singly_linked_list* list;
        struct cds_singly_linked_list_node* next;
    };

#endif // CDS_SINGLY_LINKED_LIST_TYPE_H