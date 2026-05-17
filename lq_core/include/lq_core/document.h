#pragma once

#include "types.h"
#include "utf8.h"
#include "wrapping_types.h"

typedef struct lq_document* lq_document_t;
typedef struct lq_document_callbacks
{
	lq_pixel_t(*calc_text_width)(lq_document_t document, const lq_utf8_str_t text, lq_uintptr_t font_handle);

	lq_uintptr_t(*create_font)(lq_document_t document, const lq_html_font_description_t* font_desc, lq_html_font_metrics_t* out_metrics);
	void		(*delete_font)(lq_document_t document, lq_uintptr_t font_handle);

	lq_utf8_str_t(*get_default_font_name)(lq_document_t document);
	lq_pixel_t   (*get_default_font_size)(lq_document_t document);
	void         (*get_media_features)(lq_document_t document, lq_html_media_features_t* out_media);

	void (*set_caption)(lq_document_t document, const lq_utf8_str_t caption);
} lq_document_callbacks_t;

lq_document_t lq_document_create(lq_utf8_str_t html_data, const lq_document_callbacks_t* callbacks, void* user_data);
void 	      lq_document_destroy(lq_document_t document);
void*         lq_document_get_user_data(lq_document_t document);