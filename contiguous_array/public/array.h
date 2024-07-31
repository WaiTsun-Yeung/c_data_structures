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

    void* cds_create_array(
        const size_t length, const size_t bytes_per_element
    );

    inline void* cds_create_byte_array(const size_t length){
        return cds_create_array(length, 1);
    }

    inline void* cds_copy_and_create_array(
        const void* const old_array
    ){
        return memcpy(
            cds_create_array(
                ((const struct cds_array* const)old_array)->data_length, 
                ((const struct cds_array* const)old_array)->bytes_per_element
            ), 
            old_array, 
            ((const struct cds_array* const)old_array)->data_length 
                * ((const struct cds_array* const)old_array)->bytes_per_element
        );
    }

    void* cds_resize_array(void* array, const size_t new_length);

    void* cds_copy_array(
        void* const new_array, const void* const old_array
    );

    void* cds_destroy_array(void* array);

#endif // ARRAY_H