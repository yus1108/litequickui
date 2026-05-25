#pragma once
#include "lq_hb_ft/font.h"

#include <freetype/freetype.h>
#include <hb-ft.h>

lq_core_font_interface_t lq_core_font_interface_bind_hb_ft(lq_hb_ft_font_t derived);

typedef struct lq_hb_ft_font
{
	FT_Face    face;
	hb_font_t* hb_font;
}* lq_hb_ft_font_t;

void lq_hb_ft_font_init(lq_hb_ft_font_t font, FT_Library lib, const lq_utf8_str_t path, lq_uint32_t index, lq_pixel_t size);
void lq_hb_ft_font_deinit(lq_hb_ft_font_t font);