#pragma once

#include "../Logger/Logger.h"
#include "../Point/Point.h"

#include <array> /* std::array */

#define WIN32_LEAN_AND_MEAN
#include <Windows.h> /* Windows stuff */

inline volatile LONG g_ScreenWidth{};
inline volatile LONG g_ScreenHeight{};
inline constexpr const char g_TetrisWindow[13]{ "NES - Tetris" };
inline constexpr const char g_VirtualPadsWindow[13]{ "Virtual Pads" };

namespace Utils
{
	void InitializeTetris();
	void GetDesktopResolution(volatile LONG& x, volatile LONG& y);
	void ConvertNDCToScreenCoords(LONG& x, LONG& y, const Point& wantedCoords);
	void TakeScreenshot(HDC& hdc, HDC& hDest, HBITMAP& hbDesktop);
	void CleanupBitMap(HDC& hdc, HDC& hDest, HBITMAP& hbDesktop);

	template<typename T, size_t N>
	void ResetArray(std::array<T, N>& arr)
	{
		for (size_t i{}; i < N; ++i)
			arr[i] = T{};
	}

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

#define __NODISCARD [[nodiscard]]
}