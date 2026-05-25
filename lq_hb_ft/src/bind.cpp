#include "lq_hb_ft/bind.h"
#include "bind.hpp"

static inline void lq_hb_ft_font_register_add_fn_bind(lq_uintptr_t ctx, const lq_utf8_str_t font_path, const lq_utf8_str_t opt_family)
{
	lq_hb_ft_font_register_add((lq_hb_ft_font_register_t)ctx, font_path, opt_family);
}

static inline lq_core_font_interface_t lq_hb_ft_font_register_find_or_create_fn_bind(lq_uintptr_t ctx, const lq_font_query_t* query)
{
	return lq_hb_ft_font_register_find_or_create((lq_hb_ft_font_register_t)ctx, query);
}

LQ_HB_FT_API void lq_hb_ft_font_register_bind_to(lq_core_font_register_interface_t* out_interface, lq_hb_ft_font_register_t derived)
{
	LQ_DEBUG_ASSERT(out_interface != NULL, "");
	LQ_DEBUG_ASSERT(derived != NULL, "");

	out_interface->ctx = (lq_uintptr_t)derived;
	out_interface->add = lq_hb_ft_font_register_add_fn_bind;
	out_interface->find_or_create = lq_hb_ft_font_register_find_or_create_fn_bind;

	return LQ_HB_FT_API void();
}

void lq_hb_ft_font_bind_to(lq_core_font_interface_t* out_interface, lq_hb_ft_font_t derived)
{
	LQ_DEBUG_ASSERT(out_interface != NULL, "");
	LQ_DEBUG_ASSERT(derived != NULL, "");

	out_interface->ctx = (lq_uintptr_t)derived;
}