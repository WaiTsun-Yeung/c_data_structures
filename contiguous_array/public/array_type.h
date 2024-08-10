#ifndef ARRAY_TYPE_H
#define ARRAY_TYPE_H    

    #include <stddef.h>

    struct cds_array{
        size_t data_length;
        size_t reserved_length;
        size_t bytes_per_element;
        void* data;
    };

#endif // ARRAY_TYPE_H