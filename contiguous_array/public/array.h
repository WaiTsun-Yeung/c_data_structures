#ifndef ARRAY_H
#define ARRAY_H

    #include <stddef.h>
    #include <string.h>

    void* cds_create_byte_array(const size_t length);

    inline void* cds_create_array(
        const size_t length, const size_t bytes_per_element
    ){
        return cds_create_byte_array(length * bytes_per_element);
    }

    inline void* cds_copy_and_create_byte_array(
        const void* const old_array, const size_t length
    ){
        return memcpy(cds_create_byte_array(length), old_array, length);
    }

    inline void* cds_copy_and_create_array(
        const void* const old_array, const size_t length, 
        const size_t bytes_per_element
    ){
        return memcpy(
            cds_create_array(length, bytes_per_element), 
            old_array, 
            length * bytes_per_element
        );
    }

    void* cds_copy_byte_array(
        void* const new_array, const void* const old_array, const size_t length
    );

    void* cds_copy_array(
        void* const new_array, const void* const old_array, const size_t length,
        const size_t bytes_per_element
    );

    void* cds_resize_byte_array(void* array, const size_t new_length);

    void* cds_resize_byte_array_s(
        void* array, const size_t new_length, const size_t old_length
    );

    inline void* cds_resize_array(
        void* array, const size_t new_length, const size_t bytes_per_element
    ){
        return cds_resize_byte_array(array, new_length * bytes_per_element);
    }

    void* cds_resize_array_s(
        void* array, const size_t new_length, 
        const size_t bytes_per_element, const size_t old_length
    );

    void* cds_destroy_array(void* array);

#endif // ARRAY_H