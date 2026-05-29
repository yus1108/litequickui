#pragma once
#include "defines.h"

#include <lq_core/types.h>
#include <lq_core/interfaces.h>

typedef lq_core_font_register_interface_t lq_font_register_interface_t;

LQ_API lq_font_register_interface_t lq_font_register_interface_create(lq_uint32_t family_cap, lq_uint32_t inst_cap);
LQ_API void                         lq_font_register_interface_destroy(lq_font_register_interface_t* font_register);