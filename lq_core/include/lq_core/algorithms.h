#pragma once

#include "defines.h"
#include "types.h"

static inline lq_uint32_t murmur3_32_finalize(lq_uint32_t x)
{
    x ^= x >> 16;
    x *= 0x85ebca6b;
    x ^= x >> 13;
    x *= 0xc2b2ae35;
    x ^= x >> 16;
    return x;
}

typedef struct lq_core_algorithm_skyline* lq_core_algorithm_skyline_t;

LQ_CORE_API lq_core_algorithm_skyline_t lq_core_algorithm_skyline_create(lq_uint16x2_t size, lq_uint16_t padding);
LQ_CORE_API void lq_core_algorithm_skyline_destroy(lq_core_algorithm_skyline_t skyline);

LQ_CORE_API lq_bool_t lq_core_algorithm_skyline_pack(lq_core_algorithm_skyline_t skyline, lq_uint16x2_t* out_position, lq_uint16x2_t size);