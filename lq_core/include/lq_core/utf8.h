#pragma once

#include "types.h"

lq_bool_t lq_inspect_raw_utf8(lq_uint32_t* out_opt_length, lq_uint32_t* out_opt_size, const lq_byte_t* raw_utf8);
lq_bool_t lq_inspect_utf8_cstr(lq_uint32_t* out_opt_length, lq_uint32_t* out_opt_size, const lq_char_t* data);
typedef struct lq_utf8* lq_utf8_t;
typedef struct lq_utf8_str* lq_utf8_str_t;

lq_utf8_str_t    lq_utf8_str_create(const lq_char_t* cstr);
void             lq_utf8_str_destroy(lq_utf8_str_t str);
lq_uint32_t      lq_utf8_str_length(lq_utf8_str_t str);
lq_uint32_t      lq_utf8_str_size(lq_utf8_str_t str);
const lq_char_t* lq_utf8_str_get_cstr(lq_utf8_str_t str);