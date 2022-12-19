#pragma once

#include "Integrian2D_Config.h"

inline bool volatile g_IsLooping{ true }; // Maybe move this somewhere else

#ifdef _WIN32
	#ifdef EXPORT
		#define INTEGRIAN2D_API __declspec(dllexport)
	#else
		#define INTEGRIAN2D_API __declspec(dllimport)
	#endif
#else
	#define INTEGRIAN2D_API
#endif