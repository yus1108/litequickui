#include "lq_core/vector.h"
#include "vector.hpp"

#include <stdlib.h>

lq_vector_t lq_vector_create(lq_uint32_t element_size, lq_uint32_t capacity)
{
	lq_vector_t vector = (lq_vector_t)malloc(sizeof(struct lq_vector));
	vector->array = lq_array_create(element_size, capacity);
	vector->count = 0;
	return vector;
}

void lq_vector_destroy(lq_vector_t vector)
{
	LQ_DEBUG_ASSERT(lq_vector_is_valid(vector), "Input vector is not valid.");
	lq_array_destroy(vector->array);
	free(vector);
}

lq_bool_t lq_vector_is_valid(const lq_vector_t vector)
{
	return vector != NULL && lq_array_is_valid(vector->array);
}

void* lq_vector_get(lq_vector_t vector, lq_uint32_t index)
{
	LQ_DEBUG_ASSERT(lq_vector_is_valid(vector), "Input vector is not valid.");
	LQ_DEBUG_ASSERT(index < vector->count, "Index out of bounds.");
	return lq_array_get(vector->array, index);
}

const void* lq_vector_get_const(const lq_vector_t vector, lq_uint32_t index)
{
	return lq_vector_get(vector, index);
}

lq_uint32_t lq_vector_get_count(const lq_vector_t vector)
{
	LQ_DEBUG_ASSERT(lq_vector_is_valid(vector), "Input vector is not valid.");
	return vector->count;
}

lq_uint32_t lq_vector_get_capacity(const lq_vector_t vector)
{
	LQ_DEBUG_ASSERT(lq_vector_is_valid(vector), "Input vector is not valid.");
	return lq_array_get_count(vector->array);
}

void lq_vector_reserve(lq_vector_t vector, lq_uint32_t new_capacity)
{
	LQ_DEBUG_ASSERT(lq_vector_is_valid(vector), "Input vector is not valid.");
	if (new_capacity < lq_vector_get_capacity(vector)) 
	{
		LQ_DEBUG_ASSERT(lq_false, "New capacity must be greater than or equal to current capacity.");
		return;
	}
	lq_array_resize(vector->array, new_capacity);
}
