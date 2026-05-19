#pragma once

#include "defines.h"

typedef char    lq_char_t;
typedef wchar_t lq_wchar_t;

typedef void* lq_ptr_t;

#if defined(_WIN32) || defined(_WIN64)
	#if defined(LQ_STATIC)
		#define LQ_CORE_API
	#elif defined(LQ_CORE_EXPORTS)
		#define LQ_CORE_API __declspec(dllexport)
	#else
		#define LQ_CORE_API __declspec(dllimport)
	#endif

	typedef __int8 lq_bool_t;
	#define lq_false 0
	#define lq_true  1

	typedef unsigned __int8 lq_byte_t;

	typedef __int8  lq_int8_t;
	typedef __int16 lq_int16_t;
	typedef __int32 lq_int32_t;
	typedef __int64 lq_int64_t;

	typedef unsigned __int8  lq_uint8_t;
	typedef unsigned __int16 lq_uint16_t;
	typedef unsigned __int32 lq_uint32_t;
	typedef unsigned __int64 lq_uint64_t;

	#if defined(_WIN64)
		typedef unsigned __int64 lq_uintptr_t;
	#else
		typedef unsigned __int32 lq_uintptr_t;
	#endif

#else
	#define LQ_CORE_API
	#error "Must define platform specific types for non-Windows platforms."
#endif

typedef float lq_float_t;
typedef float lq_pixel_t;

typedef struct lq_pixel2
{
	lq_pixel_t x;
	lq_pixel_t y;
} lq_pixel2_t;

LQ_CORE_API static inline lq_pixel2_t lq_pixel2_create(lq_pixel_t x, lq_pixel_t y)
{
	lq_pixel2_t vec;
	vec.x = x;
	vec.y = y;
	return vec;
}

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