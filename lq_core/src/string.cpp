#include "lq_core/string.h"
#include "string.hpp"

#include <cstdlib>
#include <cstring>

#include "lq_core/defines.h"

lq_bool_t lq_inspect_utf8_bytes(lq_uint32_t* out_opt_length, lq_uint32_t* out_opt_size, const lq_byte_t* utf8_bytes)
{
	if (utf8_bytes == NULL) return lq_false;

	lq_uint32_t length = 0;
	const lq_byte_t* curr = utf8_bytes;

	while (*curr != 0)
	{
		lq_uint32_t expectedBytes = 0;
		lq_byte_t firstByte = *curr;

		if ((firstByte & 0x80) == 0x00) { expectedBytes = 1; }
		else if ((firstByte & 0xE0) == 0xC0) { expectedBytes = 2; }
		else if ((firstByte & 0xF0) == 0xE0) { expectedBytes = 3; }
		else if ((firstByte & 0xF8) == 0xF0) { expectedBytes = 4; }
		else { return lq_false; }

		lq_uint32_t codePoint = 0;
		switch (expectedBytes)
		{
		case 4:
			if (curr[1] == 0 || curr[2] == 0 || curr[3] == 0) { return lq_false; }
			if ((curr[3] & 0xC0) != 0x80) { return lq_false; }
			if ((curr[2] & 0xC0) != 0x80) { return lq_false; }
			if ((curr[1] & 0xC0) != 0x80) { return lq_false; }
			codePoint = ((curr[0] & 0x07) << 18) | ((curr[1] & 0x3F) << 12) | ((curr[2] & 0x3F) << 6) | (curr[3] & 0x3F);
			// Check Range & Overlong
			if (codePoint < 0x10000 || codePoint > 0x10FFFF) { return lq_false; }
			break;
		case 3:
			if (curr[1] == 0 || curr[2] == 0) { return lq_false; }
			if ((curr[2] & 0xC0) != 0x80) { return lq_false; }
			if ((curr[1] & 0xC0) != 0x80) { return lq_false; }
			codePoint = ((curr[0] & 0x0F) << 12) | ((curr[1] & 0x3F) << 6) | (curr[2] & 0x3F);
			// Check Overlong & Surrogate Pair Range
			if (codePoint < 0x0800 || (codePoint >= 0xD800 && codePoint <= 0xDFFF)) { return lq_false; }
			break;
		case 2:
			if (curr[1] == 0) { return lq_false; }
			if ((curr[1] & 0xC0) != 0x80) { return lq_false; }
			codePoint = ((curr[0] & 0x1F) << 6) | (curr[1] & 0x3F);
			if (codePoint < 0x0080) { return lq_false; } // Check Overlong
			break;
		case 1:
			break;
		default:
			return lq_false;
		}

		curr += expectedBytes;
		length++;
	}

	if (out_opt_length != NULL) { *out_opt_length = length; }
	if (out_opt_size != NULL) { *out_opt_size = static_cast<lq_uint32_t>(curr - utf8_bytes) + 1; }

	return lq_true;
}

lq_bool_t lq_inspect_utf8_cstr(lq_uint32_t* out_opt_length, lq_uint32_t* out_opt_size, const lq_char_t* utf8_cstr)
{
	return lq_inspect_utf8_bytes(out_opt_length, out_opt_size, reinterpret_cast<const lq_byte_t*>(utf8_cstr));
}

LQ_CORE_API lq_bool_t lq_utf8_bytes_equal(const lq_byte_t* a, const lq_byte_t* b)
{
	LQ_DEBUG_ASSERT(a != NULL, "Input bytes a must not be null.");
	LQ_DEBUG_ASSERT(b != NULL, "Input bytes b must not be null.");
	if (a == b) { return lq_true; }

	lq_uint32_t a_size, b_size;
	LQ_DEBUG_ONLY(lq_bool_t ret = )lq_inspect_utf8_bytes(NULL, &a_size, a);
	LQ_DEBUG_ASSERT(ret == lq_true, "Input bytes a must be valid UTF-8.");	
	LQ_DEBUG_ONLY(ret = )lq_inspect_utf8_bytes(NULL, &b_size, b);
	LQ_DEBUG_ASSERT(ret == lq_true, "Input bytes b must be valid UTF-8.");

	if (a_size != b_size) { return lq_false; }

	if (a_size == 0) { return lq_true; }

	return std::memcmp(a, b, a_size) == 0 ? lq_true : lq_false;
}

