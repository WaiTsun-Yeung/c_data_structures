#ifndef ARRAY_TYPE_H
#define ARRAY_TYPE_H    

    #include <stddef.h>

    struct cds_array{
        void* data;
        size_t bytes_per_element;
        size_t data_length;
        size_t reserved_length;
    };

#endif // ARRAY_TYPE_H