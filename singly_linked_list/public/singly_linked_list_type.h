#ifndef CDS_SINGLY_LINKED_LIST_TYPE_H
#define CDS_SINGLY_LINKED_LIST_TYPE_H

    #include <stddef.h>
    #include <threads.h>

    struct cds_singly_linked_list{
        mtx_t mutex;
        int mutex_type;
        struct cds_singly_linked_list_node* front;
    };

    struct cds_singly_linked_list_node{
        size_t data_offset;
        size_t bytes_per_element;
        struct cds_singly_linked_list* list;
        struct cds_singly_linked_list_node* next;
    };

#endif // CDS_SINGLY_LINKED_LIST_TYPE_H