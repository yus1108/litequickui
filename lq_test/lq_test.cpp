#include "lq_test.h"

#include <iostream>

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

#include <Windows.h>

int main()
{
	DETECT_MEMORY_LEAKS(-1);

	WCHAR localeName[LOCALE_NAME_MAX_LENGTH];
	GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH);
	LANGID langId = GetUserDefaultUILanguage();
	WCHAR langCode[9];
	GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT, LOCALE_SISO639LANGNAME, langCode, 9);
	lq_debug_print_format_wstring(LQ_WTEXT(__FILE__), (unsigned int)(__LINE__), L"User default locale: %s, UI language: %u, ISO 639 language code: %s", localeName, langId, langCode);

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
	std::cout << "Testing lq_document with HTML: " << html_cstr << std::endl;

	lq_utf8_str_t html_utf8 = lq_utf8_str_create(html_cstr);
	lq_document_callbacks_t callbacks = {};
	callbacks.calc_text_width = [](const lq_byte_t* raw_utf8_text, lq_uintptr_t font_handle, lq_uintptr_t user_data) -> lq_pixel_t
		{
			lq_uint32_t length;
			lq_inspect_raw_utf8(&length, nullptr, raw_utf8_text);
			return length * 10.0f; // Return a dummy width based on character count
		};
	callbacks.create_font = [](lq_wrapper_font_metrics_t* out_metrics, const lq_wrapper_font_description_t* font_desc, const lq_wrapper_document_t document) -> lq_uintptr_t
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
	callbacks.delete_font = [](lq_uintptr_t font_handle, lq_uintptr_t user_data)
		{
			// No actual font resource to clean up in this dummy implementation
		};
	callbacks.draw_text = [](lq_uintptr_t hdc, const lq_byte_t* raw_utf8_text, lq_uintptr_t font_handle, const lq_color_t* color, const lq_rect_t* quad, lq_uintptr_t user_data)
		{
			std::cout << "Drawing text: \"" << lq_cast_to_cstr(raw_utf8_text) << "\" at (" << quad->x << ", " << quad->y << ") with width " << quad->width << " and height " << quad->height << std::endl;
		};
	callbacks.get_default_font_name = [](lq_uintptr_t user_data) -> lq_utf8_str_t
		{
			user_data_t* userData = reinterpret_cast<user_data_t*>(user_data);
			return userData->default_font_name;
		};
	callbacks.get_default_font_size = [](lq_uintptr_t user_data) -> lq_pixel_t
		{
			return 16.0f;
		};
	callbacks.get_media_features = [](lq_wrapper_media_features_t* out_media, lq_uintptr_t user_data)
		{
			out_media->type = LQ_WRAPPER_MEDIA_TYPE_SCREEN;
			out_media->width = 1920.0f;
			out_media->height = 1080.0f;
			out_media->device_width = 1920.0f;
			out_media->device_height = 1080.0f;
			out_media->color = 24;
		};
	callbacks.get_viewport = [](lq_rect_t* out_viewport, lq_uintptr_t user_data)
		{
			out_viewport->x = 0.0f;
			out_viewport->y = 0.0f;
			out_viewport->width = 1920.0f;
			out_viewport->height = 1080.0f;
		};
	callbacks.set_caption = [](const lq_byte_t* raw_utf8_caption, lq_uintptr_t user_data)
		{
			std::cout << "Document caption set to: " << lq_cast_to_cstr(raw_utf8_caption) << std::endl;
		};
	user_data_t userData;
	userData.default_font_name = lq_utf8_str_create("Noto Sans");
	lq_document_t document = lq_document_create(html_utf8, &callbacks, reinterpret_cast<lq_uintptr_t>(&userData));

	lq_pixel_t layout_height = lq_document_calc_layout(document, 1920.0f, LQ_WRAPPER_RENDER_TYPE_ALL);
	std::cout << "Calculated layout height: " << layout_height << std::endl;

	lq_rect_t clip = lq_rect_create(0.0f, 0.0f, 1920.0f, 1080.0f);
	lq_document_draw(document, 0, 0.0f, 0.0f, &clip);

	lq_document_destroy(document);
	lq_utf8_str_destroy(html_utf8);
	lq_utf8_str_destroy(userData.default_font_name);
	return lq_true;
}