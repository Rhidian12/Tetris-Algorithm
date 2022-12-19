#pragma once

#include <Math/TypeDefines.h>

#include "../Logger/Logger.h"

#include <array> /* std::array */

#define WIN32_LEAN_AND_MEAN
#include <Windows.h> /* Windows stuff */

inline constexpr static uint32_t g_ScreenWidth{ 920u }; // min 400
inline constexpr static uint32_t g_ScreenHeight{ 720u }; // min 680
inline constexpr const char g_TetrisWindow[13]{ "NES - Tetris" };
inline constexpr const char g_VirtualPadsWindow[13]{ "Virtual Pads" };
inline const static Integrian2D::Point2f g_BoardSize{ 10.f, 22.f };
inline const static Integrian2D::Point2f g_BlockSize{ 34.f,30.f };
inline const static Integrian2D::Point2f g_BlockOffset{ 6.f, 4.f };
inline constexpr static uint32_t g_NrOfBoard{ 220u };
inline constexpr static uint8_t g_MaxNrOfBlocks{ 4u };

namespace Utils
{
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