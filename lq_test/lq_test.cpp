#include "lq_test.h"

#include <lq_core.h>

#if defined(_DEBUG)
	#if defined(_WIN32)

		#define DETECT_MEMORY_LEAKS(goToLine) win_detect_memory_leak(goToLine)

		#define _CRTDBG_MAP_ALLOC
		#include <cstdlib>
		#include <crtdbg.h>

		void win_detect_memory_leak(int goToLine)
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
			_CrtSetBreakAlloc(goToLine);
		}
	#else
		#define DETECT_MEMORY_LEAKS(goToLine)
	#endif
#else
	#define DETECT_MEMORY_LEAKS(goToLine)
#endif // _WIN32

int main()
{
	DETECT_MEMORY_LEAKS(-1);

	lq_bool_t result = test_lq_document_minimal_implementation();
	if (result == lq_false)
	{
		LQ_ASSERT(false, "lq_document minimal implementation test failed");
		return -1;
	}

	return 0;
}

lq_bool_t test_lq_document_minimal_implementation()
{
	typedef struct user_data
	{
		lq_utf8_str_t default_font_name;
	} user_data_t;


	const lq_char_t* html_cstr = "<!DOCTYPE html><html><head><title>Test</title></head><body><h1>Hello CMake.</h1></body></html>";
	lq_debug_print_format_string(__FILE__, __LINE__, "Testing lq_document with HTML: %s", html_cstr);

	lq_utf8_str_t html_utf8 = lq_utf8_str_create(html_cstr);
	lq_document_callbacks_t callbacks = {};
	callbacks.calc_text_width = [](lq_document_t document, const lq_byte_t* raw_utf8_text, lq_uintptr_t font_handle) -> lq_pixel_t
		{
			lq_uint32_t length;
			lq_inspect_raw_utf8(&length, nullptr, raw_utf8_text);
			return length * 10.0f; // Return a dummy width based on character count
		};
	callbacks.create_font = [](lq_document_t document, const lq_html_font_description_t* font_desc, lq_html_font_metrics_t* out_metrics) -> lq_uintptr_t
		{
			out_metrics->font_size = font_desc->size;
			out_metrics->height = font_desc->size * 1.2f;
			out_metrics->ascent = font_desc->size * 0.8f;
			out_metrics->descent = font_desc->size * 0.4f;
			out_metrics->x_height = font_desc->size * 0.5f;
			out_metrics->ch_width = font_desc->size * 0.6f;
			out_metrics->sub_shift = font_desc->size * 0.2f;
			out_metrics->super_shift = font_desc->size * 0.3f;
			return 1; // Return a dummy non-zero font handle
		};
	callbacks.delete_font = [](lq_document_t document, lq_uintptr_t font_handle)
		{
			// No actual font resource to clean up in this dummy implementation
		};
	callbacks.get_default_font_name = [](lq_document_t document) -> lq_utf8_str_t
		{
			user_data_t* userData = static_cast<user_data_t*>(lq_document_get_user_data(document));
			return userData->default_font_name;
		};
	callbacks.get_default_font_size = [](lq_document_t document) -> lq_pixel_t
		{
			return 16.0f;
		};
	callbacks.get_media_features = [](lq_document_t document, lq_html_media_features_t* out_media)
		{
			out_media->type = LQ_HTML_MEDIA_TYPE_SCREEN;
			out_media->width = 1920.0f;
			out_media->height = 1080.0f;
			out_media->device_width = 1920.0f;
			out_media->device_height = 1080.0f;
			out_media->color = 24;
		};
	callbacks.set_caption = [](lq_document_t document, const lq_byte_t* raw_utf8_caption)
		{
			lq_debug_print_format_string(__FILE__, __LINE__, "Document caption set to: %s", raw_utf8_caption);
		};
	user_data_t userData;
	userData.default_font_name = lq_utf8_str_create("Noto Sans");
	lq_document_t document = lq_document_create(html_utf8, &callbacks, &userData);

	lq_document_destroy(document);
	lq_utf8_str_destroy(html_utf8);
	lq_utf8_str_destroy(userData.default_font_name);
	return lq_true;
}