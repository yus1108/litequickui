#pragma once
#include "defines.h"
#include "types.h"

typedef struct lq_vector* lq_vector_t;
typedef lq_bool_t(*lq_vector_element_find_fn)(const void* element, const void* key);

LQ_CORE_API lq_vector_t lq_vector_create(lq_uint32_t element_size, lq_uint32_t capacity);
LQ_CORE_API void 	  lq_vector_destroy(lq_vector_t vector);

LQ_CORE_API lq_bool_t lq_vector_is_valid(const lq_vector_t vector);

// Returns the element which index is equal to the current count and then increments the count.
LQ_CORE_API void* lq_vector_acquire_back(lq_vector_t vector);
// Returns the last element and then decrements the count.
LQ_CORE_API void* lq_vector_return_back(lq_vector_t vector);

LQ_CORE_API void*       lq_vector_get(lq_vector_t vector, lq_uint32_t index);
LQ_CORE_API const void* lq_vector_get_const(const lq_vector_t vector, lq_uint32_t index);
LQ_CORE_API lq_uint32_t lq_vector_get_count(const lq_vector_t vector);
LQ_CORE_API lq_uint32_t lq_vector_get_capacity(const lq_vector_t vector);

LQ_CORE_API void lq_vector_reserve(lq_vector_t vector, lq_uint32_t new_capacity);
LQ_CORE_API void lq_vector_shrink_to_fit(lq_vector_t vector);

LQ_CORE_API lq_bool_t   lq_vector_contains(const lq_vector_t vector, const void* key, lq_vector_element_find_fn equals_fn);
LQ_CORE_API lq_bool_t   lq_vector_contains_range(const lq_vector_t vector, lq_uint32_t start_index, lq_uint32_t end_index, const void* key, lq_vector_element_find_fn equals_fn);

LQ_CORE_API void*       lq_vector_find(const lq_vector_t vector, const void* key, lq_vector_element_find_fn equals_fn);
LQ_CORE_API const void* lq_vector_find_const(const lq_vector_t vector, const void* key, lq_vector_element_find_fn equals_fn);
LQ_CORE_API lq_bool_t   lq_vector_find_index(const lq_vector_t vector, lq_uint32_t* out_index, const void* key, lq_vector_element_find_fn equals_fn);
LQ_CORE_API lq_bool_t   lq_vector_find_index_range(const lq_vector_t vector, lq_uint32_t* out_index, lq_uint32_t start_index, lq_uint32_t end_index, const void* key, lq_vector_element_find_fn equals_fn);
LQ_CORE_API void*       lq_vector_find_range(const lq_vector_t vector, lq_uint32_t start_index, lq_uint32_t end_index, const void* key, lq_vector_element_find_fn equals_fn);
LQ_CORE_API const void* lq_vector_find_range_const(const lq_vector_t vector, lq_uint32_t start_index, lq_uint32_t end_index, const void* key, lq_vector_element_find_fn equals_fn);
