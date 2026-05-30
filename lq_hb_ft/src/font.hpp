#pragma once

#include <lq_core/interfaces.h>
#include <lq_core/string.h>
#include <lq_core/types.h>

#include <freetype/freetype.h>
#include <hb-ft.h>

typedef struct lq_hb_ft_font
{
	FT_Face    face;
	hb_font_t* hb_font;
}* lq_hb_ft_font_t;

void lq_hb_ft_font_init(lq_hb_ft_font_t font, FT_Library lib, const lq_utf8_str_t path, lq_uint32_t index, lq_pixel_t size);
void lq_hb_ft_font_deinit(lq_hb_ft_font_t font);

lq_pixel_t lq_hb_ft_font_get_height(lq_hb_ft_font_t font);
lq_pixel_t lq_hb_ft_font_get_ascender(lq_hb_ft_font_t font);
lq_pixel_t lq_hb_ft_font_get_descender(lq_hb_ft_font_t font);
lq_pixel_t lq_hb_ft_font_get_x_height(lq_hb_ft_font_t font);
lq_pixel_t lq_hb_ft_font_get_0_height(lq_hb_ft_font_t font);
lq_pixel_t lq_hb_ft_font_calc_text_width(lq_hb_ft_font_t font, const lq_byte_t* utf8_bytes);

lq_core_font_interface_t lq_hb_ft_font_bind(lq_hb_ft_font_t font);

static inline lq_pixel_t lq_hb_ft_font_override_get_height(lq_uintptr_t ctx)
{
	return lq_hb_ft_font_get_height((lq_hb_ft_font_t)ctx);
}

static inline lq_pixel_t lq_hb_ft_font_override_get_ascender(lq_uintptr_t ctx)
{
	return lq_hb_ft_font_get_ascender((lq_hb_ft_font_t)ctx);
}

static inline lq_pixel_t lq_hb_ft_font_override_get_descender(lq_uintptr_t ctx)
{
	return lq_hb_ft_font_get_descender((lq_hb_ft_font_t)ctx);
}

static inline lq_pixel_t lq_hb_ft_font_override_get_x_height(lq_uintptr_t ctx)
{
	return lq_hb_ft_font_get_x_height((lq_hb_ft_font_t)ctx);
}

static inline lq_pixel_t lq_hb_ft_font_override_get_0_height(lq_uintptr_t ctx)
{
	return lq_hb_ft_font_get_0_height((lq_hb_ft_font_t)ctx);
}

static inline lq_pixel_t lq_hb_ft_font_override_calc_text_width(lq_uintptr_t ctx, const lq_byte_t* utf8_bytes)
{
	return lq_hb_ft_font_calc_text_width((lq_hb_ft_font_t)ctx, utf8_bytes);
}