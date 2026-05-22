#include "lq_hb_ft.h"

#include "font_register.hpp"

LQ_HB_FT_API lq_core_font_register_interface_t lq_core_font_register_interface_bind_hb_ft(lq_hb_ft_font_register_t derived)
{
	lq_core_font_register_interface_t font_register;
	font_register.ctx = (uintptr_t)derived;
	font_register.add = lq_core_font_register_bind_hb_ft_add_fn;
	font_register.find_or_create = lq_core_font_register_bind_hb_ft_find_or_create_fn;
	return font_register;
}