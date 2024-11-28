#include <stdlib.h>

int cds_rand_positive(void);

#ifndef CDS_RAND_H
#define CDS_RAND_H
    static inline int cds_rand_range(const int min, const int max){
        return min + rand() / (RAND_MAX / (max - min + 1) + 1);
    }
#endif