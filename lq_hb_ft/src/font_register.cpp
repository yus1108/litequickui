#include "lq_hb_ft/font_register.h"
#include "font_register.hpp"

#include <math.h>
#include <freetype/tttables.h>

static inline void lq_hb_ft_font_register_add_fn_bind(lq_uintptr_t ctx, const lq_utf8_str_t font_path, const lq_utf8_str_t opt_family)
{
	lq_hb_ft_font_register_add((lq_hb_ft_font_register_t)ctx, font_path, opt_family);
}

static inline lq_core_font_interface_t lq_hb_ft_font_register_find_or_create_fn_bind(lq_uintptr_t ctx, const lq_font_query_t* query)
{
	return lq_hb_ft_font_register_find_or_create((lq_hb_ft_font_register_t)ctx, query);
}

static inline void lq_hb_ft_font_register_reserve_sources_fn_bind(lq_uintptr_t ctx, const lq_utf8_str_t family, lq_uint32_t cap)
{
	lq_hb_ft_font_register_reserve_sources((lq_hb_ft_font_register_t)ctx, family, cap);
}

lq_core_font_register_interface_t lq_hb_ft_font_register_create_and_bind(lq_uint32_t family_cap, lq_uint32_t inst_cap)
{
	lq_core_font_register_interface_t font_register_interface;
	font_register_interface.ctx = (lq_uintptr_t)lq_hb_ft_font_register_create(family_cap, inst_cap);
	font_register_interface.add = lq_hb_ft_font_register_add_fn_bind;
	font_register_interface.find_or_create = lq_hb_ft_font_register_find_or_create_fn_bind;
	font_register_interface.reserve_sources = lq_hb_ft_font_register_reserve_sources_fn_bind;
	return font_register_interface;
}

LQ_HB_FT_API void lq_hb_ft_font_register_destroy(lq_core_font_register_interface_t* font_register)
{
	LQ_DEBUG_ASSERT(font_register != NULL, "font_register must not be NULL");
	lq_hb_ft_font_register_destroy((lq_hb_ft_font_register_t)font_register->ctx);
}

lq_hb_ft_font_register_t lq_hb_ft_font_register_create(lq_uint32_t src_cap, lq_uint32_t inst_cap)
{
	lq_hb_ft_font_register_t font_register = new lq_hb_ft_font_register;
	font_register->families = lq_hb_ft_font_family_pool_create(src_cap);
	font_register->instances = lq_hb_ft_font_instance_pool_create(inst_cap);
	FT_Init_FreeType(&font_register->lib);
	return font_register;
}

void lq_hb_ft_font_register_destroy(lq_hb_ft_font_register_t font_register)
{
	LQ_DEBUG_ASSERT(font_register != NULL, "font_register must not be NULL");
	lq_hb_ft_font_family_pool_destroy(&font_register->families);
	lq_hb_ft_font_instance_pool_destroy(&font_register->instances);
	FT_Done_FreeType(font_register->lib);
	delete font_register;
}

void lq_hb_ft_font_register_add(lq_hb_ft_font_register_t font_register, const lq_utf8_str_t path, const lq_utf8_str_t opt_family)
{
	LQ_DEBUG_ASSERT(font_register != NULL, "font_register must not be NULL");

	const lq_char_t* path_cstr = lq_utf8_str_get_cstr(path);

	FT_Face face = nullptr;
	LQ_DEBUG_ONLY(FT_Error err = )FT_New_Face(font_register->lib, path_cstr, -1, &face);
	LQ_DEBUG_ASSERT(err == 0, "Failed to read font file: %s", path_cstr);
	const FT_Long face_count = face->num_faces;
	FT_Done_Face(face);

	for (FT_Long i = 0; i < face_count; ++i)
	{
		LQ_DEBUG_ONLY(err = )FT_New_Face(font_register->lib, path_cstr, 0, &face);
		LQ_DEBUG_ASSERT(err == 0, "Failed to read face for the index of %i for %s", i, path_cstr);

		const lq_utf8_str_t family_name = opt_family == NULL ? lq_utf8_str_create_cstr(face->family_name) : opt_family;
		lq_hb_ft_font_family_t* font_family = lq_hb_ft_font_family_pool_find(&font_register->families, family_name);
		if (font_family == nullptr)
		{
			font_family = lq_hb_ft_font_family_pool_acquire_back(&font_register->families);
			font_family->name = family_name;
			font_family->sources = lq_hb_ft_font_source_pool_create(LQ_HB_FT_FONT_SOURCE_POOL_DEFAULT_CAPACITY);
		}

		TT_OS2* os2 = static_cast<TT_OS2*>(FT_Get_Sfnt_Table(face, FT_SFNT_OS2));
		LQ_DEBUG_ASSERT(os2 != nullptr, "Cannot load OS/2 table for %s", path_cstr);

		LQ_DEBUG_ASSERT(lq_hb_ft_font_source_pool_contains(&font_family->sources, path) == lq_false, "Font source with the same path already exists: %s", path_cstr);
		lq_hb_ft_font_source_t* source = lq_hb_ft_font_source_pool_acquire_back(&font_family->sources);
		source->weight = static_cast<int>(os2->usWeightClass);
		source->style = (os2->fsSelection & 1) ? LQ_WRAPPER_FONT_STYLE_ITALIC : LQ_WRAPPER_FONT_STYLE_NORMAL;
		source->path = path;
		LQ_DEBUG_ONLY(source->name = lq_utf8_str_create_cstr(FT_Get_Postscript_Name(face)));
		FT_Done_Face(face);
	}
}

