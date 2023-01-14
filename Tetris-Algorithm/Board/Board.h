#pragma once

#include "../Config.h"
#include "../Delegate/Delegate.h"

#include <array> /* std::array */

class Board final
{
	using BoardStorage = std::array<std::array<bool, m_BoardSize.x>, m_BoardSize.y>;
public:
	Board(class ScreenGrabber* pScreenGrabber);

	void UpdatePrevious();

#ifdef _DEBUG
	void DebugBoardState() const;
#endif

	void Update(const bool getNextPiece);

	void Remove(const std::array<Tetris::Point, g_MaxNrOfBlocks>& points);
	void Add(const std::array<Tetris::Point, g_MaxNrOfBlocks>& points);

	__NODISCARD bool IsCoordinateOccupied(const Tetris::Point& coord) const;
	__NODISCARD bool IsAnyRowComplete(const std::array<Tetris::Point, g_MaxNrOfBlocks>& points);
	__NODISCARD bool IsRowComplete(const int row) const;
	__NODISCARD int GetNrOfHoles() const;
	__NODISCARD int GetNewNrOfHoles(const std::array<Tetris::Point, g_MaxNrOfBlocks>& points);
	__NODISCARD int GetBumpiness() const;
	__NODISCARD int GetNewBumpiness(const std::array<Tetris::Point, g_MaxNrOfBlocks>& points);
	__NODISCARD int GetColHeight(const int col) const;
	__NODISCARD int GetAggregateHeight() const;
	__NODISCARD int GetNewAggregateHeight(const std::array<Tetris::Point, g_MaxNrOfBlocks>& points);
	__NODISCARD bool DoesRowContainPieces(const int row) const;
	__NODISCARD const BoardStorage& GetPreviousBoardState() const;
	__NODISCARD const BoardStorage& GetBoardState() const;
	__NODISCARD const std::array<std::array<bool, 4>, 2>& GetNextPiece() const;
	__NODISCARD Delegate<>& GetOnNewPieceSpawned();
	__NODISCARD int GetNrOfCompletedLines() const;
	__NODISCARD int GetNewNrOfCompletedLines(const std::array<Tetris::Point, g_MaxNrOfBlocks>& points);
	__NODISCARD int GetHighestRow() const;
	__NODISCARD int GetNrOfPiecesInRow(const int row) const;
	__NODISCARD const Tetris::Point& GetBoardSize() const { return m_BoardSize; }

private:
	void SetBoardState(const bool getNextPiece);

	inline constexpr static Tetris::Point m_ScreenStart{ 822L, 887L };

	inline constexpr static Tetris::Point m_NextStart{ 1302L, 577L };

	inline constexpr static Tetris::Point m_BlockSize{ 34L, 29L };
	inline constexpr static Tetris::Point m_BlockOffset{ 5L, 5L };

	/* Board Information */
	BoardStorage m_PreviousBoardState;
	BoardStorage m_BoardState;
	std::array<std::array<bool, 4>, 2> m_NextPiece;
	bool m_IsPreviousBoardStateSet;

	Delegate<> m_OnNewPieceSpawned;

	class ScreenGrabber* m_pScreenGrabber;
};