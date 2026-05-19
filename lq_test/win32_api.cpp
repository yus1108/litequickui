#if defined(_WIN32) || defined(_WIN64)
#include "win32_api.h"
#include "win32_api.hpp"

#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "imm32.lib")
#pragma comment(lib, "windowsapp")

#include <mutex>
#include <memory>

lq_bool_t win32_set_console_output_cp(lq_uint32_t code_page)
{
	return SetConsoleOutputCP(code_page) ? lq_true : lq_false;
}

const lq_wchar_t* win32_get_default_class_name(void)
{
	static const lq_wchar_t* class_name = NULL;
	static std::mutex mutex;

	std::lock_guard<std::mutex> lock(mutex);
	if (class_name == NULL)
	{
		class_name = L"Default Window Class";
		WNDCLASSW wc = {};
		wc.lpfnWndProc = &WndProc;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetModuleHandle(NULL);
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = class_name;

		if (RegisterClassW(&wc) == FALSE)
		{
			LQ_DEBUG_ASSERT(false, "Failed to register window class. Error code: %lu", GetLastError());
			return NULL;
		}
	}
	return class_name;
}

lq_bool_t win32_app_is_valid(win32_app_t app)
{
	return (app != NULL) && (app->hwnd != NULL);
}

win32_app_t win32_app_create(lq_utf8_str_t title, lq_uintptr_t user_data, const win32_app_callbacks_t* callbacks, const lq_rect_t* rect)
{
	const lq_wchar_t* class_name = win32_get_default_class_name();
	if (class_name == NULL) { return NULL; }

	win32_app_t app = new struct win32_app;
	if (app == NULL)
	{
		LQ_DEBUG_ASSERT(false, "Failed to allocate memory for win32_app struct.");
		return NULL;
	}
	app->user_data = user_data;
	app->callbacks = *callbacks;
	app->hwnd = NULL;

	const lq_char_t* title_cstr = lq_utf8_str_get_cstr(title);
	lq_int32_t wchar_count = MultiByteToWideChar(CP_UTF8, 0, title_cstr, -1, NULL, 0);
	std::unique_ptr<wchar_t[]> title_wstr = std::make_unique<wchar_t[]>(wchar_count);
	MultiByteToWideChar(CP_UTF8, 0, title_cstr, -1, title_wstr.get(), wchar_count);

	int x, y, width, height;
	if (rect != NULL)
	{
		x = static_cast<int>(roundf(rect->x));
		y = static_cast<int>(roundf(rect->y));
		width = static_cast<int>(roundf(rect->width));
		height = static_cast<int>(roundf(rect->height));
	}
	else
	{
		x = CW_USEDEFAULT;
		y = 0;
		width = CW_USEDEFAULT;
		height = 0;
	}

	CreateWindowExW(0, class_name, title_wstr.get(), 0, x, y, width, height, NULL, NULL, GetModuleHandle(NULL), (LPVOID)app);
	if (!app->hwnd) {
		MessageBoxW(NULL, L"CreateWindowExW 실패", L"Error", MB_OK);
		return NULL;
	}

	if (SetWindowLong(app->hwnd, GWL_STYLE, WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME) == 0)
	{
		LQ_DEBUG_ASSERT(false, "Failed to set window style. Error code: %lu", GetLastError());
		DestroyWindow(app->hwnd);
		delete app;
		return NULL;
	}

	if (UpdateWindow(app->hwnd) == FALSE)
	{
		LQ_DEBUG_ASSERT(false, "Failed to update window. Error code: %lu", GetLastError());
		DestroyWindow(app->hwnd);
		delete app;
		return NULL;
	}

	return app;
}

void win32_app_destroy(win32_app_t app)
{
	LQ_DEBUG_ASSERT(app != NULL, "Input app must not be null.");
	if (win32_app_is_valid(app)) { DestroyWindow(app->hwnd); }
	delete app;
}

