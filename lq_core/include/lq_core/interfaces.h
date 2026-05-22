#pragma once

#include "types.h"
#include "string.h"
#include "font_query.h"

typedef struct lq_core_font_interface
{
	lq_uintptr_t ctx;
} lq_core_font_interface_t;

typedef struct lq_core_font_register_interface
{
	lq_uintptr_t ctx;
	void (*add)(lq_uintptr_t ctx, const lq_utf8_str_t font_path, const lq_utf8_str_t opt_family);
	lq_core_font_interface_t (*find_or_create)(lq_uintptr_t ctx, const lq_font_query_t* query);
} lq_core_font_register_interface_t;