#pragma once
#include "lq_core/algorithms.h"

#include <vector>

typedef struct lq_core_algorithm_skyline_node
{
	lq_uint16x2_t pos;
	lq_uint16_t   width;
} lq_core_algorithm_skyline_node_t;

typedef struct lq_core_algorithm_skyline
{
	std::vector<lq_core_algorithm_skyline_node_t> nodes;
	lq_uint16x2_t size;
	lq_uint16_t   padding;
}* lq_core_algorithm_skyline_t;

lq_bool_t lq_core_algorithm_skyline_find_fit(const lq_core_algorithm_skyline_t skyline, lq_uint16_t* out_y, lq_uint32_t index, lq_uint16x2_t size);
void      lq_core_algorithm_skyline_insert(lq_core_algorithm_skyline_t skyline, lq_uint32_t index, lq_uint16x2_t pos, lq_uint16x2_t size);
void      lq_core_algorithm_skyline_merge(lq_core_algorithm_skyline_t skyline);