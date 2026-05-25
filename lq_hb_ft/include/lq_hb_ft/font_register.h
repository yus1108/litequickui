#pragma once
#include <lq_core.h>

#include "defines.h"

typedef struct lq_hb_ft_font_register* lq_hb_ft_font_register_t;

// family_cap: The initial capacity for the font family pool (i.e. the number of font families that can be registered before needing to resize the pool).
// inst_cap: The initial capacity for the font instance pool (i.e. the number of font instances that can be created before needing to resize the pool).
LQ_HB_FT_API lq_hb_ft_font_register_t lq_hb_ft_font_register_create(lq_uint32_t family_cap, lq_uint32_t inst_cap);
LQ_HB_FT_API void                     lq_hb_ft_font_register_destroy(lq_hb_ft_font_register_t font_register);

void                     lq_hb_ft_font_register_add(lq_hb_ft_font_register_t font_register, const lq_utf8_str_t path, const lq_utf8_str_t opt_family);
lq_core_font_interface_t lq_hb_ft_font_register_find_or_create(lq_hb_ft_font_register_t font_register, const lq_font_query_t* query);

LQ_HB_FT_API void lq_hb_ft_font_register_reserve_sources(lq_hb_ft_font_register_t font_register, const lq_utf8_str_t family, lq_uint32_t cap);