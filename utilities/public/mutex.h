#include <threads.h>

#include "status.h"

#ifndef CDS_MUTEX_H
#define CDS_MUTEX_H

    static const struct timespec cds_default_mutex_timeout = {
        .tv_sec = 900, .tv_nsec = 0
    };

    /// @brief Locks a mutex.
    /// @param[in,out] mutex The mutex to lock
    /// @param[in] mutex_timeout The timeout for the mutex. This only applies if
    ///     the mutex type is `mtx_timed`.
    /// @param[in] mutex_type The type of the mutex, defined in `threads.h`.
    /// @return returns `thrd_success` if the mutex is successfully locked.
    ///     `thrd_timedout` if the mutex operation times out.
    ///     `thrd_error` if the mutex operation fails otherwise.
    static inline int cds_mutex_lock(
        mtx_t *restrict const mutex, 
        const struct timespec *restrict const mutex_timeout, 
        const int mutex_type
    ){
        if (
            mutex_timeout && mutex_type & mtx_timed
        ) return mtx_timedlock(mutex, mutex_timeout);
        else return mtx_lock(mutex);
    }

    static inline void cds_mutex_unlock(mtx_t* const mutex){
        while(mtx_unlock(mutex) == thrd_error);
    }

#endif // CDS_MUTEX_H