#include <stdint.h>

#include "array_type.h"

struct cds_array* cds_create_uintmax_t_array(const size_t values_count, ...);

struct cds_array* cds_create_int_array(const size_t values_count, ...);

struct cds_array* cds_create_intmax_t_array(const size_t values_count, ...);

struct cds_array* cds_create_uint8_t_array(const size_t values_count, ...);

struct cds_array* cds_create_uint16_t_array(const size_t values_count, ...);

struct cds_array* cds_create_uint32_t_array(const size_t values_count, ...);

struct cds_array* cds_create_uint64_t_array(const size_t values_count, ...);

struct cds_array* cds_create_int8_t_array(const size_t values_count, ...);

struct cds_array* cds_create_int16_t_array(const size_t values_count, ...);

struct cds_array* cds_create_int32_t_array(const size_t values_count, ...);

struct cds_array* cds_create_int64_t_array(const size_t values_count, ...);

struct cds_array* cds_create_float_array(const size_t values_count, ...);

struct cds_array* cds_create_double_array(const size_t values_count, ...);

struct cds_array* cds_create_long_double_array(const size_t values_count, ...);

struct cds_array* cds_create_char_array(const size_t values_count, ...);

struct cds_array* cds_create_unsigned_char_array(
    const size_t values_count, ...
);

struct cds_array* cds_create_signed_char_array(const size_t values_count, ...);