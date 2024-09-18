#include <stdalign.h>

#include "array.h"

int main(){
    for (int i = 0; i < 1000000; ++i){
        struct cds_array* array 
            = cds_create_array(
                0, sizeof(int), alignof(int)
            );
        if (!cds_is_array_empty(array))
            return 1;
        cds_destroy_array(&array);
    }
    return 0;
}