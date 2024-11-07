#ifndef CDS_STATUS_H
#define CDS_STATUS_H

    enum cds_status{
        ///< The function completes successfully.
        CDS_SUCCESS = 0,
        ///< The function fails to complete because of null arguments.
        CDS_NULL_ARG,
        ///< The function fails to complete because of invalid arguments.
        CDS_INVALID_ARG,
        ///< The function fails to complete because of 
        ///<     memory allocation failure.
        CDS_ALLOC_ERROR,
        ///< The function fails to complete because the allocated buffer 
        ///<     is too small.
        CDS_BUFFER_OVERFLOW,
        ///< The function fails to complete because of
        ///<     buffer copy failure.
        CDS_COPY_ERROR,
        ///< The mutex operation timed out and failed.
        CDS_MUTEX_TIMEOUT,
        ///< The mutex operation failed.
        CDS_MUTEX_ERROR,
    };

#endif // CDS_STATUS_H

void cds_print_error_message(
    const errno_t error_code, 
    const char* const filename, const int line_number, 
    const char* const function_name,
    const char* const context_description
);