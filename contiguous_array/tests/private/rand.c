#include <stdlib.h>

int cds_rand_positive(){
    int num = 0;
    while (!num) num = rand();
    return num;
}