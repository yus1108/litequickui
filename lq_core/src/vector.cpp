 #include "lq_core/vector.h"
#include "vector.hpp"

#include <stdlib.h>
#include <string.h>

lq_vector_t lq_vector_create(lq_uint32_t element_size, lq_uint32_t capacity)
{
	lq_vector_t vector = (lq_vector_t)malloc(sizeof(struct lq_vector));
	LQ_DEBUG_ASSERT(vector != NULL, "Failed to allocate memory for vector.");
	vector->array = lq_array_create(element_size, capacity);
	LQ_DEBUG_ASSERT(vector->array != NULL, "Failed to create array for vector.");
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
	return (vector != NULL && lq_array_is_valid(vector->array)) ? lq_true : lq_false;
}

void* lq_vector_acquire_back(lq_vector_t vector)
{
	LQ_DEBUG_ASSERT(lq_vector_is_valid(vector), "Input vector is not valid.");
	LQ_DEBUG_ASSERT(vector->count < LQ_UINT32_MAX, "vector count overflow");

	if (vector->count >= lq_array_get_count(vector->array))
	{
		LQ_DEBUG_ASSERT(lq_false, "Reserve enough capacity to avoid memory allocation which is expensive.");
		(vector->count > LQ_UINT32_MAX / LQ_POOL_CAPACITY_MULTIPLIER) ? lq_array_resize(vector->array, LQ_UINT32_MAX) : lq_array_resize(vector->array, vector->count * LQ_POOL_CAPACITY_MULTIPLIER);
	}

	return lq_array_get(vector->array, vector->count++);
}

void* lq_vector_return_back(lq_vector_t vector)
{
	LQ_DEBUG_ASSERT(lq_vector_is_valid(vector), "Input vector is not valid.");
	LQ_DEBUG_ASSERT(vector->count > 0, "Vector is already empty.");
	return lq_array_get(vector->array, vector->count--);
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

void lq_vector_shrink_to_fit(lq_vector_t vector)
{
	LQ_DEBUG_ASSERT(lq_vector_is_valid(vector), "Input vector is not valid.");
	if (vector->count == lq_array_get_count(vector->array)) { return; }
	lq_array_t temp = vector->array;
	const lq_uint32_t element_size = lq_array_get_element_size(temp);
	vector->array = lq_array_create(element_size, vector->count);
	memcpy(lq_array_get(vector->array, 0), lq_array_get(temp, 0), element_size * vector->count);
	lq_array_destroy(temp);
}

lq_bool_t lq_vector_contains(const lq_vector_t vector, const void* key, lq_vector_element_find_fn equals_fn)
{
	LQ_DEBUG_ASSERT(lq_vector_is_valid(vector), "Input vector is not valid.");
	LQ_DEBUG_ASSERT(key != NULL, "Key must not be NULL.");
	LQ_DEBUG_ASSERT(equals_fn != NULL, "Equals function must not be NULL.");
	return lq_array_contains(vector->array, key, equals_fn);
}

lq_bool_t lq_vector_contains_range(const lq_vector_t vector, lq_uint32_t start_index, lq_uint32_t end_index, const void* key, lq_vector_element_find_fn equals_fn)
{
	LQ_DEBUG_ASSERT(lq_vector_is_valid(vector), "Input vector is not valid.");
	LQ_DEBUG_ASSERT(end_index <= vector->count, "end_index must be less than or equal to vector count.");
	return lq_array_contains_range(vector->array, start_index, end_index, key, equals_fn);
}

void* lq_vector_find(const lq_vector_t vector, const void* key, lq_vector_element_find_fn equals_fn)
{
	LQ_DEBUG_ASSERT(lq_vector_is_valid(vector), "Input vector is not valid.");
	return lq_array_find(vector->array, key, equals_fn);
}

const void* lq_vector_find_const(const lq_vector_t vector, const void* key, lq_vector_element_find_fn equals_fn)
{
	LQ_DEBUG_ASSERT(lq_vector_is_valid(vector), "Input vector is not valid.");
	return lq_array_find_const(vector->array, key, equals_fn);
}

lq_bool_t lq_vector_find_index(const lq_vector_t vector, lq_uint32_t* out_index, const void* key, lq_vector_element_find_fn equals_fn)
{
	LQ_DEBUG_ASSERT(lq_vector_is_valid(vector), "Input vector is not valid.");
	return lq_array_find_index(vector->array, out_index, key, equals_fn);
}

lq_bool_t lq_vector_find_index_range(const lq_vector_t vector, lq_uint32_t* out_index, lq_uint32_t start_index, lq_uint32_t end_index, const void* key, lq_vector_element_find_fn equals_fn)
{
	LQ_DEBUG_ASSERT(lq_vector_is_valid(vector), "Input vector is not valid.");
	LQ_DEBUG_ASSERT(end_index <= vector->count, "end_index must be less than or equal to vector count.");
	return lq_array_find_index_range(vector->array, out_index, start_index, end_index, key, equals_fn);
}


void* lq_vector_find_range(const lq_vector_t vector, lq_uint32_t start_index, lq_uint32_t end_index, const void* key, lq_vector_element_find_fn equals_fn)
{
	LQ_DEBUG_ASSERT(lq_vector_is_valid(vector), "Input vector is not valid.");
	LQ_DEBUG_ASSERT(end_index <= vector->count, "end_index must be less than or equal to vector count.");
	return lq_array_find_range(vector->array, start_index, end_index, key, equals_fn);
}

const void* lq_vector_find_range_const(const lq_vector_t vector, lq_uint32_t start_index, lq_uint32_t end_index, const void* key, lq_vector_element_find_fn equals_fn)
{
	LQ_DEBUG_ASSERT(lq_vector_is_valid(vector), "Input vector is not valid.");
	LQ_DEBUG_ASSERT(end_index <= vector->count, "end_index must be less than or equal to vector count.");
	return lq_array_find_range_const(vector->array, start_index, end_index, key, equals_fn);
}