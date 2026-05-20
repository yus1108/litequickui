#pragma once

#include "types.h"
#include "utf8.h"

typedef struct lq_font_register_interface
{
	lq_uintptr_t ctx;
	void (*register_font)(lq_uintptr_t ctx, const lq_utf8_str_t font_path);
	void (*register_font_utf8_bytes)(lq_uintptr_t ctx, const lq_byte_t* font_path);
} lq_font_register_t;