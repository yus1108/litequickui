#pragma once
#include "lq_hb_ft/font.h"

#include <freetype/freetype.h>
#include <hb-ft.h>

typedef struct lq_hb_ft_font
{
	FT_Face    face;
	hb_font_t* hb_font;
}* lq_hb_ft_font_t;