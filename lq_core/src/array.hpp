#pragma once
#include "lq_core/array.h"

typedef struct lq_array
{
	void* elements;
	lq_uint32_t element_size;
	lq_uint32_t element_count;
}*lq_array_t;