#ifndef CDS_ARRAY_TYPE_H
#define CDS_ARRAY_TYPE_H    

    #include <stddef.h>
    
    struct cds_array{
        size_t data_offset;
        size_t bytes_per_element;
        size_t elements_count;
        size_t reserved_count;
    };

#endif // CDS_ARRAY_TYPE_H