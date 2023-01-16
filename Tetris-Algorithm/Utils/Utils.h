#pragma once

#include "../Config.h"

#include "../Logger/Logger.h"
#include "../Point/Point.h"
#include "../Tetromino/Tetromino.h"
#include "../Board/Board.h"

#include <array> /* std::array */
#include <numeric> /* std::numeric_limits */
#include <type_traits> /* std::is_fundamental_v, ... */

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
	void ConvertNDCToScreenCoords(LONG& x, LONG& y, const volatile Tetris::Point& wantedCoords);
	void SendMousePress(const volatile Tetris::Point& coords);
	Tetromino FindCurrentPiece(Board* pBoard);
	Tetromino FindNextPiece(Board* pBoard);

	template<typename T, size_t N>
	void ResetArray(std::array<T, N>& arr)
	{
		for (size_t i{}; i < N; ++i)
			arr[i] = T{};
	}

	template<typename T, typename = std::enable_if_t<std::is_fundamental_v<T>>>
	__forceinline constexpr bool AreEqual(const T a, const T b, const T epsilon = std::numeric_limits<T>::epsilon())
	{
		return static_cast<T>(abs(a - b)) <= epsilon;
	}
}