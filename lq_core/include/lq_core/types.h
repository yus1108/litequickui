#pragma once

#include "defines.h"

typedef float lq_float_t;
typedef float lq_pixel_t;

typedef struct lq_color
{
	union
	{
		struct
		{
			lq_byte_t r;
			lq_byte_t g;
			lq_byte_t b;
			lq_byte_t a;
		} comp;
		lq_uint32_t rgba;
	} u;
} lq_color_t;

LQ_CORE_API static inline lq_color_t lq_color_create(lq_byte_t r, lq_byte_t g, lq_byte_t b, lq_byte_t a)
{
	lq_color_t color;
	color.u.comp.r = r;
	color.u.comp.g = g;
	color.u.comp.b = b;
	color.u.comp.a = a;
	return color;
}

typedef struct lq_rect
{
	lq_pixel_t x;
	lq_pixel_t y;
	lq_pixel_t width;
	lq_pixel_t height;
} lq_rect_t;

LQ_CORE_API static inline lq_rect_t lq_rect_create(lq_pixel_t x, lq_pixel_t y, lq_pixel_t width, lq_pixel_t height)
{
	lq_rect_t rect;
	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;
	return rect;
}