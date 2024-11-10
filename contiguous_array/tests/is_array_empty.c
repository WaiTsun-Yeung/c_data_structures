#include <stdalign.h>

#include "status.h"
#include "array.h"

int main(){
    for (int i = 0; i < 1000000; ++i){
        enum cds_status return_state;
        struct cds_array* array 
            = cds_create_array(
                0, sizeof(int), alignof(int), &return_state
            );
        if (return_state) return return_state;
        if (!cds_is_array_empty(array))
            return 256;
        cds_destroy_array(&array);
    }
    return 0;
}