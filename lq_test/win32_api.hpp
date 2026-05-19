#pragma once
#if defined(_WIN32) || defined(_WIN64)

#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "imm32.lib")
#pragma comment(lib, "windowsapp")

#include "win32_api.h"

#include <dwmapi.h>
#include <Windows.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.UI.ViewManagement.h>

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

#endif