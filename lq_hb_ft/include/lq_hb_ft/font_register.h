#pragma once
#include "defines.h"

#include <lq_core/interfaces.h>
#include <lq_core/types.h>

LQ_HB_FT_API lq_core_font_register_interface_t lq_hb_ft_font_register_create_and_bind(lq_uint32_t family_cap, lq_uint32_t inst_cap);
LQ_HB_FT_API void 				               lq_hb_ft_font_register_destroy(lq_core_font_register_interface_t* font_register);