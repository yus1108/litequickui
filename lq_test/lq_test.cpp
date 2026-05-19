#include "lq_test.h"

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

int main(void)
{
	DETECT_MEMORY_LEAKS(-1);

	//WCHAR localeName[LOCALE_NAME_MAX_LENGTH];
	//GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH);
	//LANGID langId = GetUserDefaultUILanguage();
	//WCHAR langCode[9];
	//GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT, LOCALE_SISO639LANGNAME, langCode, 9);
	//lq_debug_print_format_wstring(LQ_WTEXT(__FILE__), (unsigned int)(__LINE__), L"User default locale: %s, UI language: %u, ISO 639 language code: %s", localeName, langId, langCode);

	lq_bool_t result = test_lq_document_minimal_implementation();
	if (result == lq_false)
	{
		LQ_ASSERT(lq_false, "lq_document minimal implementation test failed");
		return -1;
	}

#if defined(_WIN32) || defined(_WIN64)
	result = test_win32_api_implmenetation();
	if (result == lq_false)
	{
		LQ_ASSERT(lq_false, "Win32 API implementation test failed");
		return -1;
	}
#endif

	return 0;
}