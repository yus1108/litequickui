#include "lq_hb_ft.h"

static inline void lq_core_font_register_bind_hb_ft_add_fn(lq_uintptr_t ctx, const lq_utf8_str_t font_path, const lq_utf8_str_t opt_family)
{
	lq_hb_ft_font_register_add((lq_hb_ft_font_register_t)ctx, font_path, opt_family);
}

static inline lq_core_font_interface_t lq_core_font_register_bind_hb_ft_find_or_create_fn(lq_uintptr_t ctx, const lq_font_query_t* query)
{
	return lq_hb_ft_font_register_find_or_create((lq_hb_ft_font_register_t)ctx, query);
}

LQ_HB_FT_API lq_core_font_register_interface_t lq_core_font_register_interface_bind_hb_ft(lq_hb_ft_font_register_t derived)
{
	lq_core_font_register_interface_t font_register;
	font_register.ctx = (lq_uintptr_t)derived;
	font_register.add = lq_core_font_register_bind_hb_ft_add_fn;
	font_register.find_or_create = lq_core_font_register_bind_hb_ft_find_or_create_fn;
	return font_register;
}