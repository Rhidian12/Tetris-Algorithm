#pragma once

#include "../Utils/Utils.h"
#include "../Point/Point.h"

#include <array> /* std::array */

class Board final
{
public:
	Board(class ScreenGrabber* pScreenGrabber);

	void Update(const uint64_t currentFrame);

	__NODISCARD bool IsCoordinateOccupied(const Point& coord) const;
	__NODISCARD bool IsAnyRowComplete(const std::array<Point, g_MaxNrOfBlocks>& points);
	__NODISCARD bool IsRowComplete(const uint8_t row) const;
	__NODISCARD uint8_t GetNrOfHoles() const;
	__NODISCARD uint8_t GetNewNrOfHoles(const std::array<Point, g_MaxNrOfBlocks>& points);
	__NODISCARD uint8_t GetBumpiness() const;
	__NODISCARD uint8_t GetNewBumpiness(const std::array<Point, g_MaxNrOfBlocks>& points);
	__NODISCARD uint8_t GetColHeight(const uint8_t col) const;
	__NODISCARD uint8_t GetAggregateHeight() const;
	__NODISCARD uint8_t GetNewAggregateHeight(const std::array<Point, g_MaxNrOfBlocks>& points);
	__NODISCARD const std::array<bool, m_BoardSize.x* m_BoardSize.y>& GetPreviousBoardState() const;
	__NODISCARD const std::array<bool, m_BoardSize.x* m_BoardSize.y>& GetBoardState() const;

private:
	void SetBoardState();
#ifdef _DEBUG
	void DebugBoardState() const;
#endif

	inline constexpr static Point m_ScreenStart{ 822L, 247L };
	inline constexpr static Point m_ScreenEnd{ 1172L, 889L };

	inline constexpr static Point m_BlockSize{ 34L,30L };
	inline constexpr static Point m_BlockOffset{ 6L, 4L };

	/* Board Information */
	std::array<bool, m_BoardSize.x* m_BoardSize.y> m_PreviousBoardState;
	std::array<bool, m_BoardSize.x* m_BoardSize.y> m_BoardState;
	bool m_IsPreviousBoardStateSet;

	class ScreenGrabber* m_pScreenGrabber;
};