#include "litequickui/font_register.h"

#include "lq_hb_ft/font_register.h"

lq_font_register_interface_t lq_font_register_interface_create(lq_uint32_t family_cap, lq_uint32_t inst_cap)
{
#if defined(LQ_TEXT_BACKEND_HB_FT)
	return lq_hb_ft_font_register_create_and_bind(family_cap, inst_cap);
#else
	#error "No text backend is defined. Please define a text backend to create a font register interface."
	return lq_font_register_interface_t();
#endif
}

void lq_font_register_interface_destroy(lq_font_register_interface_t* font_register)
{
#if defined(LQ_TEXT_BACKEND_HB_FT)
	lq_hb_ft_font_register_destroy(font_register);
#else
	#error "No text backend is defined. Please define a text backend to destroy a font register interface."
#endif
}
