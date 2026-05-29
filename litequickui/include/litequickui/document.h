#pragma once
#include "defines.h"
#include "font_register.h"

#include <lq_core/types.h>
#include <lq_core/string.h>
#include <lq_core/wrapper_types.h>

typedef struct lq_document* lq_document_t;

typedef struct lq_document_callbacks
{
	lq_utf8_str_t(*get_default_font_name)(lq_uintptr_t user_data);
	lq_pixel_t   (*get_default_font_size)(lq_uintptr_t user_data);
	void         (*get_media_features)(lq_wrapper_media_features_t* out_media, lq_uintptr_t user_data);
	void         (*get_viewport)(lq_rect_t* out_viewport, lq_uintptr_t user_data);
	void         (*set_caption)(const lq_byte_t* utf8_caption, lq_uintptr_t user_data);
} lq_document_callbacks_t;

typedef struct lq_document_description
{
	lq_utf8_str_t html_data;
	lq_document_callbacks_t      callbacks;
	lq_font_register_interface_t font_register;
	lq_uintptr_t user_data;
} lq_document_description_t;

LQ_API lq_document_t lq_document_create(lq_document_description* description);
LQ_API void          lq_document_destroy(lq_document_t document);

LQ_API lq_uintptr_t lq_document_get_user_data(const lq_document_t document);

LQ_API lq_pixel_t lq_document_calc_layout(lq_document_t document, lq_pixel_t max_width, lq_wrapper_render_type render_type);
LQ_API void       lq_document_draw(lq_document_t document, lq_uintptr_t hdc, lq_pixel2_t pos, const lq_rect_t* clip);