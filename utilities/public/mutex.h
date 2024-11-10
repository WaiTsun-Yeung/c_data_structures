#include <threads.h>

#include "status.h"

enum cds_status cds_mutex_lock(
    mtx_t *restrict const mutex, 
    const struct timespec *restrict const mutex_timeout, 
    const int mutex_type,
    enum cds_status *restrict const return_state
);

#ifndef CDS_MUTEX_H
#define CDS_MUTEX_H

    static const struct timespec cds_default_mutex_timeout = {
        .tv_sec = 900, .tv_nsec = 0
    };

#endif // CDS_MUTEX_H