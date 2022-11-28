#pragma once

#include "../Point/Point.h"

#include <string> /* std::string */
#include <array> /* std::array */

class TetrisAlgorithm final
{
public:
	TetrisAlgorithm() = default;

	void Start();

private:
	void SendMousePress(const Point& coords);
	void TakeScreenshot();
	void GetBoardState();

	inline constexpr static Point m_LeftCoords{ 84L, 252L };
	inline constexpr static Point m_DownCoords{ 111L, 306L };
	inline constexpr static Point m_UpCoords{ 111L, 239L };
	inline constexpr static Point m_RightCoords{ 138L, 252L };

	inline constexpr static Point m_BlockSize{ 34L,30L };
	inline constexpr static Point m_BlockOffset{ 6L, 4L };

	inline constexpr static Point m_ScreenStart{ 822L, 247L };
	inline constexpr static Point m_ScreenEnd{ 1172L, 889L };

	inline constexpr static Point m_NextStart{ 1273L, 550L };
	inline constexpr static Point m_NextEnd{ 1419L, 550L };

	inline constexpr static Point m_BoardSize{ 10L, 20L };

	std::string m_ModuleName;
	std::array<bool, m_BoardSize.x* m_BoardSize.y> m_PreviousBoardState;
	std::array<bool, m_BoardSize.x* m_BoardSize.y> m_BoardState;
};