lq_core_font_interface_t lq_hb_ft_font_register_find_or_create(lq_hb_ft_font_register_t font_register, const lq_font_query_t* query)
{
	LQ_DEBUG_ASSERT(font_register != NULL, "font_register must not be NULL");
	LQ_DEBUG_ASSERT(query != NULL, "query must not be NULL");
	LQ_DEBUG_ASSERT(lq_utf8_str_is_valid(query->family), "query family must be a valid UTF-8 string");
	LQ_DEBUG_ASSERT(query->size > 0, "query pixel size must be greater than 0");
	LQ_DEBUG_ASSERT(query->style == LQ_WRAPPER_FONT_STYLE_NORMAL || query->style == LQ_WRAPPER_FONT_STYLE_ITALIC, "query style must be either normal or italic");
	LQ_DEBUG_ASSERT(query->weight > 0, "query weight must be greater than 0");

	lq_hb_ft_font_instance_t* instance = lq_hb_ft_font_instance_pool_find(&font_register->instances, query);
	if (instance != NULL)
	{
		return instance->font_interface;
	}

	lq_hb_ft_font_family_t* font_family = lq_hb_ft_font_family_pool_find(&font_register->families, query->family);
	if (font_family == NULL)
	{
		LQ_DEBUG_ASSERT(lq_false, "No font family found for the query: %s", lq_utf8_str_get_cstr(query->family));
		return lq_core_font_interface_t();
	}

	if (font_family->sources.count == 0)
	{
		LQ_DEBUG_ASSERT(lq_false, "No font sources found for the font family: %s", lq_utf8_str_get_cstr(font_family->name));
		return lq_core_font_interface_t();
	}

	lq_hb_ft_font_source_pool_t candidate_sources = lq_hb_ft_font_source_pool_create(font_family->sources.count);
	for (lq_uint32_t i = 0; i < font_family->sources.count; i++)
	{
		lq_hb_ft_font_source_t* source = lq_hb_ft_font_source_pool_get(&font_family->sources, i);
		if (source->style != query->style) { continue; }
		lq_hb_ft_font_source_pool_acquire_back(&candidate_sources);
	}

	lq_hb_ft_font_source_pool_t* candidate_sources_ptr = &candidate_sources;
	if (candidate_sources.count == 0)
	{
		LQ_DEBUG_ASSERT(lq_false, "Fallback to consider all font sources of the family as candidates since no font source matches the style for the query: %s", lq_utf8_str_get_cstr(query->family));
		candidate_sources_ptr = &font_family->sources;
	}

	int best_weight = LQ_INT32_MIN;
	int best_diff = LQ_INT32_MAX;
	const lq_hb_ft_font_source_t* best_source = NULL;
	for (lq_uint32_t i = 0; i < candidate_sources_ptr->count; i++)
	{
		lq_hb_ft_font_source_t* candidate_source = lq_hb_ft_font_source_pool_get(candidate_sources_ptr, i);
		if (candidate_source->weight == query->weight)
		{
			lq_hb_ft_font_instance_t* new_instance = lq_hb_ft_font_instance_pool_acquire_back(&font_register->instances);
			lq_hb_ft_font_init(&new_instance->font, font_register->lib, candidate_source->path, 0, query->size);
			new_instance->font_interface = lq_hb_ft_font_bind(&new_instance->font);
			new_instance->query = *query;

			lq_hb_ft_font_source_pool_destroy(&candidate_sources);
			return new_instance->font_interface;
		}

		int diff_new = abs(candidate_source->weight - query->weight);
		if (diff_new < best_diff)
		{
			best_weight = candidate_source->weight;
			best_diff = diff_new;
			best_source = candidate_source;
		}
		else if (diff_new == best_diff)
		{
			if (candidate_source->weight > best_weight)
			{
				best_weight = candidate_source->weight;
				best_diff = diff_new;
				best_source = candidate_source;
			}
		}
		else
		{
			// Do nothing here...
		}
	}

	if (best_source == NULL)
	{
		LQ_DEBUG_ASSERT(lq_false, "No font source found for the query: %s", lq_utf8_str_get_cstr(query->family));
		lq_hb_ft_font_source_pool_destroy(&candidate_sources);
		return lq_core_font_interface_t();
	}

	lq_hb_ft_font_instance_t* new_instance = lq_hb_ft_font_instance_pool_acquire_back(&font_register->instances);
	lq_hb_ft_font_init(&new_instance->font, font_register->lib, best_source->path, 0, query->size);
	new_instance->font_interface = lq_hb_ft_font_bind(&new_instance->font);
	new_instance->query = *query;

	lq_hb_ft_font_source_pool_destroy(&candidate_sources);
	return new_instance->font_interface;
}

