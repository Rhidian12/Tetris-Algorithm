#pragma once

#include "../Utils/Utils.h"
#include "../Point/Point.h"
#include "../Tetromino/Tetromino.h"

#include <string> /* std::string */
#include <array> /* std::array */
#include <queue> /* std::queue */

class TetrisAlgorithm final
{
public:
	TetrisAlgorithm(class Board* pBoard);

	void Update(const uint64_t currentFrame);

	__NODISCARD bool IsExecutingBestMove() const;

private:
	struct MoveToExecute final
	{
		std::array<Point, 4> TargetPos;
		uint8_t TargetRotation;
	};

	void SendMousePress(const Point& coords) const;
	void FindCurrentPiece();
	void CalculateBestMove();
	void ExecuteBestMove(const uint64_t currentFrame);
	void CalculateClicksToExecute();
	__NODISCARD int8_t EvaluatePosition(const std::array<Point, 4>& points) const;

	__NODISCARD uint8_t GetRowIndex(const uint8_t index) const;
	__NODISCARD uint8_t GetColumnIndex(const uint8_t index) const;

	inline constexpr static Point m_LeftCoords{ 84L, 252L };
	inline constexpr static Point m_DownCoords{ 111L, 306L };
	inline constexpr static Point m_UpCoords{ 111L, 239L };
	inline constexpr static Point m_RightCoords{ 138L, 252L };
	inline constexpr static Point m_PadsCoords{ 80L, 168L };
	inline constexpr static Point m_StickyCoords{ 110L, 220L };
	inline constexpr static Point m_RotateRight{ 275L, 828L };
	inline constexpr static Point m_RotateLeft{ 246L, 828L };

	inline constexpr static Point m_NextStart{ 1273L, 550L };
	inline constexpr static Point m_NextEnd{ 1419L, 550L };

	/* Algorithm Parameters */
	inline constexpr static int8_t m_ClearLineScore{ static_cast<int8_t>(150) };
	inline constexpr static int8_t m_HoleScore{ static_cast<int8_t>(-50) };
	inline constexpr static int8_t m_BumpinessScore{ static_cast<int8_t>(-30) };
	inline constexpr static int8_t m_AggregateHeightScore{ static_cast<int8_t>(-40) };

	/* Program Information */
	bool m_IsBestMoveCalculated;
	class Board* m_pBoard;
	MoveToExecute m_BestMove;
	bool m_IsExecutingBestMove;
	std::queue<Point> m_ClicksToExecute;

	/* Tetris Information */
	Tetromino m_CurrentPiece;
};