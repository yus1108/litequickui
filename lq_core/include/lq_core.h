#pragma once

#include "lq_core/type.h"

typedef struct lq_context*  lq_context_t;

lq_context_t lq_context_create(void);
void         lq_context_destroy(lq_context_t context);

typedef struct lq_utf8* lq_utf8_t;
typedef struct lq_utf8_string* lq_utf8_string_t;

lq_utf8_string_t lq_utf8_string_create(lq_context_t context, const lq_byte_t* data);
void             lq_utf8_string_destroy(lq_context_t context, lq_utf8_string_t str);
lq_uint32_t      lq_utf8_string_length(lq_utf8_string_t str);
lq_uint32_t      lq_utf8_string_size(lq_utf8_string_t str);

typedef struct lq_document* lq_document_t;
typedef struct lq_document_callbacks 
{
	lq_utf8_string_t(*get_default_font_name)(lq_context_t context, lq_document_t document);
	lq_pixel_t      (*get_default_font_size)(lq_context_t context, lq_document_t document);
	void            (*get_media_features)(lq_context_t context, lq_document_t document, lq_media_features_t* out_media);
} lq_document_callbacks_t;

lq_document_t lq_document_create(lq_context_t context, lq_utf8_string_t html_data, const lq_document_callbacks_t* callbacks, void* user_data);
void 	      lq_document_destroy(lq_context_t context, lq_document_t document);
void*         lq_document_get_user_data(lq_document_t document);