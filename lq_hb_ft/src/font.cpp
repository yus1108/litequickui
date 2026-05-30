#include "font.hpp"

#include "lq_hb_ft/defines.h"

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

lq_pixel_t lq_hb_ft_font_get_height(lq_hb_ft_font_t font)
{
	LQ_DEBUG_ASSERT(font != NULL, "font must not be NULL");
	LQ_DEBUG_ASSERT(font->face != NULL, "font face must not be NULL");
	return font->face->size->metrics.height / 64.0f;
}

lq_pixel_t lq_hb_ft_font_get_ascender(lq_hb_ft_font_t font)
{
	LQ_DEBUG_ASSERT(font != NULL, "font must not be NULL");
	LQ_DEBUG_ASSERT(font->face != NULL, "font face must not be NULL");
	return font->face->size->metrics.ascender / 64.0f;
}

lq_pixel_t lq_hb_ft_font_get_descender(lq_hb_ft_font_t font)
{
	LQ_DEBUG_ASSERT(font != NULL, "font must not be NULL");
	LQ_DEBUG_ASSERT(font->face != NULL, "font face must not be NULL");
	return font->face->size->metrics.descender / 64.0f;
}

lq_pixel_t lq_hb_ft_font_get_x_height(lq_hb_ft_font_t font)
{
	LQ_DEBUG_ASSERT(font != NULL, "font must not be NULL");
	LQ_DEBUG_ASSERT(font->face != NULL, "font face must not be NULL");

	FT_UInt x_index = FT_Get_Char_Index(font->face, 'x');
	if (x_index == 0)
	{
		LQ_DEBUG_ASSERT(false, "Font does not contain an 'x' glyph, cannot determine x-height. Using font height instead.");
		return lq_hb_ft_font_get_height(font);
	}

	FT_Load_Glyph(font->face, x_index, FT_LOAD_TARGET_NORMAL | FT_LOAD_RENDER);
	return font->face->glyph->metrics.height / 64.0f;
}

lq_pixel_t lq_hb_ft_font_get_0_height(lq_hb_ft_font_t font)
{
	LQ_DEBUG_ASSERT(font != NULL, "font must not be NULL");
	LQ_DEBUG_ASSERT(font->face != NULL, "font face must not be NULL");

	FT_UInt zero_index = FT_Get_Char_Index(font->face, '0');
	if (zero_index == 0)
	{
		LQ_DEBUG_ASSERT(false, "Font does not contain a '0' glyph, cannot determine 0-height. Using font height instead.");
		return lq_hb_ft_font_get_height(font);
	}

	FT_Load_Glyph(font->face, zero_index, FT_LOAD_TARGET_NORMAL | FT_LOAD_RENDER);
	return font->face->glyph->metrics.height / 64.0f;
}

lq_pixel_t lq_hb_ft_font_calc_text_width(lq_hb_ft_font_t font, const lq_byte_t* utf8_bytes)
{
	LQ_DEBUG_ASSERT(lq_inspect_utf8_bytes(NULL, NULL, utf8_bytes), "utf8_bytes must be a valid UTF-8 string");

	hb_buffer_t* buf = hb_buffer_create();
	hb_buffer_add_utf8(buf, (const char*)utf8_bytes, -1, 0, -1);
	hb_buffer_guess_segment_properties(buf);
	hb_shape(font->hb_font, buf, nullptr, 0);

	lq_uint32_t count;
	hb_glyph_position_t* pos = hb_buffer_get_glyph_positions(buf, &count);

	lq_int32_t width = 0;
	for (lq_uint32_t i = 0; i < count; i++)
	{
		width += pos[i].x_advance; // 26.6 fixed point
	}
	hb_buffer_destroy(buf);
	return width / 64.0f;
}

lq_core_font_interface_t lq_hb_ft_font_bind(lq_hb_ft_font_t font)
{
	LQ_DEBUG_ASSERT(font != NULL, "font must not be NULL");
	LQ_DEBUG_ASSERT(font->face != NULL, "font face must not be NULL");
	LQ_DEBUG_ASSERT(font->hb_font != NULL, "font hb_font must not be NULL");

	lq_core_font_interface_t font_interface;
	font_interface.ctx = (lq_uintptr_t)font;
	font_interface.get_height = lq_hb_ft_font_override_get_height;
	font_interface.get_ascender = lq_hb_ft_font_override_get_ascender;
	font_interface.get_descender = lq_hb_ft_font_override_get_descender;
	font_interface.get_x_height = lq_hb_ft_font_override_get_x_height;
	font_interface.get_0_height = lq_hb_ft_font_override_get_0_height;
	font_interface.calc_text_width = lq_hb_ft_font_override_calc_text_width;
	LQ_STATIC_ASSERT(sizeof(lq_core_font_interface_t) == 56, SIZE_MISMATCH);
	return font_interface;
}