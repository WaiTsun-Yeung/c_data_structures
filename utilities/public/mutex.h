#include <threads.h>

#include "status.h"

enum cds_status cds_mutex_lock(
    mtx_t *restrict const mutex, 
    const struct timespec *restrict const mutex_timeout, const int mutex_type
);

#ifndef CDS_MUTEX_H
#define CDS_MUTEX_H

    static const struct timespec cds_default_mutex_timeout = {
        .tv_sec = 900, .tv_nsec = 0
    };

    static inline void cds_mutex_init(mtx_t* const mutex, const int mutex_type){
        while(mtx_init(mutex, mutex_type) == thrd_error);
    }

    static inline void cds_mutex_unlock(mtx_t* const mutex){
        while(mtx_unlock(mutex) == thrd_error);
    }

#endif // CDS_MUTEX_H