lq_utf8_str_t lq_utf8_str_create(const lq_byte_t* utf8_bytes)
{
	LQ_DEBUG_ASSERT(utf8_bytes != NULL, "Input bytes must not be null.");

	lq_uint32_t length, size;
	if (lq_inspect_utf8_bytes(&length, &size, utf8_bytes) == lq_false) { return NULL; }

	lq_utf8_str_t str = static_cast<lq_utf8_str_t>(malloc(sizeof(struct lq_utf8_str)));
	str->bytes = static_cast<lq_byte_t*>(malloc(size));
	std::memcpy(str->bytes, utf8_bytes, size);
	str->length = length;
	str->size = size;
	return str;
}

lq_utf8_str_t lq_utf8_str_create_cstr(const lq_char_t* utf8_cstr)
{
	LQ_DEBUG_ASSERT(utf8_cstr != NULL, "Input bytes must not be null.");
	return lq_utf8_str_create(reinterpret_cast<const lq_byte_t*>(utf8_cstr));
}

lq_utf8_str_t lq_utf8_str_create_copy(const lq_utf8_str_t str)
{
	LQ_DEBUG_ASSERT(lq_utf8_str_is_valid(str), "str to copy must be valid");
	lq_utf8_str_t new_str = static_cast<lq_utf8_str_t>(malloc(sizeof(struct lq_utf8_str)));
	new_str->bytes = static_cast<lq_byte_t*>(malloc(str->size));
	std::memcpy(new_str->bytes, str->bytes, str->size);
	new_str->length = str->length;
	new_str->size = str->size;
	return new_str;
}

void lq_utf8_str_destroy(lq_utf8_str_t str)
{
	LQ_DEBUG_ASSERT(lq_utf8_str_is_valid(str), "Input string must be valid.");

	free(str->bytes);
	free(str);
}

lq_bool_t lq_utf8_str_is_valid(const lq_utf8_str_t str)
{
	return str != NULL && (str->bytes != NULL || str->length == 0) && ((str->length > 0 && str->size > 0) || (str->length == 0 && str->size == 0));
}

lq_bool_t lq_utf8_str_is_empty(const lq_utf8_str_t str)
{
	LQ_DEBUG_ASSERT(lq_utf8_str_is_valid(str), "Input string must be valid.");
	return str->length == 0;
}

lq_uint32_t lq_utf8_str_get_length(const lq_utf8_str_t str)
{
	LQ_DEBUG_ASSERT(lq_utf8_str_is_valid(str), "Input string must be valid.");
	return str->length;
}

lq_uint32_t lq_utf8_str_get_size(const lq_utf8_str_t str)
{
	LQ_DEBUG_ASSERT(lq_utf8_str_is_valid(str), "Input string must be valid.");
	return str->size;
}

const lq_char_t* lq_utf8_str_get_cstr(const lq_utf8_str_t str)
{
	LQ_DEBUG_ASSERT(lq_utf8_str_is_valid(str), "Input string must be valid.");
	return reinterpret_cast<const lq_char_t*>(str->bytes);
}

const lq_byte_t* lq_utf8_str_get_bytes(const lq_utf8_str_t str)
{
	LQ_DEBUG_ASSERT(lq_utf8_str_is_valid(str), "Input string must be valid.");
	return str->bytes;
}

lq_bool_t lq_utf8_str_equals(const lq_utf8_str_t a, const lq_utf8_str_t b)
{
	LQ_DEBUG_ASSERT(lq_utf8_str_is_valid(a), "Input string a must be valid.");
	LQ_DEBUG_ASSERT(lq_utf8_str_is_valid(b), "Input string b must be valid.");

	if (a == b) { return lq_true; }
	if (a->length != b->length) { return lq_false; }

	// Skip checking b's length since we already know a and b have the same length. We can just check a's length against 0 to cover both cases.
	if (a->length == 0) { return lq_true; }

	return std::memcmp(a->bytes, b->bytes, a->size) == 0 ? lq_true : lq_false;
}
