#include "list_type.h"
#include "list.h"

int main() {
    for (size_t i = 0; i < 1000000; ++i) 
        free(cds_create_list_node(sizeof(int)));
    return 0;
}