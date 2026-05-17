#pragma once

#include "types.h"

typedef struct lq_utf8* lq_utf8_t;
typedef struct lq_utf8_str* lq_utf8_str_t;

lq_utf8_str_t    lq_utf8_str_create(const lq_char_t* cstr);
void             lq_utf8_str_destroy(lq_utf8_str_t str);
lq_uint32_t      lq_utf8_str_length(lq_utf8_str_t str);
lq_uint32_t      lq_utf8_str_size(lq_utf8_str_t str);
const lq_char_t* lq_utf8_str_get_cstr(lq_utf8_str_t str);