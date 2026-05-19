#include "lq_core/utf8.h"
#include "utf8.hpp"

#include <cstdlib>
#include <cstring>

#include "lq_core/defines.h"

lq_bool_t lq_inspect_raw_utf8(lq_uint32_t* out_opt_length, lq_uint32_t* out_opt_size, const lq_byte_t* raw_utf8)
{
	if (raw_utf8 == NULL) return lq_false;

	lq_uint32_t length = 0;
	const lq_byte_t* curr = raw_utf8;

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
	if (out_opt_size != NULL) { *out_opt_size = static_cast<lq_uint32_t>(curr - raw_utf8) + 1; }

	return lq_true;
}

lq_bool_t lq_inspect_utf8_cstr(lq_uint32_t* out_opt_length, lq_uint32_t* out_opt_size, const lq_char_t* data)
{
	return lq_inspect_raw_utf8(out_opt_length, out_opt_size, reinterpret_cast<const lq_byte_t*>(data));
}

LQ_CORE_API lq_utf8_str_t lq_utf8_str_create(const lq_byte_t* raw_utf8)
{
	LQ_DEBUG_ASSERT(raw_utf8 != NULL, "Input cstr must not be null.");

	lq_uint32_t length, size;
	if (lq_inspect_raw_utf8(&length, &size, raw_utf8) == lq_false) { return NULL; }

	lq_utf8_str_t str = static_cast<lq_utf8_str_t>(malloc(sizeof(struct lq_utf8_str)));
	str->cstr = static_cast<lq_byte_t*>(malloc(size));
	std::memcpy(str->cstr, raw_utf8, size);
	str->length = length;
	str->size = size;
	return str;

	return LQ_CORE_API lq_utf8_str_t();
}

lq_utf8_str_t lq_utf8_str_create_cstr(const lq_char_t* cstr)
{
	LQ_DEBUG_ASSERT(cstr != NULL, "Input cstr must not be null.");
	return lq_utf8_str_create(reinterpret_cast<const lq_byte_t*>(cstr));
}

void lq_utf8_str_destroy(lq_utf8_str_t str)
{
	LQ_DEBUG_ASSERT(str != NULL, "Input string must not be null.");
	LQ_DEBUG_ASSERT(str->cstr != NULL, "String data must not be null.");
	LQ_DEBUG_ASSERT(str->size > 0, "String size must be greater than 0.");

	free(str->cstr);
	free(str);
}

lq_uint32_t lq_utf8_str_get_length(const lq_utf8_str_t str)
{
	LQ_DEBUG_ASSERT(str != NULL, "Input string must not be null.");
	LQ_DEBUG_ASSERT(str->cstr != NULL, "String cstr must not be null.");
	LQ_DEBUG_ASSERT(str->size > 0, "String size must be greater than 0.");
	LQ_DEBUG_ASSERT(str->length > 0, "String length must be greater than 0.");

	return str->length;
}

lq_uint32_t lq_utf8_str_get_size(const lq_utf8_str_t str)
{
	LQ_DEBUG_ASSERT(str != NULL, "Input string must not be null.");
	LQ_DEBUG_ASSERT(str->cstr != NULL, "String cstr must not be null.");
	LQ_DEBUG_ASSERT(str->size > 0, "String size must be greater than 0.");
	LQ_DEBUG_ASSERT(str->length > 0, "String length must be greater than 0.");

	return str->size;
}

const lq_char_t* lq_utf8_str_get_cstr(const lq_utf8_str_t str)
{
	LQ_DEBUG_ASSERT(str != NULL, "Input string must not be null.");
	LQ_DEBUG_ASSERT(str->cstr != NULL, "String cstr must not be null.");
	LQ_DEBUG_ASSERT(str->size > 0, "String size must be greater than 0.");

	return reinterpret_cast<const lq_char_t*>(str->cstr);
}

LQ_CORE_API const lq_byte_t* lq_utf8_str_get_bytes(const lq_utf8_str_t str)
{
	LQ_DEBUG_ASSERT(str != NULL, "Input string must not be null.");
	LQ_DEBUG_ASSERT(str->cstr != NULL, "String cstr must not be null.");
	LQ_DEBUG_ASSERT(str->size > 0, "String size must be greater than 0.");

	return str->cstr;
}
