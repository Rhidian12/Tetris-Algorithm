#pragma once

#include "../Logger/Logger.h"

namespace Utils
{
#ifdef _DEBUG
#define __BREAK() __debugbreak()
#define __ASSERT(expr) \
	if ((expr)) {} \
	else \
	{ \
		Debug::Logger::GetInstance().LogAssertion(#expr, __LINE__, __FILE__, Debug::MessageColour::Yellow, true); \
		__BREAK(); \
	}
#define __CASSERT(expr) \
	if constexpr ((expr)) {} \
	else \
	{ \
		Debug::Logger::GetInstance().LogAssertion(#expr, __LINE__, __FILE__, Debug::MessageColour::Yellow, true); \
		__BREAK(); \
	}
#else
#define __ASSERT(expr)
#define __CASSERT(expr)
#endif
}