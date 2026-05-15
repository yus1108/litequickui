#pragma once

#if defined(_WIN32) || defined(_WIN64)
	#if defined(LQ_STATIC)
		#define LQ_CORE_API
	#elif defined(LQ_CORE_EXPORTS)
		#define LQ_CORE_API __declspec(dllexport)
	#else
		#define LQ_CORE_API __declspec(dllimport)
	#endif

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

typedef struct lq_utf8* lq_utf8_t;
typedef struct lq_utf8_string* lq_utf8_string_t;

lq_utf8_string_t lq_utf8_string_create(const lq_byte_t* data, lq_uint32_t size);
void             lq_utf8_string_destroy(lq_utf8_string_t str);

typedef struct lq_document* lq_document_t;

typedef struct lq_document_callbacks 
{
	void (*draw_text)(void* user, const lq_byte_t* text, lq_pixel_t x, lq_pixel_t y); // TODO: temporary callback for simple implementation, need to be replaced by more complex one in the future
} lq_document_callbacks_t;

lq_document_t lq_document_create(const lq_byte_t* html_data, lq_uint32_t html_size, const lq_document_callbacks_t* callbacks);