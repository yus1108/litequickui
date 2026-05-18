#pragma once

#include "lq_core/types.h"

lq_bool_t test_lq_document_minimal_implementation(void);
#if defined(_WIN32) || defined(_WIN64)
	lq_bool_t test_win32_api_implmenetation(void);
#endif