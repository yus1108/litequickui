#pragma once
#include <lq_core.h>
#if defined(LQ_TEXT_BACKEND_HB_FT)
#include <lq_hb_ft.h>
#endif

#include "litequickui/defines.h"

typedef struct lq_document* lq_document_t;

typedef struct lq_document_callbacks
{
	lq_utf8_str_t(*get_default_font_name)(lq_uintptr_t user_data);
	lq_pixel_t   (*get_default_font_size)(lq_uintptr_t user_data);
	void         (*get_media_features)(lq_wrapper_media_features_t* out_media, lq_uintptr_t user_data);
	void         (*get_viewport)(lq_rect_t* out_viewport, lq_uintptr_t user_data);
	void         (*set_caption)(const lq_byte_t* utf8_caption, lq_uintptr_t user_data);
} lq_document_callbacks_t;

LQ_API lq_document_t lq_document_create(const lq_utf8_str_t html_data, const lq_document_callbacks_t* callbacks, lq_uintptr_t user_data);
LQ_API void          lq_document_destroy(lq_document_t document);

LQ_API lq_uintptr_t lq_document_get_user_data(const lq_document_t document);

LQ_API lq_pixel_t lq_document_calc_layout(lq_document_t document, lq_pixel_t max_width, lq_wrapper_render_type render_type);
LQ_API void       lq_document_draw(lq_document_t document, lq_uintptr_t hdc, lq_pixel2_t pos, const lq_rect_t* clip);