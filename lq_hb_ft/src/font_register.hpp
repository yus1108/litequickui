#pragma once
#include "lq_hb_ft/font_register.h"

#include <freetype/freetype.h>

void lq_hb_ft_font_register_add(lq_hb_ft_font_register_t font_register, const lq_utf8_str_t path, const lq_utf8_str_t opt_family);

static inline void lq_core_font_register_bind_hb_ft_add_fn(lq_uintptr_t ctx, const lq_utf8_str_t font_path, const lq_utf8_str_t opt_family)
{
	lq_hb_ft_font_register_t font_register = (lq_hb_ft_font_register_t)ctx;
	lq_hb_ft_font_register_add(font_register, font_path, opt_family);
}

lq_core_font_interface_t lq_hb_ft_font_register_find_or_create(lq_hb_ft_font_register_t font_register, const lq_font_query_t* query);

static inline lq_core_font_interface_t lq_core_font_register_bind_hb_ft_find_or_create_fn(lq_uintptr_t ctx, const lq_font_query_t* query)
{
	lq_hb_ft_font_register_t font_register = (lq_hb_ft_font_register_t)ctx;
	return lq_hb_ft_font_register_find_or_create(font_register, query);
}

typedef struct lq_hb_ft_font_source
{
	LQ_DEBUG_ONLY(lq_utf8_str_t name);
	lq_utf8_str_t path;
	lq_wrapper_font_style style;
	lq_uint16_t weight;
} lq_hb_ft_font_source_t;

static inline lq_bool_t lq_hb_ft_font_source_find_by_path(const void* element, const void* key)
{
	if (element == key) { return lq_true; }
	const lq_hb_ft_font_source_t* source = (const lq_hb_ft_font_source_t*)element;
	const lq_utf8_str_t* key_path = (const lq_utf8_str_t*)key;
	return lq_utf8_str_equals(source->path, *key_path);
}

typedef struct lq_hb_ft_font_source_pool
{
	lq_array_t  array; // lq_hb_ft_font_source_t
	lq_uint32_t count;
} lq_hb_ft_font_source_pool_t;

lq_hb_ft_font_source_pool_t lq_hb_ft_font_source_pool_create(lq_uint32_t capacity);
void                        lq_hb_ft_font_source_pool_destroy(lq_hb_ft_font_source_pool_t* sources);

lq_hb_ft_font_source_t* lq_hb_ft_font_source_pool_acquire_back(lq_hb_ft_font_source_pool_t* sources);
lq_bool_t               lq_hb_ft_font_source_pool_contains(const lq_hb_ft_font_source_pool_t* sources, const lq_utf8_str_t path);
lq_hb_ft_font_source_t* lq_hb_ft_font_source_pool_find(const lq_hb_ft_font_source_pool_t* sources, const lq_utf8_str_t* path);

typedef struct lq_hb_ft_font_family
{
	lq_utf8_str_t name;
	lq_hb_ft_font_source_pool_t sources;
} lq_hb_ft_font_family_t;

static inline lq_bool_t lq_hb_ft_font_family_find_by_name(const void* element, const void* key)
{
	const lq_hb_ft_font_family_t* family = (const lq_hb_ft_font_family_t*)element;
	const lq_utf8_str_t* key_name = (const lq_utf8_str_t*)key;
	return lq_utf8_str_equals(family->name, *key_name);
}

typedef struct lq_hb_ft_font_family_pool
{
	lq_array_t  array; // lq_hb_ft_font_family_t
	lq_uint32_t count;
} lq_hb_ft_font_family_pool_t;

lq_hb_ft_font_family_pool_t lq_hb_ft_font_family_pool_create(lq_uint32_t capacity);
void                        lq_hb_ft_font_family_pool_destroy(lq_hb_ft_font_family_pool_t* families);

lq_bool_t lq_hb_ft_font_family_pool_is_empty(const lq_hb_ft_font_family_pool_t* families);

lq_hb_ft_font_family_t* lq_hb_ft_font_family_pool_acquire_back(lq_hb_ft_font_family_pool_t* families);
lq_hb_ft_font_family_t* lq_hb_ft_font_family_pool_find(const lq_hb_ft_font_family_pool_t* families, const lq_utf8_str_t family);

typedef struct lq_hb_ft_font_instance
{
	FT_Face face;
	lq_font_query_t query;
} lq_hb_ft_font_instance_t;

typedef struct lq_hb_ft_font_instance_pool
{
	lq_array_t  array; // lq_hb_ft_font_instance_t
	lq_uint32_t count;
} lq_hb_ft_font_instance_pool_t;

lq_hb_ft_font_instance_pool_t lq_hb_ft_font_instance_pool_create(lq_uint32_t capacity);
void                          lq_hb_ft_font_instance_pool_destroy(lq_hb_ft_font_instance_pool_t* entries);

typedef struct lq_hb_ft_font_register
{
	lq_hb_ft_font_family_pool_t   families;
	lq_hb_ft_font_instance_pool_t instances;
	FT_Library lib;
}* lq_hb_ft_font_register_t;