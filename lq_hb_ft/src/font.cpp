#include "lq_hb_ft/font.h"
#include "font.hpp"

#include <math.h>

void lq_hb_ft_font_init(lq_hb_ft_font_t font, FT_Library lib, const lq_utf8_str_t path, lq_uint32_t index, lq_pixel_t size)
{
	LQ_DEBUG_ASSERT(font != NULL, "font must not be NULL");
	LQ_DEBUG_ASSERT(font->face == NULL, "font face must be NULL");
	LQ_DEBUG_ASSERT(font->hb_font == NULL, "font hb_font must be NULL");
	LQ_DEBUG_ASSERT(lib != NULL, "lib must not be NULL");
	LQ_DEBUG_ASSERT(lq_utf8_str_is_valid(path), "path must be a valid UTF-8 string");
	LQ_DEBUG_ASSERT(size > 0, "size must be greater than 0");

	LQ_DEBUG_ONLY(FT_Error err = )FT_New_Face(lib, lq_utf8_str_get_cstr(path), index, &font->face);
	LQ_DEBUG_ASSERT(err == 0, "Failed to read face for %s", lq_utf8_str_get_cstr(path));
	FT_Set_Pixel_Sizes(font->face, 0, static_cast<FT_UInt>(roundf(size)));

	font->hb_font = hb_ft_font_create(font->face, NULL);
	hb_ft_font_set_funcs(font->hb_font);
}

void lq_hb_ft_font_deinit(lq_hb_ft_font_t font)
{
	LQ_DEBUG_ASSERT(font != NULL, "font must not be NULL");
	LQ_DEBUG_ASSERT(font->face != NULL, "font face must not be NULL");
	LQ_DEBUG_ASSERT(font->hb_font != NULL, "font hb_font must not be NULL");

	hb_font_destroy(font->hb_font);
	FT_Done_Face(font->face);
}

lq_core_font_interface_t lq_hb_ft_font_bind(lq_hb_ft_font_t font)
{
	LQ_DEBUG_ASSERT(font != NULL, "font must not be NULL");
	LQ_DEBUG_ASSERT(font->face != NULL, "font face must not be NULL");
	LQ_DEBUG_ASSERT(font->hb_font != NULL, "font hb_font must not be NULL");

	lq_core_font_interface_t font_interface;
	font_interface.ctx = (lq_uintptr_t)font;
	return font_interface;
}