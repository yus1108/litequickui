#pragma once

#include "types.h"
#include "string.h"
#include "wrapper_types.h"

typedef struct lq_font_query
{
	lq_utf8_str_t family;
	lq_pixel_t    size;
	lq_wrapper_font_style style;
	lq_uint16_t weight;
} lq_font_query_t;

static inline lq_font_query_t lq_font_query_create(lq_utf8_str_t family, lq_pixel_t size, lq_wrapper_font_style style, lq_uint16_t weight)
{
	lq_font_query_t query;
	query.family = family;
	query.size = size;
	query.style = style;
	query.weight = weight;
	return query;
}

static inline lq_bool_t lq_font_query_equals(const void* a, const void* b)
{
	if (a == b) { return lq_true; }

	const lq_font_query* queryA = (const lq_font_query*)a;
	const lq_font_query* queryB = (const lq_font_query*)b;
	if (queryA->size != queryB->size || queryA->style != queryB->style || queryA->weight != queryB->weight)
	{
		return lq_false;
	}
	return lq_utf8_str_equals(queryA->family, queryB->family);
}