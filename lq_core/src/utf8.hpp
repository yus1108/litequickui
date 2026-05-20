#pragma once
#include "lq_core/utf8.h"

#include "lq_core/defines.h"

enum
{
	LQ_UINT8_MAX_SIZE = 4,
};

typedef struct lq_utf8
{
	lq_byte_t  bytes[LQ_UINT8_MAX_SIZE];
	lq_uint8_t size;
} *lq_utf8_t;

typedef struct lq_utf8_str
{
	lq_byte_t*  cstr;
	lq_uint32_t length;
	lq_uint32_t size;
} *lq_utf8_str_t;

