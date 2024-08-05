#include <stdlib.h>

int cds_rand_positive(){
    int num = 0;
    while (!num) num = rand();
    return num;
}

int cds_rand_range(const int min, const int max){
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}