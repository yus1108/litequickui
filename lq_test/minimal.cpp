#include "lq_test.h"

#include <iostream>

#include <lq_core.h>

lq_bool_t test_lq_core_doument_minimal_implementation(void)
{
	typedef struct user_data
	{
		lq_utf8_str_t default_font_name;
	} user_data_t;


	const lq_char_t* html_cstr = "<!DOCTYPE html><html><head><title>Test</title></head><body><h1>Hello CMake.</h1></body></html>";
	std::cout << "Testing lq_core_document with HTML: " << html_cstr << std::endl;

	lq_utf8_str_t html_utf8 = lq_utf8_str_create_cstr(html_cstr);
	lq_core_document_callbacks_t callbacks = {};
	callbacks.calc_text_width = [](const lq_byte_t* utf8_text, lq_uintptr_t font_handle, lq_uintptr_t user_data) -> lq_pixel_t
		{
			LQ_UNUSED(font_handle, user_data);
			lq_uint32_t length;
			lq_inspect_utf8_bytes(&length, nullptr, utf8_text);
			return length * 10.0f; // Return a dummy width based on character count
		};
	callbacks.create_font = [](lq_wrapper_font_metrics_t* out_metrics, const lq_wrapper_font_description_t* font_desc, const lq_wrapper_document_t document, lq_uintptr_t user_data) -> lq_uintptr_t
		{
			LQ_UNUSED(font_desc, document, user_data);
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
			LQ_UNUSED(font_handle, user_data);
			// No actual font resource to clean up in this dummy implementation
		};
	callbacks.draw_text = [](lq_uintptr_t hdc, const lq_byte_t* utf8_text, lq_uintptr_t font_handle, const lq_color_t* color, const lq_rect_t* quad, lq_uintptr_t user_data)
		{
			LQ_UNUSED(hdc, font_handle, color, user_data);
			std::cout << "Drawing text: \"" << lq_cast_to_cstr(utf8_text) << "\" at (" << quad->x << ", " << quad->y << ") with width " << quad->width << " and height " << quad->height << std::endl;
		};
	callbacks.get_default_font_name = [](lq_uintptr_t user_data) -> lq_utf8_str_t
		{
			user_data_t* userData = reinterpret_cast<user_data_t*>(user_data);
			return userData->default_font_name;
		};
	callbacks.get_default_font_size = [](lq_uintptr_t user_data) -> lq_pixel_t
		{
			LQ_UNUSED(user_data);
			return 16.0f;
		};
	callbacks.get_media_features = [](lq_wrapper_media_features_t* out_media, lq_uintptr_t user_data)
		{
			LQ_UNUSED(user_data);
			out_media->type = LQ_WRAPPER_MEDIA_TYPE_SCREEN;
			out_media->width = 1920.0f;
			out_media->height = 1080.0f;
			out_media->device_width = 1920.0f;
			out_media->device_height = 1080.0f;
			out_media->color = 24;
		};
	callbacks.get_viewport = [](lq_rect_t* out_viewport, lq_uintptr_t user_data)
		{
			LQ_UNUSED(user_data);
			out_viewport->x = 0.0f;
			out_viewport->y = 0.0f;
			out_viewport->width = 1920.0f;
			out_viewport->height = 1080.0f;
		};
	callbacks.set_caption = [](const lq_byte_t* utf8_caption, lq_uintptr_t user_data)
		{
			LQ_UNUSED(user_data);
			std::cout << "Document caption set to: " << lq_cast_to_cstr(utf8_caption) << std::endl;
		};
	user_data_t userData;
	userData.default_font_name = lq_utf8_str_create_cstr("Noto Sans");
	lq_core_document_t document = lq_core_document_create(html_utf8, &callbacks, reinterpret_cast<lq_uintptr_t>(&userData));

	lq_pixel_t layout_height = lq_core_document_calc_layout(document, 1920.0f, LQ_WRAPPER_RENDER_TYPE_ALL);
	std::cout << "Calculated layout height: " << layout_height << std::endl;

	lq_rect_t clip = lq_rect_create(0.0f, 0.0f, 1920.0f, 1080.0f);
	lq_core_document_draw(document, 0, lq_pixel2_create(0.f, 0.f), &clip);

	lq_core_document_destroy(document);
	lq_utf8_str_destroy(html_utf8);
	lq_utf8_str_destroy(userData.default_font_name);
	return lq_true;
}
