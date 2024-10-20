#ifndef CDS_STATUS_H
#define CDS_STATUS_H

    enum cds_status{
        ///< The function completes successfully.
        CDS_SUCCESS = 0,
        ///< The mutex operation timed out and failed.
        CDS_MUTEX_TIMEOUT = 1,
        ///< The function fails to complete because of null arguments.
        CDS_NULL_ARG = 2,
        ///< The function fails to complete because of invalid arguments.
        CDS_INVALID_ARG = 3
    };

#endif // CDS_STATUS_H