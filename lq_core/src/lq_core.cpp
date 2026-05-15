#include "lq_core.h"

#include <litehtml.h>

enum
{
	LQ_UINT8_MAX_SIZE = 4,
};

typedef struct lq_utf8
{
	lq_byte_t  bytes[LQ_UINT8_MAX_SIZE];
	lq_uint8_t size;
} *lq_utf8_t;

typedef struct lq_utf8_string
{
	lq_byte_t* data;
	lq_uint32_t size;
} *lq_utf8_string_t;


lq_utf8_string_t lq_utf8_string_create(const lq_byte_t* data, lq_uint32_t size)
{
	return nullptr;
}

void lq_utf8_string_destroy(lq_utf8_string_t str)
{

}

lq_document_t lq_document_create(const lq_byte_t* html_data, lq_uint32_t html_size, const lq_document_callbacks_t* callbacks)
{
	return nullptr;
}
