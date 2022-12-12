#pragma once

#include "../Point/Point.h"
#include "../Tetromino/Tetromino.h"

#include <string> /* std::string */
#include <array> /* std::array */

class TetrisAlgorithm final
{
public:
	TetrisAlgorithm() = default;

	void Initialize();
	void Start();

private:
	void SendMousePress(const Point& coords) const;
	void TakeScreenshot();
	void GetBoardState();
	void FindCurrentPiece();
	void CalculateBestMove();

	[[nodiscard]] uint8_t GetRowIndex(const uint8_t index) const;
	[[nodiscard]] uint8_t GetColumnIndex(const uint8_t index) const;

#ifdef _DEBUG
	void DebugBoardState() const;
#endif

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

	/* Program Information */
	std::string m_ModuleName;
	uint64_t m_CurrentFrame;
	bool m_IsPreviousBoardStateSet;

	/* Board Information */
	std::array<bool, m_BoardSize.x* m_BoardSize.y> m_PreviousBoardState;
	std::array<bool, m_BoardSize.x* m_BoardSize.y> m_BoardState;

	/* Tetris Information */
	Tetromino m_CurrentPiece;
};