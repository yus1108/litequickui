#pragma once

// MACRO DEFINITIONS
#if !defined(NULL)
	#if defined(__cplusplus)
		#define NULL 0
	#else
		#define NULL ((void *)0)
	#endif
#endif

#if defined(_WIN32) || defined(_WIN64)
	#if defined(LQ_CORE_STATIC)
		#define LQ_CORE_API
	#elif defined(LQ_CORE_EXPORTS)
		#define LQ_CORE_API __declspec(dllexport)
	#else
		#define LQ_CORE_API __declspec(dllimport)
	#endif
#else
	#define LQ_CORE_API
#endif

#define __LQ_WTEXT(x) L##x
#define LQ_WTEXT(x) __LQ_WTEXT(x)

#if defined(_DEBUG)
	#define LQ_DEBUG_ONLY(_Expression) _Expression
	#define LQ_DEBUG_ASSERT(_Expression, _Format, ...) if (!(_Expression)) { lq_debug_print_format_string(__FILE__, (unsigned int)(__LINE__), _Format, __VA_ARGS__); __debugbreak(); } ((void)0)
	#define LQ_WDEBUG_ASSERT(_Expression, _Format, ...) if (!(_Expression)) { lq_debug_print_format_wstring(LQ_WTEXT(__FILE__), (unsigned int)(__LINE__), _Format, __VA_ARGS__); __debugbreak(); } ((void)0)
#else
	#define LQ_DEBUG_ONLY(_Expression)
	#define LQ_DEBUG_ASSERT(_Expression, _Format, ...)
	#define LQ_WDEBUG_ASSERT(_Expression, _Format, ...)
	#define LQ_TDEBUG_ASSERT(_Expression, _Format, ...)
#endif
#define LQ_ASSERT(_Expression, _Format, ...) if (!(_Expression)) { lq_debug_print_format_string(__FILE__, (unsigned int)(__LINE__), _Format, __VA_ARGS__); __debugbreak(); } ((void)0)
#define LQ_UNUSED(...) __VA_ARGS__

#define LQ_STATIC_ASSERT(expr, msg) typedef char static_assertion_##msg[(expr) ? 1 : -1]

#define LQ_DEBUG_ENUM_COUNTER_BEGIN LQDebug({ constexpr const size_t LQ_INTERNAL_ENUM_BEGIN = __COUNTER__)
#define LQ_DEBUG_ENUM_COUNTER_COUNT LQDebug(__COUNTER__;)
#define LQ_DEBUG_ENUM_COUNTER_END(_EnumType, _ErrorMessage)                                              \
LQ_DEBUG_ONLY(constexpr const size_t LQ_INTERNAL_ENUM_COUNT = __COUNTER__ - 1 - LQ_INTERNAL_ENUM_BEGIN;) \
LQ_DEBUG_ONLY(static_assert(LQ_INTERNAL_ENUM_COUNT == static_cast<size_t>(_EnumType::COUNT), _ErrorMessage); })

LQ_CORE_API void lq_debug_print_format_string(const char* file, unsigned int line, const char* format, ...);
LQ_CORE_API void lq_debug_print_format_wstring(const wchar_t* file, unsigned int line, const wchar_t* format, ...);

#define LQ_UINT32_MAX 0xFFFFFFFF
#define LQ_INT32_MAX  0x7FFFFFFF
#define LQ_INT32_MIN  (-LQ_INT32_MAX - 1)

#define LQ_POOL_CAPACITY_MULTIPLIER 2