#include <threads.h>
#include <setjmp.h>

#include "status.h"

/// @brief Locks a mutex.
/// @param[in,out] mutex The mutex to lock
/// @param[in] mutex_timeout The timeout for the mutex. This only applies if the
///     mutex type is `mtx_timed`.
/// @param[in] mutex_type The type of the mutex, defined in `threads.h`.
/// @return `false` if the mutex was locked, `true` if the mutex timed out.
enum cds_status cds_mutex_lock(
    mtx_t *restrict const mutex, 
    const struct timespec *restrict const mutex_timeout, const int mutex_type
){
    if (
        mutex_timeout && mutex_type & mtx_timed
    ){
        int timedlock_result;
        do {
            timedlock_result = mtx_timedlock(mutex, mutex_timeout);
            if (timedlock_result == thrd_timedout) return CDS_MUTEX_TIMEOUT;
        } while (timedlock_result == thrd_error);
    } else {while(mtx_lock(mutex) == thrd_error);}
    return CDS_SUCCESS;
}