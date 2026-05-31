#pragma once
#include "defines.h"
#include "types.h"

typedef struct lq_array* lq_array_t;
typedef lq_bool_t(*lq_array_element_find_fn)(const void* element, const void* key);

LQ_CORE_API lq_array_t lq_array_create(lq_uint32_t element_size, lq_uint32_t element_count);
LQ_CORE_API void 	  lq_array_destroy(lq_array_t array);

LQ_CORE_API lq_bool_t lq_array_is_valid(const lq_array_t array);

LQ_CORE_API void*       lq_array_get(lq_array_t array, lq_uint32_t index);
LQ_CORE_API const void* lq_array_get_const(const lq_array_t array, lq_uint32_t index);
LQ_CORE_API lq_uint32_t lq_array_get_count(const lq_array_t array);

LQ_CORE_API void lq_array_resize(lq_array_t array, lq_uint32_t new_element_count);

LQ_CORE_API lq_bool_t   lq_array_contains(const lq_array_t array, const void* key, lq_array_element_find_fn equals_fn);
LQ_CORE_API lq_bool_t   lq_array_contains_range(const lq_array_t array, lq_uint32_t start_index, lq_uint32_t end_index, const void* key, lq_array_element_find_fn equals_fn);

LQ_CORE_API void*       lq_array_find(const lq_array_t array, const void* key, lq_array_element_find_fn equals_fn);
LQ_CORE_API const void* lq_array_find_const(const lq_array_t array, const void* key, lq_array_element_find_fn equals_fn);
LQ_CORE_API lq_bool_t   lq_array_find_index(const lq_array_t array, lq_uint32_t* out_index, const void* key, lq_array_element_find_fn equals_fn);
LQ_CORE_API lq_bool_t   lq_array_find_index_range(const lq_array_t array, lq_uint32_t* out_index, lq_uint32_t start_index, lq_uint32_t end_index, const void* key, lq_array_element_find_fn equals_fn);
LQ_CORE_API void*       lq_array_find_range(const lq_array_t array, lq_uint32_t start_index, lq_uint32_t end_index, const void* key, lq_array_element_find_fn equals_fn);
LQ_CORE_API const void* lq_array_find_range_const(const lq_array_t array, lq_uint32_t start_index, lq_uint32_t end_index, const void* key, lq_array_element_find_fn equals_fn);