LQ_HB_FT_API void lq_hb_ft_font_register_reserve_sources(lq_hb_ft_font_register_t font_register, const lq_utf8_str_t family, lq_uint32_t cap)
{
	LQ_DEBUG_ASSERT(font_register != NULL, "font_register must not be NULL");
	LQ_DEBUG_ASSERT(cap > 0, "cap must be greater than 0");

	lq_hb_ft_font_family_t* font_family = lq_hb_ft_font_family_pool_find(&font_register->families, family);
	if (font_family != nullptr)
	{
		lq_hb_ft_font_source_pool_t sources = font_family->sources;
		lq_hb_ft_font_source_pool_reserve(&sources, cap);
	}
	else
	{
		font_family = lq_hb_ft_font_family_pool_acquire_back(&font_register->families);
		font_family->name = family;
		font_family->sources = lq_hb_ft_font_source_pool_create(cap);
	}
}

lq_hb_ft_font_source_pool_t lq_hb_ft_font_source_pool_create(lq_uint32_t capacity)
{
	lq_hb_ft_font_source_pool_t sources;
	sources.array = lq_array_create(sizeof(lq_hb_ft_font_source_t), capacity);
	sources.count = 0;
	return sources;
}

void lq_hb_ft_font_source_pool_destroy(lq_hb_ft_font_source_pool_t* sources)
{
	LQ_DEBUG_ASSERT(sources != NULL, "sources must not be NULL");
	lq_array_destroy(sources->array);
	sources->array = NULL;
	sources->count = 0;
}

lq_hb_ft_font_source_t* lq_hb_ft_font_source_pool_acquire_back(lq_hb_ft_font_source_pool_t* sources)
{
	LQ_DEBUG_ASSERT(sources != NULL, "sources must not be NULL");
	LQ_DEBUG_ASSERT(sources->count < LQ_UINT32_MAX, "sources count overflow");

	if (sources->count >= lq_array_get_count(sources->array))
	{
		LQ_DEBUG_ASSERT(lq_false, "Reserve enough capacity for font families to avoid memory allocation which is expensive.");
		(sources->count > LQ_UINT32_MAX / LQ_HB_FT_POOL_CAP_MULTIPLIER) ? lq_array_resize(sources->array, LQ_UINT32_MAX) : lq_array_resize(sources->array, sources->count * LQ_HB_FT_POOL_CAP_MULTIPLIER);
	}

	return (lq_hb_ft_font_source_t*)lq_array_get(sources->array, sources->count++);
}

lq_bool_t lq_hb_ft_font_source_pool_contains(const lq_hb_ft_font_source_pool_t* sources, const lq_utf8_str_t path)
{
	LQ_DEBUG_ASSERT(sources != NULL, "sources must not be NULL");
	return lq_array_contains_range(sources->array, 0, sources->count, &path, &lq_hb_ft_font_source_find_by_path);
}

lq_hb_ft_font_source_t* lq_hb_ft_font_source_pool_find(const lq_hb_ft_font_source_pool_t* sources, const lq_utf8_str_t* path)
{
	LQ_DEBUG_ASSERT(sources != NULL, "sources must not be NULL");
	return (lq_hb_ft_font_source_t*)lq_array_find_range(sources->array, 0, sources->count, path, &lq_hb_ft_font_source_find_by_path);
}

lq_hb_ft_font_source_t* lq_hb_ft_font_source_pool_get(const lq_hb_ft_font_source_pool_t* sources, lq_uint32_t index)
{
	LQ_DEBUG_ASSERT(sources != NULL, "sources must not be NULL");
	LQ_DEBUG_ASSERT(index < sources->count, "index out of bounds");
	return (lq_hb_ft_font_source_t*)lq_array_get(sources->array, index);
}

