#pragma once

#include "types.h"

LQ_CORE_API static inline const lq_byte_t* lq_cast_to_bytes(const lq_char_t* cstr) { return (lq_byte_t*)cstr; }
LQ_CORE_API static inline const lq_char_t* lq_cast_to_cstr(const lq_byte_t* bytes) { return (const lq_char_t*)bytes; }

LQ_CORE_API lq_bool_t lq_inspect_raw_utf8(lq_uint32_t* out_opt_length, lq_uint32_t* out_opt_size, const lq_byte_t* raw_utf8) ;
LQ_CORE_API lq_bool_t lq_inspect_utf8_cstr(lq_uint32_t* out_opt_length, lq_uint32_t* out_opt_size, const lq_char_t* data);

typedef struct lq_utf8* lq_utf8_t;
typedef struct lq_utf8_str* lq_utf8_str_t;

LQ_CORE_API lq_utf8_str_t    lq_utf8_str_create(const lq_byte_t* raw_utf8);
LQ_CORE_API lq_utf8_str_t    lq_utf8_str_create_cstr(const lq_char_t* cstr);
LQ_CORE_API void             lq_utf8_str_destroy(lq_utf8_str_t str);
LQ_CORE_API lq_uint32_t      lq_utf8_str_get_length(const lq_utf8_str_t str);
LQ_CORE_API lq_uint32_t      lq_utf8_str_get_size(const lq_utf8_str_t str);
LQ_CORE_API const lq_char_t* lq_utf8_str_get_cstr(const lq_utf8_str_t str);
LQ_CORE_API const lq_byte_t* lq_utf8_str_get_bytes(const lq_utf8_str_t str);