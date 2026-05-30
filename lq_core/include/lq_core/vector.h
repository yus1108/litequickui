#pragma once
#include "defines.h"
#include "types.h"
#include "array.h"

typedef struct lq_vector* lq_vector_t;
typedef lq_bool_t(*lq_vector_element_find_fn)(const void* element, const void* key);

LQ_CORE_API lq_vector_t lq_vector_create(lq_uint32_t element_size, lq_uint32_t capacity);
LQ_CORE_API void 	  lq_vector_destroy(lq_vector_t vector);

LQ_CORE_API lq_bool_t lq_vector_is_valid(const lq_vector_t vector);

LQ_CORE_API void* lq_vector_get(lq_vector_t vector, lq_uint32_t index);
LQ_CORE_API const void* lq_vector_get_const(const lq_vector_t vector, lq_uint32_t index);
LQ_CORE_API lq_uint32_t lq_vector_get_count(const lq_vector_t vector);
LQ_CORE_API lq_uint32_t lq_vector_get_capacity(const lq_vector_t vector);

LQ_CORE_API void lq_vector_reserve(lq_vector_t vector, lq_uint32_t new_capacity);