lq_pixel_t win32_app_get_dpi(win32_app_t app)
{
	LQ_DEBUG_ASSERT(win32_app_is_valid(app), "Input app must be valid.");
	return static_cast<lq_pixel_t>(GetDpiForWindow(app->hwnd));
}

lq_pixel2_t win32_app_get_monitor_resolution(win32_app_t app)
{
	LQ_DEBUG_ASSERT(win32_app_is_valid(app), "Input app must be valid.");

	HMONITOR monitor_handle = MonitorFromWindow(app->hwnd, MONITOR_DEFAULTTONEAREST);

	MONITORINFO monitor_info = {};
	monitor_info.cbSize = sizeof(MONITORINFO);

	if (GetMonitorInfo(monitor_handle, &monitor_info) == FALSE)
	{
		LQ_DEBUG_ASSERT(false, "Failed to get monitor info. Error code: %lu", GetLastError());
		return lq_pixel2_t();
	}

	lq_pixel2_t resolution;
	resolution.x = static_cast<lq_pixel_t>(monitor_info.rcMonitor.right - monitor_info.rcMonitor.left);
	resolution.y = static_cast<lq_pixel_t>(monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top);
	return resolution;
}

lq_uintptr_t win32_app_get_user_data(win32_app_t app)
{
	LQ_DEBUG_ASSERT(win32_app_is_valid(app), "Input app must be valid.");
	return app->user_data;
}

lq_pixel2_t win32_app_get_window_size(win32_app_t app)
{
	LQ_DEBUG_ASSERT(win32_app_is_valid(app), "Input app must be valid.");

	RECT rect;
	if (GetClientRect(app->hwnd, &rect) == FALSE)
	{
		LQ_DEBUG_ASSERT(false, "Failed to get window size. Error code: %lu", GetLastError());
		return lq_pixel2_t();
	}

	lq_pixel2_t size;
	size.x = static_cast<lq_pixel_t>(rect.right - rect.left);
	size.y = static_cast<lq_pixel_t>(rect.bottom - rect.top);
	return size;
}

void win32_app_set_window_title(win32_app_t app, const lq_utf8_str_t title)
{
	LQ_DEBUG_ASSERT(win32_app_is_valid(app), "Input app must be valid.");
	LQ_DEBUG_ASSERT(title != NULL, "Input title must not be null.");

	const lq_byte_t* title_bytes = lq_utf8_str_get_bytes(title);
	win32_app_set_window_title_utf8_bytes(app, title_bytes);
}

void win32_app_set_window_title_utf8_bytes(win32_app_t app, const lq_byte_t* utf8_bytes_title)
{
	LQ_DEBUG_ASSERT(win32_app_is_valid(app), "Input app must be valid.");
	LQ_DEBUG_ASSERT(utf8_bytes_title != NULL, "Input title must not be null.");

	const lq_char_t* title_cstr = reinterpret_cast<const lq_char_t*>(utf8_bytes_title);
	lq_int32_t wchar_count = MultiByteToWideChar(CP_UTF8, 0, title_cstr, -1, NULL, 0);
	std::unique_ptr<wchar_t[]> title_wstr = std::make_unique<wchar_t[]>(wchar_count);
	MultiByteToWideChar(CP_UTF8, 0, title_cstr, -1, title_wstr.get(), wchar_count);

	SetWindowTextW(app->hwnd, title_wstr.get());
}

lq_bool_t win32_app_show(win32_app_t app)
{
	LQ_DEBUG_ASSERT(win32_app_is_valid(app), "Input app must be valid.");
	ShowWindow(app->hwnd, SW_SHOW);
	return lq_true;
}

lq_bool_t win32_app_update(win32_app_t app)
{
	LQ_DEBUG_ASSERT(win32_app_is_valid(app), "Input app must be valid.");

	MSG msg;
	while (TRUE == PeekMessage(&msg, app->hwnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return win32_app_is_valid(app);
}

#endif