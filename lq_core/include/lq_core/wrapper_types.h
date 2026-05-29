#pragma once

#include "defines.h"
#include "types.h"

// wrapper enum for litehtml::media_type
typedef enum lq_wrapper_media_type
{
	LQ_WRAPPER_MEDIA_TYPE_UNKNOWN,
	LQ_WRAPPER_MEDIA_TYPE_ALL,
	LQ_WRAPPER_MEDIA_TYPE_PRINT,
	LQ_WRAPPER_MEDIA_TYPE_SCREEN,
	LQ_WRAPPER_MEDIA_TYPE_FIRST_DEPRECATED
} lq_wrapper_media_type;

// wrapper struct for litehtml::media_features
typedef struct lq_wrapper_media_features
{
	lq_wrapper_media_type type;
	lq_pixel_t width; // (pixels) For continuous media, this is the width of the viewport including the size of a rendered scroll bar (if any). For paged media, this is the width of the page box.
	lq_pixel_t height; // (pixels) The height of the targeted display area of the output device. For continuous media, this is the height of the viewport including the size of a rendered scroll bar (if any). For paged media, this is the height of the page box.
	lq_pixel_t device_width; // (pixels) The width of the rendering surface of the output device. For continuous media, this is the width of the screen. For paged media, this is the width of the page sheet size.
	lq_pixel_t device_height; // (pixels) The height of the rendering surface of the output device. For continuous media, this is the height of the screen. For paged media, this is the height of the page sheet size.
	int color; // The number of bits per color component of the output device. If the device is not a color device, the value is zero.
	int color_index; // The number of entries in the color lookup table of the output device. If the device does not use a color lookup table, the value is zero.
	int monochrome; // The number of bits per pixel in a monochrome frame buffer. If the device is not a monochrome device, the output device value will be 0.
	lq_pixel_t resolution; // The resolution of the output device (in DPI)
} lq_wrapper_media_features_t;

// wrapper enum for litehtml::font_style
typedef enum lq_wrapper_font_style
{
	LQ_WRAPPER_FONT_STYLE_NORMAL,
	LQ_WRAPPER_FONT_STYLE_ITALIC
} lq_wrapper_font_style;

// wrapper enum for litehtml::css_units
typedef enum lq_wrapper_css_length_units
{
	LQ_WRAPPER_CSS_LENGTH_UNITS_NONE,
	LQ_WRAPPER_CSS_LENGTH_UNITS_PERCENTAGE,
	LQ_WRAPPER_CSS_LENGTH_UNITS_IN,
	LQ_WRAPPER_CSS_LENGTH_UNITS_CM,
	LQ_WRAPPER_CSS_LENGTH_UNITS_MM,
	LQ_WRAPPER_CSS_LENGTH_UNITS_EM,
	LQ_WRAPPER_CSS_LENGTH_UNITS_EX,
	LQ_WRAPPER_CSS_LENGTH_UNITS_PT,
	LQ_WRAPPER_CSS_LENGTH_UNITS_PC,
	LQ_WRAPPER_CSS_LENGTH_UNITS_PX,
	LQ_WRAPPER_CSS_LENGTH_UNITS_VW,
	LQ_WRAPPER_CSS_LENGTH_UNITS_VH,
	LQ_WRAPPER_CSS_LENGTH_UNITS_VMIN,
	LQ_WRAPPER_CSS_LENGTH_UNITS_VMAX,
	LQ_WRAPPER_CSS_LENGTH_UNITS_REM,
	LQ_WRAPPER_CSS_LENGTH_UNITS_CH,
} lq_wrapper_css_length_units;

// wrapper struct for litehtml::css_length
typedef struct lq_wrapper_css_length
{
	union
	{
		lq_float_t  value;
		lq_uint32_t predef;
	};
	lq_wrapper_css_length_units units;
	bool is_predefined;
} lq_wrapper_css_length_t;

static inline lq_wrapper_css_length_t lq_wrapper_css_length_create(void)
{
	lq_wrapper_css_length_t len;
	len.value = 0;
	len.units = LQ_WRAPPER_CSS_LENGTH_UNITS_NONE;
	len.is_predefined = false;
	return len;
}

