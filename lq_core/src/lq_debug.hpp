#pragma once

// MACRO DEFINITIONS
#if defined(_DEBUG)
#define WTEXT(x) L##x
#define LQ_DEBUG_ONLY(_Expression) _Expression
#define LQ_DEBUG_ASSERT(_Expression, _Format, ...) if (!(_Expression)) { lq_debug_print_format_string(__FILE__, (unsigned)(__LINE__), _Format, __VA_ARGS__); __debugbreak(); } ((void)0)
#define LQ_WDEBUG_ASSERT(_Expression, _Format, ...) if (!(_Expression)) { lq_debug_print_format_wstring(WTEXT(__FILE__), (unsigned)(__LINE__), _Format, __VA_ARGS__); __debugbreak(); } ((void)0)
#else
#define LQ_DEBUG_ONLY(_Expression)
#define LQ_DEBUG_ASSERT(_Expression, _Format, ...)
#define LQ_WDEBUG_ASSERT(_Expression, _Format, ...)
#define LQ_TDEBUG_ASSERT(_Expression, _Format, ...)
#endif
#define LQ_UNUSED(...) __VA_ARGS__

#define LQ_STATIC_ASSERT(expr, msg) typedef char static_assertion_##msg[(expr) ? 1 : -1]

#define LQ_DEBUG_ENUM_COUNTER_BEGIN LQDebug({ constexpr const size_t LQ_INTERNAL_ENUM_BEGIN = __COUNTER__)
#define LQ_DEBUG_ENUM_COUNTER_COUNT LQDebug(__COUNTER__;)
#define LQ_DEBUG_ENUM_COUNTER_END(_EnumType, _ErrorMessage)                                              \
LQ_DEBUG_ONLY(constexpr const size_t LQ_INTERNAL_ENUM_COUNT = __COUNTER__ - 1 - LQ_INTERNAL_ENUM_BEGIN;) \
LQ_DEBUG_ONLY(static_assert(LQ_INTERNAL_ENUM_COUNT == static_cast<size_t>(_EnumType::COUNT), _ErrorMessage); })

void lq_debug_print_format_string(const char* file, unsigned line, const char* format, ...);
void lq_debug_print_format_wstring(const wchar_t* file, unsigned line, const wchar_t* format, ...);
