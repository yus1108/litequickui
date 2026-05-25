#pragma once
#include <lq_core.h>

#include "defines.h"
#include "font.h"
#include "font_register.h"

LQ_HB_FT_API void lq_hb_ft_font_register_bind_to(lq_core_font_register_interface_t* out_interface, lq_hb_ft_font_register_t derived);