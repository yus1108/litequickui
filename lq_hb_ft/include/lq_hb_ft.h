#pragma once

#include <lq_core.h>
#include "lq_hb_ft/defines.h"
#include "lq_hb_ft/font_register.h"

LQ_HB_FT_API lq_core_font_register_interface_t lq_core_font_register_interface_bind_hb_ft(lq_hb_ft_font_register_t derived);