void lq_hb_ft_font_source_pool_reserve(lq_hb_ft_font_source_pool_t* sources, lq_uint32_t capacity)
{
	LQ_DEBUG_ASSERT(sources != NULL, "sources must not be NULL");
	LQ_DEBUG_ASSERT(capacity > 0, "capacity must be greater than 0");
	if (lq_array_get_count(sources->array) < capacity)
	{
		lq_array_resize(sources->array, capacity);
	}
}

lq_hb_ft_font_family_pool_t lq_hb_ft_font_family_pool_create(lq_uint32_t capacity)
{
	lq_hb_ft_font_family_pool_t families;
	families.array = lq_array_create(sizeof(lq_hb_ft_font_family_t), capacity);
	families.count = 0;
	return families;
}

void lq_hb_ft_font_family_pool_destroy(lq_hb_ft_font_family_pool_t* families)
{
	LQ_DEBUG_ASSERT(families != NULL, "families must not be NULL");
	lq_array_destroy(families->array);
	families->array = NULL;
	families->count = 0;
}

lq_bool_t lq_hb_ft_font_family_pool_is_empty(const lq_hb_ft_font_family_pool_t* families)
{
	LQ_DEBUG_ASSERT(families != NULL, "families must not be NULL");
	return families->count == 0;
}

lq_hb_ft_font_family_t* lq_hb_ft_font_family_pool_acquire_back(lq_hb_ft_font_family_pool_t* families)
{
	LQ_DEBUG_ASSERT(families != NULL, "families must not be NULL");
	LQ_DEBUG_ASSERT(families->count < LQ_UINT32_MAX, "families count overflow");

	if (families->count >= lq_array_get_count(families->array))
	{
		LQ_DEBUG_ASSERT(lq_false, "Reserve enough capacity for font families to avoid memory allocation which is expensive.");
		(families->count > LQ_UINT32_MAX / LQ_HB_FT_POOL_CAP_MULTIPLIER) ? lq_array_resize(families->array, LQ_UINT32_MAX) : lq_array_resize(families->array, families->count * LQ_HB_FT_POOL_CAP_MULTIPLIER);
	}

	return (lq_hb_ft_font_family_t*)lq_array_get(families->array, families->count++);
}

lq_hb_ft_font_family_t* lq_hb_ft_font_family_pool_find(const lq_hb_ft_font_family_pool_t* families, const lq_utf8_str_t family)
{
	LQ_DEBUG_ASSERT(families != NULL, "families must not be NULL");
	if (lq_hb_ft_font_family_pool_is_empty(families)) { return NULL; }
	return (lq_hb_ft_font_family_t*)lq_array_find_range(families->array, 0, families->count, &family, &lq_hb_ft_font_family_find_by_name);
}

lq_hb_ft_font_instance_pool_t lq_hb_ft_font_instance_pool_create(lq_uint32_t capacity)
{
	lq_hb_ft_font_instance_pool_t instances;
	instances.array = lq_array_create(sizeof(lq_hb_ft_font_instance_t), capacity);
	instances.count = 0;
	return instances;
}

void lq_hb_ft_font_instance_pool_destroy(lq_hb_ft_font_instance_pool_t* instances)
{
	LQ_DEBUG_ASSERT(instances != NULL, "instances must not be NULL");
	LQ_DEBUG_ASSERT(instances->array != NULL, "instances->array must not be NULL");
	lq_array_destroy(instances->array);
	instances->array = NULL;
	instances->count = 0;
}

lq_hb_ft_font_instance_t* lq_hb_ft_font_instance_pool_acquire_back(lq_hb_ft_font_instance_pool_t* instances)
{
	LQ_DEBUG_ASSERT(instances != NULL, "instances must not be NULL");
	LQ_DEBUG_ASSERT(instances->count < LQ_UINT32_MAX, "instances count overflow");

	if (instances->count >= lq_array_get_count(instances->array))
	{
		LQ_DEBUG_ASSERT(lq_false, "Reserve enough capacity for font instances to avoid memory allocation which is expensive.");
		(instances->count > LQ_UINT32_MAX / LQ_HB_FT_POOL_CAP_MULTIPLIER) ? lq_array_resize(instances->array, LQ_UINT32_MAX) : lq_array_resize(instances->array, instances->count * LQ_HB_FT_POOL_CAP_MULTIPLIER);
	}

	return (lq_hb_ft_font_instance_t*)lq_array_get(instances->array, instances->count++);
}

lq_hb_ft_font_instance_t* lq_hb_ft_font_instance_pool_find(const lq_hb_ft_font_instance_pool_t* instances, const lq_font_query_t* query)
{
	LQ_DEBUG_ASSERT(instances != NULL, "instances must not be NULL");
	LQ_DEBUG_ASSERT(query != NULL, "query must not be NULL");
	return (lq_hb_ft_font_instance_t*)lq_array_find_range(instances->array, 0, instances->count, query, &lq_hb_ft_font_instance_find_by_query);
}
