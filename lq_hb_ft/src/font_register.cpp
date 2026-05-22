#include "lq_hb_ft/font_register.h"
#include "font_register.hpp"

#include <freetype/tttables.h>

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

#include <stdio.h>
#include <direct.h>

void lq_hb_ft_font_register_add(lq_hb_ft_font_register_t font_register, const lq_utf8_str_t path, const lq_utf8_str_t opt_family)
{
	LQ_DEBUG_ASSERT(font_register != NULL, "font_register must not be NULL");

	const lq_char_t* path_cstr = lq_utf8_str_get_cstr(path);

	char buffer[1024];
	if (_getcwd(buffer, sizeof(buffer)) != NULL) {
		printf("Current directory: %s\n", buffer);
	}
	else {
		perror("_getcwd() error");
	}

	FT_Face face = nullptr;
	LQ_DEBUG_ONLY(FT_Error err = )FT_New_Face(font_register->lib, path_cstr, -1, &face);
	LQ_DEBUG_ASSERT(err == 0, "Failed to read font file: %s", path_cstr);

	const FT_Long face_count = face->num_faces;
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
			font_family->sources = lq_hb_ft_font_source_pool_create(4); // Create a font source pool with an initial capacity for 4 font sources for testing
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
	return lq_core_font_interface_t();
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
		static const lq_uint32_t CAP_MULTIPLIER = 2;
		(sources->count > LQ_UINT32_MAX / CAP_MULTIPLIER) ? lq_array_resize(sources->array, LQ_UINT32_MAX) : lq_array_resize(sources->array, sources->count * CAP_MULTIPLIER);
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
		static const lq_uint32_t CAP_MULTIPLIER = 2;
		(families->count > LQ_UINT32_MAX / CAP_MULTIPLIER) ? lq_array_resize(families->array, LQ_UINT32_MAX) : lq_array_resize(families->array, families->count * CAP_MULTIPLIER);
	}

	lq_hb_ft_font_family_t* family = (lq_hb_ft_font_family_t*)lq_array_get(families->array, families->count);
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

void lq_hb_ft_font_instance_pool_destroy(lq_hb_ft_font_instance_pool_t* entries)
{
	LQ_DEBUG_ASSERT(entries != NULL, "entries must not be NULL");
	LQ_DEBUG_ASSERT(entries->array != NULL, "entries->array must not be NULL");
	lq_array_destroy(entries->array);
	entries->array = NULL;
	entries->count = 0;
}
