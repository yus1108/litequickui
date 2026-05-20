#pragma once

#include "defines.h"
#include "types.h"

typedef struct lq_algorithm_skyline* lq_algorithm_skyline_t;

LQ_CORE_API lq_algorithm_skyline_t lq_algorithm_skyline_create(lq_uint16x2_t size, lq_uint16_t padding);
LQ_CORE_API void                   lq_algorithm_skyline_destroy(lq_algorithm_skyline_t skyline);

LQ_CORE_API lq_bool_t lq_algorithm_skyline_pack(lq_algorithm_skyline_t skyline, lq_uint16x2_t* out_position, lq_uint16x2_t size);