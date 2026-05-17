#pragma once

#if defined(NULL)
	#if defined(__cplusplus)
		#define NULL 0
	#else
		#define NULL ((void *)0)
	#endif
#endif

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

	typedef float lq_pixel_t;
#else
	#define LQ_CORE_API
	#error "Must define platform specific types for non-Windows platforms."
#endif

// wrapper enum for litehtml::media_type
typedef enum lq_media_type
{
	media_type_unknown,
	media_type_all,
	media_type_print,
	media_type_screen,
	media_type_first_deprecated
} lq_media_type_t;

// wrapper struct for litehtml::media_features
typedef struct lq_media_features
{
	lq_media_type_t type;
	lq_pixel_t width;			// (pixels) For continuous media, this is the width of the viewport including the size of a rendered scroll bar (if any). For paged media, this is the width of the page box.
	lq_pixel_t height;			// (pixels) The height of the targeted display area of the output device. For continuous media, this is the height of the viewport including the size of a rendered scroll bar (if any). For paged media, this is the height of the page box.
	lq_pixel_t device_width;	// (pixels) The width of the rendering surface of the output device. For continuous media, this is the width of the screen. For paged media, this is the width of the page sheet size.
	lq_pixel_t device_height;	// (pixels) The height of the rendering surface of the output device. For continuous media, this is the height of the screen. For paged media, this is the height of the page sheet size.
	int color;			// The number of bits per color component of the output device. If the device is not a color device, the value is zero.
	int color_index;	// The number of entries in the color lookup table of the output device. If the device does not use a color lookup table, the value is zero.
	int monochrome;		// The number of bits per pixel in a monochrome frame buffer. If the device is not a monochrome device, the output device value will be 0.
	lq_pixel_t resolution;		// The resolution of the output device (in DPI)
} lq_media_features_t;