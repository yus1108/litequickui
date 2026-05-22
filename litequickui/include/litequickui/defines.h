#pragma once

// MACRO DEFINITIONS
#if defined(_WIN32) || defined(_WIN64)
	#if defined(LQ_STATIC)
		#define LQ_API
	#elif defined(LQ_EXPORTS)
		#define LQ_API __declspec(dllexport)
	#else
		#define LQ_API __declspec(dllimport)
	#endif
#else
	#define LQ_API
#endif