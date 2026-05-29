#include "litequickui/document.h"
#include "document.hpp"

#include <stdlib.h>

inline lq_pixel_t lq_document_override_calc_text_width(const lq_byte_t* utf8_text, lq_uintptr_t font_handle, lq_uintptr_t data)
{
	LQ_UNUSED(utf8_text);
	LQ_UNUSED(font_handle);
	LQ_UNUSED(data);

	LQ_DEBUG_ASSERT(lq_false, "lq_document_override_calc_text_width is not implemented yet. You should implement this function to calculate real text width based on the font metrics and the actual text content. The font_handle can be used to retrieve the font metrics of the font used for the text, and the utf8_text can be analyzed to calculate the total width of the text.");
	return lq_pixel_t();
}

inline lq_uintptr_t lq_document_override_create_font
(
	lq_wrapper_font_metrics_t* out_metrics,
	const lq_wrapper_font_description_t* font_desc,
	const lq_wrapper_document_t litehtml_document,
	lq_uintptr_t data
)
{
	LQ_UNUSED(out_metrics);
	LQ_UNUSED(font_desc);
	LQ_UNUSED(litehtml_document);
	LQ_UNUSED(data);

	lq_font_register_interface_t font_register = ((lq_document_data_t*)data)->font_register;
	LQ_DEBUG_ASSERT(lq_false, "lq_document_override_create_font is not implemented yet. You should implement this function to create real fonts based on the font description and return a valid font handle. The out_metrics should also be filled with the actual font metrics of the created font.");
	return lq_uintptr_t();
}

inline void lq_document_override_delete_font(lq_uintptr_t font_handle, lq_uintptr_t data)
{
	LQ_UNUSED(font_handle);
	LQ_UNUSED(data);

	LQ_DEBUG_ASSERT(lq_false, "lq_document_override_delete_font is not implemented yet. You should implement this function to delete real fonts created in the lq_document_override_create_font function based on the provided font handle.");
}

inline void lq_document_override_draw_text
(
	lq_uintptr_t hdc,
	const lq_byte_t* utf8_text,
	lq_uintptr_t font_handle,
	const lq_color_t* color,
	const lq_rect_t* quad,
	lq_uintptr_t data
)
{
	LQ_UNUSED(hdc);
	LQ_UNUSED(utf8_text);
	LQ_UNUSED(font_handle);
	LQ_UNUSED(color);
	LQ_UNUSED(quad);
	LQ_UNUSED(data);
	LQ_DEBUG_ASSERT(lq_false, "lq_document_override_draw_text is not implemented yet. You should implement this function to draw text based on the provided font handle and text content. The hdc can be used as a drawing context for rendering the text, and the color and quad parameters can be used to determine the color and position of the drawn text.");
}

lq_document_t lq_document_create(lq_document_description* description)
{
	LQ_DEBUG_ASSERT(lq_utf8_str_is_empty(description->html_data) == lq_false, "HTML data must not be empty.");
	LQ_DEBUG_ASSERT(description->callbacks.get_default_font_name != nullptr, "Callbacks must not be null.");

	lq_document_t document = (lq_document_t)malloc(sizeof(struct lq_document));
	LQ_DEBUG_ASSERT(document != nullptr, "Failed to allocate memory for document.");

	document->callbacks.calc_text_width = lq_document_override_calc_text_width;
	document->callbacks.create_font = lq_document_override_create_font;
	document->callbacks.delete_font = lq_document_override_delete_font;
	document->callbacks.draw_text = lq_document_override_draw_text;
	document->callbacks.get_default_font_name = lq_document_override_get_default_font_name;
	document->callbacks.get_default_font_size = lq_document_override_get_default_font_size;
	document->callbacks.get_media_features = lq_document_override_get_media_features;
	document->callbacks.get_viewport = lq_document_override_get_viewport;
	document->callbacks.set_caption = lq_document_override_set_caption;
	LQ_STATIC_ASSERT(sizeof(lq_core_document_callbacks_t) == 72, CORE_CALLBACK_STRUCT_SIZE_MISMATCH);

	document->data.callbacks.get_default_font_name = description->callbacks.get_default_font_name;
	document->data.callbacks.get_default_font_size = description->callbacks.get_default_font_size;
	document->data.callbacks.get_media_features = description->callbacks.get_media_features;
	document->data.callbacks.get_viewport = description->callbacks.get_viewport;
	document->data.callbacks.set_caption = description->callbacks.set_caption;
	LQ_STATIC_ASSERT(sizeof(lq_document_callbacks_t) == 40, CALLBACK_STRUCT_SIZE_MISMATCH);

	document->data.font_register = description->font_register;
	document->data.user_data = description->user_data;
	document->core = lq_core_document_create(description->html_data, &document->callbacks, (lq_uintptr_t)&document->data);
	return document;
}

void lq_document_destroy(lq_document_t document)
{
	lq_core_document_destroy(document->core);
}

lq_uintptr_t lq_document_get_user_data(const lq_document_t document)
{
	LQ_DEBUG_ASSERT(document != nullptr, "Input document must not be null.");
	lq_document_data_t* data = (lq_document_data_t*)lq_core_document_get_user_data(document->core);
	LQ_DEBUG_ASSERT(data != nullptr, "User data must not be null.");
	return data->user_data;
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
