#include <iostream>
using namespace std;

#include <lq_core.h>

#if defined(_DEBUG)
	#if defined(_WIN32)

		#define DETECT_MEMORY_LEAKS(goToLine) win_detect_memory_leak(goToLine)

		#define _CRTDBG_MAP_ALLOC
		#include <cstdlib>
		#include <crtdbg.h>

		void win_detect_memory_leak(int goToLine)
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
			_CrtSetBreakAlloc(goToLine);
		}
	#else
		#define DETECT_MEMORY_LEAKS(goToLine)
	#endif
#else
	#define DETECT_MEMORY_LEAKS(goToLine)
#endif // _WIN32

typedef struct user_data
{
	int someValue;
} user_data_t;

int main()
{
	DETECT_MEMORY_LEAKS(-1);

	lq_byte_t* htmlBuffer = (lq_byte_t*)"<!DOCTYPE html><html><head><title>Test</title></head><body><h1>Hello CMake.</h1></body></html>";
	cout << htmlBuffer << endl;

	lq_context_t context = lq_context_create();
	lq_utf8_string_t htmlData = lq_utf8_string_create(context, htmlBuffer);
	lq_document_callbacks_t callbacks = {};
	callbacks.get_default_font_name = [](lq_context_t context, lq_document_t document) -> lq_utf8_string_t
		{
			const char* defaultFontName = "Noto Sans";
			return lq_utf8_string_create(context, reinterpret_cast<const lq_byte_t*>(defaultFontName));
		};
	callbacks.get_default_font_size = [](lq_context_t context, lq_document_t document) -> lq_pixel_t
		{
			return 16.0f;
		};
	callbacks.get_media_features = [](lq_context_t context, lq_document_t document, lq_media_features_t* out_media)
		{
			out_media->type = media_type_screen;
			out_media->width = 1920.0f;
			out_media->height = 1080.0f;
			out_media->device_width = 1920.0f;
			out_media->device_height = 1080.0f;
			out_media->color = 24;
		};
	user_data_t userData = { 42 };
	lq_document_t document = lq_document_create(context, htmlData, &callbacks, &userData);

	lq_context_destroy(context);

	return 0;
}