static inline lq_wrapper_css_length_t lq_wrapper_css_length_create_value(lq_float_t value, lq_wrapper_css_length_units units)
{
	lq_wrapper_css_length_t len;
	len.value = value;
	len.units = units;
	len.is_predefined = false;
	return len;
}

static inline lq_wrapper_css_length_t lq_wrapper_css_length_create_predef(lq_uint32_t predef)
{
	lq_wrapper_css_length_t len;
	len.predef = predef;
	len.units = LQ_WRAPPER_CSS_LENGTH_UNITS_NONE;
	len.is_predefined = true;
	return len;
}

static inline void lq_wrapper_css_length_set_value(lq_wrapper_css_length_t* out, lq_float_t value, lq_wrapper_css_length_units units)
{
	LQ_DEBUG_ASSERT(out != nullptr, "Output length must not be null.");
	out->value = value;
	out->units = units;
	out->is_predefined = false;
}

static inline lq_float_t lq_wrapper_css_length_get_value(const lq_wrapper_css_length_t* in)
{
	LQ_DEBUG_ASSERT(in != nullptr, "Input length must not be null.");
	LQ_DEBUG_ASSERT(in->is_predefined == false, "Input length must not be a predefined value.");
	if (in->is_predefined == false)
	{
		return in->value;
	}
	return 0;
}

static inline lq_wrapper_css_length_units lq_wrapper_css_length_get_units(const lq_wrapper_css_length_t* in)
{
	LQ_DEBUG_ASSERT(in != nullptr, "Input length must not be null.");
	LQ_DEBUG_ASSERT(in->is_predefined == false, "Input length must not be a predefined value.");
	if (in->is_predefined == false)
	{
		return in->units;
	}
	return LQ_WRAPPER_CSS_LENGTH_UNITS_NONE;
}

static inline void lq_wrapper_css_length_set_predef(lq_wrapper_css_length_t* out, lq_uint32_t predef)
{
	LQ_DEBUG_ASSERT(out != nullptr, "Output length must not be null.");
	out->predef = predef;
	out->units = LQ_WRAPPER_CSS_LENGTH_UNITS_NONE;
	out->is_predefined = true;
}

static inline lq_uint32_t lq_wrapper_css_length_get_predef(const lq_wrapper_css_length_t* in)
{
	LQ_DEBUG_ASSERT(in != nullptr, "Input length must not be null.");
	LQ_DEBUG_ASSERT(in->is_predefined == true, "Input length must be a predefined value.");
	if (in->is_predefined)
	{
		return in->predef;
	}
	return 0;
}

static inline lq_bool_t lq_wrapper_css_length_is_predef(const lq_wrapper_css_length_t* in) { return in->is_predefined; }

static inline lq_pixel_t lq_wrapper_css_length_calc_percent(const lq_wrapper_css_length_t* in, lq_pixel_t width)
{
	LQ_DEBUG_ASSERT(in != nullptr, "Input length must not be null.");
	LQ_DEBUG_ASSERT(in->is_predefined == false, "Input length must not be a predefined value.");
	if (in->is_predefined == false)
	{
		if (in->units == LQ_WRAPPER_CSS_LENGTH_UNITS_PERCENTAGE)
		{
			return (lq_pixel_t)(width * in->value / 100.0f);
		}
		else
		{
			return (lq_pixel_t)in->value;
		}
	}
	return 0;
}

static inline lq_bool_t lq_wrapper_css_length_equal(const lq_wrapper_css_length_t* a, const lq_wrapper_css_length_t* b)
{
	LQ_DEBUG_ASSERT(a != nullptr, "Input length a must not be null.");
	LQ_DEBUG_ASSERT(b != nullptr, "Input length b must not be null.");
	if (a->is_predefined != b->is_predefined)
	{
		return lq_false;
	}
	return a->is_predefined ? (a->predef == b->predef) : (a->units == b->units && a->value == b->value);
}

// wrapper enum for litehtml::text_decoration_line
typedef enum lq_wrapper_text_decoration_line
{
	LQ_WRAPPER_TEXT_DECORATION_LINE_NONE         = 0x00,
	LQ_WRAPPER_TEXT_DECORATION_LINE_UNDERLINE    = 0x01,
	LQ_WRAPPER_TEXT_DECORATION_LINE_OVERLINE     = 0x02,
	LQ_WRAPPER_TEXT_DECORATION_LINE_LINE_THROUGH = 0x04,
} lq_wrapper_text_decoration_line;

