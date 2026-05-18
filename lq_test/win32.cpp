#if defined(_WIN32) || defined(_WIN64)
	#include "lq_test.h"

	#pragma comment(lib, "Dwmapi.lib")
	#pragma comment(lib, "imm32.lib")
	#pragma comment(lib, "windowsapp")

	#include <dwmapi.h>
	#include <mutex>
	#include <memory>
	#include <Windows.h>
	#include <winrt/Windows.Foundation.Collections.h>
	#include <winrt/Windows.UI.ViewManagement.h>

	#include <lq_core.h>

	typedef struct win32_app* win32_app_t;

	typedef struct win32_app_callbacks
	{
		void (*create)(win32_app_t app);
		void (*destroy)(win32_app_t app);
	} win32_app_callbacks_t;

	typedef struct win32_app
	{
		HWND hwnd; // Window handle
		winrt::Windows::UI::ViewManagement::UISettings winrt_ui_viewmanagement_uisettings;
		winrt::event_token winrt_ui_viewmanagement_eventtokencolorsvaluechanged;
		lq_uintptr_t user_data; // User data passed during window creation
		win32_app_callbacks_t callbacks; // Callbacks for window events
	} *win32_app_t;

	static inline bool winrt_is_light_color(winrt::Windows::UI::Color& clr)
	{
		return (((5 * clr.G) + (2 * clr.R) + clr.B) > (8 * 128));
	}

	static inline LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) noexcept
	{
		switch (msg)
		{
		case WM_CREATE:
		{
			LPCREATESTRUCT create_struct = reinterpret_cast<LPCREATESTRUCT>(l_param);
			win32_app_t app = reinterpret_cast<win32_app_t>(create_struct->lpCreateParams);
			LQ_DEBUG_ASSERT(app != NULL, "Invalid userdata");
			LQ_DEBUG_ASSERT(app->hwnd == NULL, "HWND in userdata does not match the created window's HWND");
			app->hwnd = hwnd;

			SetLastError(0);
			LONG_PTR lastAddress = SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(app));
			if (lastAddress == 0)
			{
				// SetWindowLongPtr function returns non-zero for both returned value and GetLastError() if it's an error.
				DWORD lastError = GetLastError();
				if (lastError != 0)
				{
					return lastError;
				}
			}

			winrt::init_apartment();
			// Apply proper light/dark theme title bar
			app->winrt_ui_viewmanagement_uisettings = winrt::Windows::UI::ViewManagement::UISettings();
			auto foreground = app->winrt_ui_viewmanagement_uisettings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Foreground);
			BOOL isDarkMode = static_cast<BOOL>(winrt_is_light_color(foreground));
			::DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &isDarkMode, sizeof(isDarkMode));

			// Add ColorValuesChanged event handler to apply proper light/dark theme title bar
			app->winrt_ui_viewmanagement_eventtokencolorsvaluechanged = app->winrt_ui_viewmanagement_uisettings.ColorValuesChanged
			(
				[hwnd, app](auto&&...)
				{
					auto foregroundRevoker = app->winrt_ui_viewmanagement_uisettings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Foreground);
					BOOL isDarkModeRevoker = static_cast<BOOL>(winrt_is_light_color(foregroundRevoker));
					::DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &isDarkModeRevoker, sizeof(isDarkModeRevoker));
				}
			);

			LQ_DEBUG_ASSERT(app->callbacks.create != NULL, "on_create callback must be provided");
			app->callbacks.create(app);
			break;
		}
		case WM_DESTROY:
		{
			win32_app_t app = reinterpret_cast<win32_app_t>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			LQ_DEBUG_ASSERT(app != NULL, "Invalid userdata");

			LQ_DEBUG_ASSERT(app->callbacks.destroy != NULL, "on_destroy callback must be provided");
			app->callbacks.destroy(app);

			// Remove ColorValuesChanged event handler
			app->winrt_ui_viewmanagement_uisettings.ColorValuesChanged(app->winrt_ui_viewmanagement_eventtokencolorsvaluechanged);
			app->hwnd = NULL;
			PostQuitMessage(0);
			return 0;
		}
		default:
			return DefWindowProc(hwnd, msg, w_param, l_param);
		};
		return 0;
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

	lq_uintptr_t win32_app_get_user_data(win32_app_t app)
	{
		LQ_DEBUG_ASSERT(win32_app_is_valid(app), "Input app must be valid.");
		return app->user_data;
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

	lq_bool_t test_win32_api_implmenetation()
	{
		typedef struct win32_app_user_data
		{
			lq_utf8_str_t title;
		} win32_app_user_data_t;
		win32_app_user_data_t app_data = {};
		app_data.title = lq_utf8_str_create("한글 윈도우");

		win32_app_callbacks_t callbacks = {};
		callbacks.create = [](win32_app_t app)
			{
				win32_app_user_data_t* appData = (win32_app_user_data_t*)win32_app_get_user_data(app);

				const lq_char_t* title_cstr = lq_utf8_str_get_cstr(appData->title);
				lq_int32_t wchar_count = MultiByteToWideChar(CP_UTF8, 0, title_cstr, -1, NULL, 0);
				lq_wchar_t* title_wcstr = (wchar_t*)malloc(sizeof(wchar_t) * wchar_count);
				MultiByteToWideChar(CP_UTF8, 0, title_cstr, -1, title_wcstr, wchar_count);
				OutputDebugStringW(title_wcstr);
				free(title_wcstr);
				OutputDebugStringA(" Window created.\n");
			};
		callbacks.destroy = [](win32_app_t app)
			{
				win32_app_user_data_t* appData = (win32_app_user_data_t*)win32_app_get_user_data(app);

				const lq_char_t* title_cstr = lq_utf8_str_get_cstr(appData->title);
				lq_int32_t wchar_count = MultiByteToWideChar(CP_UTF8, 0, title_cstr, -1, NULL, 0);
				lq_wchar_t* title_wcstr = (wchar_t*)malloc(sizeof(wchar_t) * wchar_count);
				MultiByteToWideChar(CP_UTF8, 0, title_cstr, -1, title_wcstr, wchar_count);
				OutputDebugStringW(title_wcstr);
				free(title_wcstr);
				OutputDebugStringA(" Window destroyed.\n");
			};

		win32_app_t app = win32_app_create(app_data.title, (lq_uintptr_t)&app_data, &callbacks, NULL);
		if (app == NULL) { return lq_false; }

		win32_app_show(app);

		while (win32_app_update(app))
		{
		}
	
		win32_app_destroy(app);
		lq_utf8_str_destroy(app_data.title);
		return lq_true;
	}
#endif