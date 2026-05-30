#pragma once

#include "types.h"
#include "string.h"
#include "font_query.h"

typedef struct lq_core_font_interface
{
	lq_uintptr_t ctx;
	lq_pixel_t (*get_height)(lq_uintptr_t ctx);
	lq_pixel_t (*get_ascender)(lq_uintptr_t ctx);
	lq_pixel_t (*get_descender)(lq_uintptr_t ctx);
	lq_pixel_t (*get_x_height)(lq_uintptr_t ctx);
	lq_pixel_t (*get_0_height)(lq_uintptr_t ctx);
} lq_core_font_interface_t;

typedef struct lq_core_font_register_interface
{
	lq_uintptr_t ctx;
	void (*add)(lq_uintptr_t ctx, const lq_utf8_str_t font_path, const lq_utf8_str_t opt_family);
	lq_core_font_interface_t (*find_or_create)(lq_uintptr_t ctx, const lq_font_query_t* query);
	void (*reserve_sources)(lq_uintptr_t ctx, const lq_utf8_str_t family, lq_uint32_t cap);
} lq_core_font_register_interface_t;

typedef lq_core_font_interface_t (*lq_core_font_register_create_fn_t)(lq_uint32_t family_cap, lq_uint32_t inst_cap);
typedef void (*lq_core_font_register_destroy_fn_t)(lq_core_font_register_interface_t* font_register);

static inline lq_core_font_interface_t lq_core_font_register_create(lq_core_font_register_create_fn_t create_fn, lq_uint32_t family_cap, lq_uint32_t inst_cap)
{
	LQ_DEBUG_ASSERT(create_fn != NULL, "Font register create function must not be null.");
	return create_fn(family_cap, inst_cap);
}

static inline void lq_core_font_register_destroy(lq_core_font_register_interface_t* font_register, lq_core_font_register_destroy_fn_t destroy_fn)
{
	LQ_DEBUG_ASSERT(font_register != NULL, "Font register must not be null.");
	LQ_DEBUG_ASSERT(destroy_fn != NULL, "Font register destroy function must not be null.");
	destroy_fn(font_register);
}