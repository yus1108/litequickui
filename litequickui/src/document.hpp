#pragma once
#include "litequickui/document.h"

#if defined(LQ_TEXT_BACKEND_HB_FT)
	#include <lq_hb_ft.h>
#endif

typedef struct lq_document_data
{
	lq_font_register_interface_t font_register;
	lq_document_callbacks_t      callbacks;
	lq_uintptr_t user_data;
} lq_document_data_t;

typedef struct lq_document
{
	lq_core_document_t core;
	lq_core_document_callbacks_t callbacks;
	lq_document_data_t data;
}* lq_document_t;

static inline lq_pixel_t lq_document_override_calc_text_width
(
	const lq_byte_t* utf8_text,
	lq_uintptr_t font_handle,
	lq_uintptr_t data
);

static inline lq_uintptr_t lq_document_override_create_font
(
	lq_wrapper_font_metrics_t* out_metrics,
	const lq_wrapper_font_description_t* font_desc,
	const lq_wrapper_document_t litehtml_document,
	lq_uintptr_t data
);

static inline void lq_document_override_delete_font(lq_uintptr_t font_handle, lq_uintptr_t data);

static inline void lq_document_override_draw_text
(
	lq_uintptr_t hdc,
	const lq_byte_t* utf8_text,
	lq_uintptr_t font_handle,
	const lq_color_t* color,
	const lq_rect_t* quad,
	lq_uintptr_t data
);

static inline lq_utf8_str_t lq_document_override_get_default_font_name(lq_uintptr_t data)
{
	lq_document_data_t* doc_data = (lq_document_data_t*)data;
	LQ_DEBUG_ASSERT(doc_data->callbacks.get_default_font_name != nullptr, "get_default_font_name callback must be provided.");
	return doc_data->callbacks.get_default_font_name(doc_data->user_data);
}

static inline lq_pixel_t lq_document_override_get_default_font_size(lq_uintptr_t data)
{
	lq_document_data_t* doc_data = (lq_document_data_t*)data;
	LQ_DEBUG_ASSERT(doc_data->callbacks.get_default_font_size != nullptr, "get_default_font_size callback must be provided.");
	return doc_data->callbacks.get_default_font_size(doc_data->user_data);
}

static inline void lq_document_override_get_media_features(lq_wrapper_media_features_t* out_media, lq_uintptr_t data)
{
	lq_document_data_t* doc_data = (lq_document_data_t*)data;
	LQ_DEBUG_ASSERT(doc_data->callbacks.get_media_features != nullptr, "get_media_features callback must be provided.");
	doc_data->callbacks.get_media_features(out_media, doc_data->user_data);
}

static inline void lq_document_override_get_viewport(lq_rect_t* out_viewport, lq_uintptr_t data)
{
	lq_document_data_t* doc_data = (lq_document_data_t*)data;
	LQ_DEBUG_ASSERT(doc_data->callbacks.get_viewport != nullptr, "get_viewport callback must be provided.");
	doc_data->callbacks.get_viewport(out_viewport, doc_data->user_data);
}

static inline void lq_document_override_set_caption(const lq_byte_t* utf8_caption, lq_uintptr_t data)
{
	lq_document_data_t* doc_data = (lq_document_data_t*)data;
	LQ_DEBUG_ASSERT(doc_data->callbacks.set_caption != nullptr, "set_caption callback must be provided.");
	doc_data->callbacks.set_caption(utf8_caption, doc_data->user_data);
}