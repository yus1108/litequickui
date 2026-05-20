#pragma once

// MACRO DEFINITIONS
#if defined(_WIN32) || defined(_WIN64)
	#if defined(LQ_HB_FT_STATIC)
		#define LQ_HB_FT_API
	#elif defined(LQ_HB_FT_EXPORTS)
		#define LQ_HB_FT_API __declspec(dllexport)
	#else
		#define LQ_HB_FT_API __declspec(dllimport)
	#endif
#else
	#define LQ_HB_FT_API
#endif