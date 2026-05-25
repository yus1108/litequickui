#pragma once

#include "types.h"
#include "string.h"
#include "wrapper_types.h"

typedef struct lq_core_document *lq_core_document_t;

// wrapper callbacks of litehtml::document_container virtual functions 
// to avoid exposing litehtml types in the public API of lq_core. 
// The document container will call these callbacks to perform necessary operations, 
// which are required by the litehtml library for rendering the document.
typedef struct lq_core_document_callbacks
{
	lq_pixel_t(*calc_text_width)(const lq_byte_t* utf8_text, lq_uintptr_t font_handle, lq_uintptr_t user_data);

	lq_uintptr_t(*create_font)(lq_wrapper_font_metrics_t* out_metrics, const lq_wrapper_font_description_t* font_desc, const lq_wrapper_document_t document);
	void        (*delete_font)(lq_uintptr_t font_handle, lq_uintptr_t user_data);

	void (*draw_text)(lq_uintptr_t hdc, const lq_byte_t* utf8_text, lq_uintptr_t font_handle, const lq_color_t* color, const lq_rect_t* quad, lq_uintptr_t user_data);

	lq_utf8_str_t(*get_default_font_name)(lq_uintptr_t user_data);
	lq_pixel_t   (*get_default_font_size)(lq_uintptr_t user_data);
	void         (*get_media_features)(lq_wrapper_media_features_t* out_media, lq_uintptr_t user_data);
	void         (*get_viewport)(lq_rect_t* out_viewport, lq_uintptr_t user_data);

	void (*set_caption)(const lq_byte_t* utf8_caption, lq_uintptr_t user_data);
} lq_core_document_callbacks_t;

LQ_CORE_API lq_core_document_t lq_core_document_create(const lq_utf8_str_t html_data, const lq_core_document_callbacks_t* callbacks, lq_uintptr_t user_data);
LQ_CORE_API void 	           lq_core_document_destroy(lq_core_document_t document);

LQ_CORE_API lq_uintptr_t lq_core_document_get_user_data(const lq_core_document_t document);

LQ_CORE_API lq_pixel_t lq_core_document_calc_layout(lq_core_document_t document, lq_pixel_t max_width, lq_wrapper_render_type render_type);
LQ_CORE_API void       lq_core_document_draw(lq_core_document_t document, lq_uintptr_t hdc, lq_pixel2_t pos, const lq_rect_t* clip);