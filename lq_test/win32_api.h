#pragma once
#if defined(_WIN32) || defined(_WIN64)
#include <lq_core.h>

lq_bool_t win32_set_console_output_cp(lq_uint32_t code_page);

typedef struct win32_app* win32_app_t;
typedef struct win32_app_callbacks
{
	void (*create)(win32_app_t app);
	void (*destroy)(win32_app_t app);
} win32_app_callbacks_t;

lq_bool_t   win32_app_is_valid(win32_app_t app);
win32_app_t win32_app_create(lq_utf8_str_t title, lq_uintptr_t user_data, const win32_app_callbacks_t* callbacks, const lq_rect_t* rect);
void        win32_app_destroy(win32_app_t app);

lq_pixel_t   win32_app_get_dpi(win32_app_t app);
lq_pixel2_t  win32_app_get_monitor_resolution(win32_app_t app);
lq_uintptr_t win32_app_get_user_data(win32_app_t app);
lq_pixel2_t  win32_app_get_window_size(win32_app_t app);

void win32_app_set_window_title(win32_app_t app, const lq_utf8_str_t title);
void win32_app_set_window_title_utf8_bytes(win32_app_t app, const lq_byte_t* utf8_bytes_title);

lq_bool_t win32_app_show(win32_app_t app);
lq_bool_t win32_app_update(win32_app_t app);

#endif