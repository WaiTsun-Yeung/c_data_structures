#include <threads.h>

#include "status.h"

/// @brief Locks a mutex.
/// @param[in,out] mutex The mutex to lock
/// @param[in] mutex_timeout The timeout for the mutex. This only applies if
///     the mutex type is `mtx_timed`.
/// @param[in] mutex_type The type of the mutex, defined in `threads.h`.
/// @return returns `thrd_success` if the mutex is successfully locked.
///     `thrd_timedout` if the mutex operation times out.
///     `thrd_error` if the mutex operation fails otherwise.
enum cds_status cds_mutex_lock(
    mtx_t *restrict const mutex, 
    const struct timespec *restrict const mutex_timeout, 
    const int mutex_type,
    enum cds_status *restrict const return_state
){
    int mutex_lock_result;
    if (
        mutex_timeout && mutex_type & mtx_timed
    ) mutex_lock_result = mtx_timedlock(mutex, mutex_timeout);
    else mutex_lock_result = mtx_lock(mutex);
    switch (mutex_lock_result){
        case thrd_success: 
            if (return_state) *return_state = CDS_SUCCESS;
            return CDS_SUCCESS;
        case thrd_timedout: 
            if (return_state) *return_state = CDS_MUTEX_TIMEOUT;
            return CDS_MUTEX_TIMEOUT;
        case thrd_error: 
            if (return_state) *return_state = CDS_MUTEX_ERROR;
            return CDS_MUTEX_ERROR;
        default: 
            if (return_state) *return_state = CDS_IMPOSSIBLE;
            return CDS_IMPOSSIBLE;
    }
}