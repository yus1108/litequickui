#include "lq_core/array.h"
#include "array.hpp"

#include <stdlib.h>
#include <string.h>

lq_array_t lq_array_create(lq_uint32_t element_size, lq_uint32_t element_count)
{
	lq_array_t array = (lq_array_t)malloc(sizeof(struct lq_array));
	if (array == NULL)
	{
		return NULL;
	}

	array->element_size = element_size;
	array->element_count = element_count;
	if (element_count == 0)
	{
		array->elements = NULL;
		return array;
	}

	array->elements = calloc(element_count, element_size);
	if (array->elements == NULL)
	{
		free(array);
		return NULL;
	}
	return array;
}

void lq_array_destroy(lq_array_t array)
{
	LQ_DEBUG_ASSERT(lq_array_is_valid(array), "array must be valid");
	if (array->elements != NULL)
	{
		free(array->elements);
	}
	free(array);
}

lq_bool_t lq_array_is_valid(const lq_array_t array)
{
	return array != NULL && (array->elements != NULL || array->element_count == 0);
}

void* lq_array_get(lq_array_t array, lq_uint32_t index)
{
	LQ_DEBUG_ASSERT(lq_array_is_valid(array), "array must be valid");
	LQ_DEBUG_ASSERT(index < array->element_count, "index out of bounds");
	return (lq_byte_t*)array->elements + (index * array->element_size);
}

const void* lq_array_get_const(const lq_array_t array, lq_uint32_t index)
{
	LQ_DEBUG_ASSERT(lq_array_is_valid(array), "array must be valid");
	LQ_DEBUG_ASSERT(index < array->element_count, "index out of bounds");
	return (const lq_byte_t*)array->elements + (index * array->element_size);
}

lq_uint32_t lq_array_get_count(const lq_array_t array)
{
	LQ_DEBUG_ASSERT(lq_array_is_valid(array), "array must be valid");
	return array->element_count;
}

void lq_array_resize(lq_array_t array, lq_uint32_t new_element_count)
{
	LQ_DEBUG_ASSERT(lq_array_is_valid(array), "array must be valid");

	void* new_elements = malloc(array->element_size * new_element_count);
	if (new_elements == NULL) { return; }

	if (array->elements != NULL)
	{
		memcpy(new_elements, array->elements, array->element_size * (new_element_count < array->element_count ? new_element_count : array->element_count));
		free(array->elements);
	}
	array->elements = new_elements;
	array->element_count = new_element_count;
}

lq_bool_t lq_array_contains(const lq_array_t array, const void* key, lq_array_element_find_fn equals_fn)
{
	return lq_array_find_range(array, 0, array->element_count, key, equals_fn) != NULL;
}

lq_bool_t lq_array_contains_range(const lq_array_t array, lq_uint32_t start_index, lq_uint32_t end_index, const void* key, lq_array_element_find_fn equals_fn)
{
	return lq_array_find_range(array, start_index, end_index, key, equals_fn) != NULL;
}

void* lq_array_find(const lq_array_t array, const void* key, lq_array_element_find_fn equals_fn)
{
	return lq_array_find_range(array, 0, array->element_count, key, equals_fn);
}

const void* lq_array_find_const(const lq_array_t array, const void* key, lq_array_element_find_fn equals_fn)
{
	return lq_array_find_range(array, 0, array->element_count, key, equals_fn);
}

void* lq_array_find_range(const lq_array_t array, lq_uint32_t start_index, lq_uint32_t end_index, const void* key, lq_array_element_find_fn equals_fn)
{
	LQ_DEBUG_ASSERT(lq_array_is_valid(array), "array must be valid");
	LQ_DEBUG_ASSERT(key != NULL, "key must not be NULL");
	LQ_DEBUG_ASSERT(equals_fn != NULL, "equals_fn must not be NULL");
	LQ_DEBUG_ASSERT(start_index <= end_index, "start_index must be less than or equal to end_index");
	LQ_DEBUG_ASSERT(end_index <= array->element_count, "end_index must be less than or equal to array->element_count");

	if (array->elements == NULL)
	{
		return NULL;
	}

	for (lq_uint32_t i = start_index; i < end_index; i++)
	{
		void* current_element = (lq_byte_t*)array->elements + (i * array->element_size);
		if (equals_fn(current_element, key)) { return current_element; }
	}
	return NULL;
}

void* lq_array_find_range_const(const lq_array_t array, lq_uint32_t start_index, lq_uint32_t end_index, const void* key, lq_array_element_find_fn equals_fn)
{
	return lq_array_find_range(array, start_index, end_index, key, equals_fn);
}
