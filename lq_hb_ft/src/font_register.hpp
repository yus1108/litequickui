#pragma once
#include "lq_hb_ft/font_register.h"

#include <freetype/freetype.h>

static inline const lq_uint32_t LQ_HB_FT_FONT_SOURCE_POOL_DEFAULT_CAPACITY = 4;

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
void                    lq_hb_ft_font_source_pool_reserve(lq_hb_ft_font_source_pool_t* sources, lq_uint32_t capacity);

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
void                          lq_hb_ft_font_instance_pool_destroy(lq_hb_ft_font_instance_pool_t* instances);

typedef struct lq_hb_ft_font_register
{
	lq_hb_ft_font_family_pool_t   families;
	lq_hb_ft_font_instance_pool_t instances;
	FT_Library lib;
}* lq_hb_ft_font_register_t;