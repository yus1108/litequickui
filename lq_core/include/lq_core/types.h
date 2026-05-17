#pragma once

#if defined(NULL)
	#if defined(__cplusplus)
		#define NULL 0
	#else
		#define NULL ((void *)0)
	#endif
#endif

typedef char lq_char_t;

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
		};
		lq_uint32_t rgba;
	};
} lq_color;

static inline lq_color lq_color_create(lq_byte_t r, lq_byte_t g, lq_byte_t b, lq_byte_t a = 255)
{
	lq_color color;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	return color;
}