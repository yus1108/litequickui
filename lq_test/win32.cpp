#if defined(_WIN32) || defined(_WIN64)
#include "lq_test.h"

#include <locale.h>
#include <stdio.h>

#include <lq_core.h>
#include <lq_hb_ft.h>
#include <litequickui.h>

#include "win32_api.h"

lq_bool_t test_win32_api_implmenetation(void)
{
	setlocale(LC_ALL, ".utf8");
	win32_set_console_output_cp(65001);

	typedef struct win32_app_user_data
	{
		lq_utf8_str_t families[1];
		lq_utf8_str_t font_paths[1];
		lq_utf8_str_t title;
		lq_document_t document;
		lq_core_font_register_interface_t font_register;
	} win32_app_user_data_t;
	win32_app_user_data_t app_data = {};
	app_data.title = lq_utf8_str_create_cstr("Test Window");
	app_data.families[0] = lq_utf8_str_create_cstr("Noto Sans");
	app_data.font_paths[0] = lq_utf8_str_create_cstr("fonts/noto_sans/static/NotoSans-Regular.ttf");

	win32_app_callbacks_t callbacks = {};
	callbacks.create = [](win32_app_t app)
		{
			win32_app_user_data_t* appData = (win32_app_user_data_t*)win32_app_get_user_data(app);
			printf("Window created with title: %s\n", lq_utf8_str_get_cstr(appData->title));
		};
	callbacks.destroy = [](win32_app_t app)
		{
			win32_app_user_data_t* appData = (win32_app_user_data_t*)win32_app_get_user_data(app);
			printf("Window destroyed with title: %s\n", lq_utf8_str_get_cstr(appData->title));
		};

	win32_app_t app = win32_app_create(app_data.title, (lq_uintptr_t)&app_data, &callbacks, NULL);
	if (app == NULL) { return lq_false; }

	app_data.font_register = lq_font_register_interface_create(10, 100);
	app_data.font_register.reserve_sources(app_data.font_register.ctx, app_data.families[0], 4); // Reserve enough capacity for 4 font sources for the test font family

	app_data.font_register.add(app_data.font_register.ctx, app_data.font_paths[0], app_data.families[0]); // Register a common system font for testing

	static const int       BITS_PER_COLOR_CHANNEL         = 8;
	static const float     DEFAULT_FONT_SIZE              = 16.0f;
	static const lq_byte_t DEFAULT_FONT_NAME_UTF8_BYTES[] = "Noto Sans";

	typedef struct lq_html_user_data
	{
		win32_app_t   app;
		lq_utf8_str_t default_font_name;
	} lq_html_user_data_t;
	lq_html_user_data_t html_data = {};
	html_data.app = app;
	html_data.default_font_name = lq_utf8_str_create(DEFAULT_FONT_NAME_UTF8_BYTES);

	lq_document_callbacks_t doc_callbacks = {};
	doc_callbacks.get_media_features = [](lq_wrapper_media_features_t* out_media, lq_uintptr_t user_data)
		{
			lq_pixel2_t monitor_resolution = win32_app_get_monitor_resolution(((lq_html_user_data_t*)user_data)->app);
			lq_pixel2_t window_size = win32_app_get_window_size(((lq_html_user_data_t*)user_data)->app);

			out_media->resolution = win32_app_get_dpi(((lq_html_user_data_t*)user_data)->app); // The resolution of the output device (in DPI)

			// litequickui only supports screen media type for now, so we can ignore the distinction between paged and continuous media types for this test.
			out_media->type = LQ_WRAPPER_MEDIA_TYPE_SCREEN;
			
			out_media->width = window_size.x;                // The width of the viewport including the size of a rendered scroll bar (if any).
			out_media->height = window_size.y;               // The height of the targeted display area of the output device.
			out_media->device_width = monitor_resolution.x;  // The width of the screen.
			out_media->device_height = monitor_resolution.y; // The height of the screen.

			out_media->color = BITS_PER_COLOR_CHANNEL; // 8 bits per color channel is more than enough for modern UI rendering.
			out_media->color_index = 0;                // Assume no color lookup table (i.e. true color display)
			out_media->monochrome = 0;                 // Assume not a monochrome display
		};
	doc_callbacks.set_caption = [](const lq_byte_t* utf8_caption, lq_uintptr_t user_data)
		{
			win32_app_t app = ((lq_html_user_data_t*)user_data)->app;
			win32_app_set_window_title_utf8_bytes(app, utf8_caption);
		};
	doc_callbacks.get_default_font_size = [](lq_uintptr_t user_data) -> lq_pixel_t
		{
			LQ_UNUSED(user_data);
			return DEFAULT_FONT_SIZE; // Return a default font size of 16 pixels for testing
		};
	doc_callbacks.get_default_font_name = [](lq_uintptr_t user_data) -> lq_utf8_str_t
		{
			lq_html_user_data_t* htmlUserData = (lq_html_user_data_t*)user_data;
			return htmlUserData->default_font_name; // Return a default font name for testing
		};

	lq_utf8_str_t html_str = lq_utf8_str_create_cstr("<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><title>Test Document</title></head><body><h1>Hello, World!</h1></body></html>");
	lq_document_description_t doc_desc = {};
	doc_desc.html_data = html_str;
	doc_desc.callbacks = doc_callbacks;
	doc_desc.font_register = app_data.font_register;
	doc_desc.user_data = (lq_uintptr_t)&html_data;
	app_data.document = lq_document_create(&doc_desc);
	lq_utf8_str_destroy(html_str);

	win32_app_show(app);

	while (win32_app_update(app))
	{
	}

	lq_document_destroy(app_data.document);
	lq_utf8_str_destroy(html_data.default_font_name);

	win32_app_destroy(app);
	lq_utf8_str_destroy(app_data.title);
	lq_font_register_interface_destroy(&app_data.font_register);
	return lq_true;
}
#endif