// wrapper enum for litehtml::text_decoration_style
typedef enum lq_wrapper_text_decoration_style
{
	LQ_WRAPPER_TEXT_DECORATION_STYLE_SOLID,
	LQ_WRAPPER_TEXT_DECORATION_STYLE_DOUBLE,
	LQ_WRAPPER_TEXT_DECORATION_STYLE_DOTTED,
	LQ_WRAPPER_TEXT_DECORATION_STYLE_DASHED,
	LQ_WRAPPER_TEXT_DECORATION_STYLE_WAVY,
	LQ_WRAPPER_TEXT_DECORATION_STYLE_MAX,
} lq_wrapper_text_decoration_style;

// wrapper enum for litehtml::text_emphasis_position
typedef enum lq_wrapper_text_emphasis_position
{
	LQ_WRAPPER_TEXT_EMPHASIS_POSITION_OVER  = 0x00,
	LQ_WRAPPER_TEXT_EMPHASIS_POSITION_UNDER = 0x01,
	LQ_WRAPPER_TEXT_EMPHASIS_POSITION_LEFT  = 0x02,
	LQ_WRAPPER_TEXT_EMPHASIS_POSITION_RIGHT = 0x04,
} lq_wrapper_text_emphasis_position;

// wrapper struct for litehtml::font_description
typedef struct lq_wrapper_font_description
{
	const lq_byte_t*      utf8_family; // Font Family
	lq_pixel_t            size;        // Font size
	lq_wrapper_font_style style;       // Font style, see the enum lq_font_style
	lq_uint32_t           weight;      // Font weight.
	lq_uint32_t                      decoration_line;      // Decoration line. A bitset of flags of the enum lq_wrapper_text_decoration_line
	lq_wrapper_css_length_t          decoration_thickness; // Decoration line thickness in pixels. See predefined values in enum lq_wrapper_css_length_units
	lq_wrapper_text_decoration_style decoration_style;     // Decoration line style. See enum lq_wrapper_text_decoration_style
	lq_color_t                       decoration_color;     // Decoration line color
	const lq_byte_t* utf8_emphasis_style; // Text emphasis style
	lq_color_t       emphasis_color;      // Text emphasis color
	lq_int32_t       emphasis_position;   // Text emphasis position, a bitset of flags of the enum lq_wrapper_text_emphasis_position
} lq_wrapper_font_description_t;

// wrapper struct for litehtml::font_metrics
typedef struct lq_wrapper_font_metrics
{
	lq_pixel_t font_size   = 0;       // Font size in pixels. The same as size argument of the create_font function
	lq_pixel_t height      = 0;       // Font height in pixels.
	lq_pixel_t ascent      = 0;       // The distance from the baseline to the top of a line of text.
	lq_pixel_t descent     = 0;       // The distance from the baseline to the bottom of a line of text.
	lq_pixel_t x_height    = 0;       // Height of the symbol x
	lq_pixel_t ch_width    = 0;       // Width of the symbol 0
	lq_bool_t  draw_spaces = lq_true; // True to call draw text function for spaces. If False, just use space width without draw.
	lq_pixel_t sub_shift   = 0;   // The baseline shift for subscripts.
	lq_pixel_t super_shift = 0; // The baseline shift for superscripts.
} lq_wrapper_font_metrics_t;

static inline lq_pixel_t lq_wrapper_font_metrics_base_line(const lq_wrapper_font_metrics_t* metrics)
{
	LQ_DEBUG_ASSERT(metrics != nullptr, "Input font metrics must not be null.");
	return metrics->descent;
}

// wrapper enum for litehtml::render_type
typedef enum lq_wrapper_render_type
{
	LQ_WRAPPER_RENDER_TYPE_ALL,
	LQ_WRAPPER_RENDER_TYPE_NO_FIXED,
	LQ_WRAPPER_RENDER_TYPE_ONLY_FIXED,
} lq_wrapper_render_type_t;

// wrapper type for litehtml::document pointer
typedef lq_uintptr_t lq_wrapper_document_t;