#ifndef ARRAY_H
#define ARRAY_H

    #include <stddef.h>
    #include <string.h>

    struct cds_array{
        size_t data_length;
        size_t reserved_length;
        size_t bytes_per_element;
        void* data;
    };

    struct cds_array cds_create_array(
        const size_t length, const size_t bytes_per_element
    );

    inline struct cds_array cds_create_byte_array(const size_t length){
        return cds_create_array(length, 1);
    }

    struct cds_array cds_copy_and_create_array(
        const struct cds_array* const old_array
    );

    struct cds_array* cds_resize_array(
        struct cds_array* const array, const size_t new_length
    );

    struct cds_array* cds_copy_array(
        struct cds_array* const new_array, 
        const struct cds_array* const old_array
    );

    struct cds_array* cds_destroy_array(struct cds_array* const array);

#endif // ARRAY_H