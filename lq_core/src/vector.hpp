#pragma once
#include "lq_core/vector.h"

typedef struct lq_vector
{
	lq_array_t  array;
	lq_uint32_t count;
}* lq_vector_t;