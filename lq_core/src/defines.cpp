#include "lq_core/defines.h"

#include <cwchar>
#include <cstdio>
#include <memory>
#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif

inline void lq_print_debug_output_string(const char* formatBuffer)
{
#if defined(_WIN32) || defined(_WIN64)
	OutputDebugStringA(formatBuffer);
#else
	fprintf(stderr, formatBuffer);
#endif
}

inline void lq_print_debug_output_wstring(const wchar_t* formatBuffer)
{
#if defined(_WIN32) || defined(_WIN64)
	OutputDebugStringW(formatBuffer);
#else
	wfprintf(stderr, formatBuffer);
#endif
}

void lq_debug_print_format_string(const char* file, unsigned int line, const char* format, ...)
{
	constexpr const char DEBUG_OUTPUT_FORMAT[] = "%s(%u) %s\n";

	// evaluate buffer count for formatted string
	va_list argPtr;
	va_start(argPtr, format);
	const size_t formatBufferCount = static_cast<size_t>(std::vsnprintf(NULL, 0, format, argPtr)) + 1;
	va_end(argPtr);

	// allocate memory using the evaluated length
	std::unique_ptr<char[]> formatBuffer = std::make_unique<char[]>(formatBufferCount);
	if (formatBuffer == nullptr)
	{
		lq_print_debug_output_string("Failed allocate memory for debug output");
		return;
	}

	// format string and print it to debug output
	va_start(argPtr, format);
	std::vsnprintf(formatBuffer.get(), formatBufferCount, format, argPtr);
	va_end(argPtr);

	// allocate memory for output and format string
	const size_t outputCount = static_cast<size_t>(std::snprintf(NULL, 0, DEBUG_OUTPUT_FORMAT, file, line, formatBuffer.get())) + 1;
	std::unique_ptr<char[]> output = std::make_unique<char[]>(outputCount);
	if (output == nullptr)
	{
		// deallocate memory for format buffer
		lq_print_debug_output_string("Failed allocate memory for debug output");
		return;
	}
	std::snprintf(output.get(), outputCount, DEBUG_OUTPUT_FORMAT, file, line, formatBuffer.get());

	// print debug output
	lq_print_debug_output_string(output.get());
}

void lq_debug_print_format_wstring(const wchar_t* file, unsigned int line, const wchar_t* format, ...)
{
#if defined(_WIN32) || defined(_WIN64)
	constexpr const wchar_t DEBUG_OUTPUT_FORMAT[] = L"%s(%u) %s\n";

	// evaluate buffer count for formatted string
	va_list argPtr;
	va_start(argPtr, format);
	const size_t formatBufferCount = static_cast<size_t>(std::vswprintf(NULL, 0, format, argPtr)) + 1;
	va_end(argPtr);

	// allocate memory using the evaluated length
	std::unique_ptr<wchar_t[]> formatBuffer = std::make_unique<wchar_t[]>(formatBufferCount);
	if (formatBuffer == nullptr)
	{
		lq_print_debug_output_string("Failed allocate memory for debug output");
		return;
	}

	// format string and print it to debug output
	va_start(argPtr, format);
	std::vswprintf(formatBuffer.get(), formatBufferCount, format, argPtr);
	va_end(argPtr);

	// allocate memory for output and format string
	const size_t outputCount = static_cast<size_t>(std::swprintf(nullptr, 0, DEBUG_OUTPUT_FORMAT, file, line, formatBuffer.get())) + 1;
	std::unique_ptr<wchar_t[]> output = std::make_unique<wchar_t[]>(outputCount);
	if (output == nullptr)
	{
		// deallocate memory for format buffer
		lq_print_debug_output_string("Failed allocate memory for debug output");
		return;
	}
	std::swprintf(output.get(), outputCount, DEBUG_OUTPUT_FORMAT, file, line, formatBuffer.get());
	// print debug output
	lq_print_debug_output_wstring(output.get());
#else
	#error "TODO: Implement debug output for non-Windows platforms."
#endif
}
