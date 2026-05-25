#include "litequickui/document.h"
#include "document.hpp"

lq_document_t lq_dochment_create(const lq_utf8_str_t html_data, const lq_document_callbacks_t* callbacks, lq_uintptr_t user_data)
{
	LQ_DEBUG_ASSERT(lq_false, "lq_document_create is not implemented yet.");
	return lq_document_t();
}

void lq_document_destroy(lq_document_t document)
{
	LQ_DEBUG_ASSERT(lq_false, "lq_document_destroy is not implemented yet.");
	return void();
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
