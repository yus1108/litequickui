#include "litequickui/document.h"
#include "document.hpp"

#include <stdlib.h>

lq_document_t lq_document_create(const lq_utf8_str_t html_data, const lq_document_callbacks_t* callbacks, lq_uintptr_t user_data)
{
	LQ_DEBUG_ASSERT(lq_utf8_str_is_empty(html_data) == lq_false, "HTML data must not be empty.");
	LQ_DEBUG_ASSERT(callbacks != nullptr, "Callbacks must not be null.");

	lq_document_t document = (lq_document_t)malloc(sizeof(struct lq_document));
	LQ_DEBUG_ASSERT(document != nullptr, "Failed to allocate memory for document.");

	document->callbacks.calc_text_width = [](const lq_byte_t* utf8_text, lq_uintptr_t font_handle, lq_uintptr_t user_data) -> lq_pixel_t
		{
			LQ_UNUSED(font_handle);
			LQ_UNUSED(user_data);
			lq_uint32_t utf8_text_length, utf8_text_size;
			lq_inspect_utf8_bytes(&utf8_text_length, &utf8_text_size, utf8_text);
			LQ_ASSERT(lq_false, "TODO: Implement calc_text_width callback to calculate real text width based on the font metrics and the actual text content.");
			return static_cast<lq_pixel_t>(utf8_text_length * 8); // Assume each character is 8 pixels wide for testing
		};
	document->callbacks.create_font = [](lq_wrapper_font_metrics_t* out_metrics, const lq_wrapper_font_description_t* font_desc, const lq_wrapper_document_t document) -> lq_uintptr_t
		{
			LQ_UNUSED(document);
			LQ_UNUSED(font_desc);
			// For testing purposes, we can return a dummy font handle and metrics.
			out_metrics->ascent = 12.0f;
			out_metrics->descent = 4.0f;
			LQ_ASSERT(lq_false, "TODO: Implement create_font callback to create real fonts and calculate real font metrics based on the font description.");
			return 0; // Dummy font handle
		};
	document->callbacks.delete_font = [](lq_uintptr_t font_handle, lq_uintptr_t user_data)
		{
			LQ_UNUSED(font_handle);
			LQ_UNUSED(user_data);
			// For testing purposes, we don't need to do anything here since we are not actually creating real fonts.
			LQ_ASSERT(lq_false, "TODO: Implement delete_font callback to delete real fonts created in the create_font callback.");
		};
	document->callbacks.draw_text = [](lq_uintptr_t hdc, const lq_byte_t* utf8_text, lq_uintptr_t font_handle, const lq_color_t* color, const lq_rect_t* quad, lq_uintptr_t user_data)
		{
			LQ_UNUSED(hdc);
			LQ_UNUSED(utf8_text);
			LQ_UNUSED(font_handle);
			LQ_UNUSED(color);
			LQ_UNUSED(quad);
			LQ_UNUSED(user_data);
			// For testing purposes, we don't need to do anything here since we are not actually drawing real text.
			return void();
		};
	document->callbacks.get_default_font_name = callbacks->get_default_font_name;
	document->callbacks.get_default_font_size = callbacks->get_default_font_size;
	document->callbacks.get_media_features = callbacks->get_media_features;
	document->callbacks.get_viewport = callbacks->get_viewport;
	document->callbacks.set_caption = callbacks->set_caption;

	document->core = lq_core_document_create(html_data, &document->callbacks, user_data);
	return document;
}

void lq_document_destroy(lq_document_t document)
{
	lq_core_document_destroy(document->core);
}

lq_uintptr_t lq_document_get_user_data(const lq_document_t document)
{
	LQ_DEBUG_ASSERT(lq_false, "lq_document_get_user_data is not implemented yet.");
	return lq_uintptr_t();
}

lq_pixel_t lq_document_calc_layout(lq_document_t document, lq_pixel_t max_width, lq_wrapper_render_type render_type)
{
	LQ_DEBUG_ASSERT(lq_false, "lq_document_calc_layout is not implemented yet.");
	return lq_pixel_t();
}

void lq_document_draw(lq_document_t document, lq_uintptr_t hdc, lq_pixel2_t pos, const lq_rect_t* clip)
{
	LQ_DEBUG_ASSERT(lq_false, "lq_document_draw is not implemented yet.");
	return void();
}
