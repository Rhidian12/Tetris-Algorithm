#pragma once

#include "../Utils/Utils.h"
#include "../Point/Point.h"
#include "../Tetromino/Tetromino.h"
#include "../Timer/Timepoint/Timepoint.h"

#include <string> /* std::string */
#include <array> /* std::array */
#include <queue> /* std::queue */

class TetrisAlgorithm final
{
public:
	TetrisAlgorithm(class Board* pBoard);

	void OnNewPieceSpawned();
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
	void ExecuteBestMove();
	void CalculateClicksToExecute();
	__NODISCARD float EvaluatePosition(const std::array<Point, 4>& points) const;

	__NODISCARD uint8_t GetRowIndex(const uint8_t index) const;
	__NODISCARD uint8_t GetColumnIndex(const uint8_t index) const;

	inline constexpr static Point m_LeftCoords{ 84L, 252L };
	inline constexpr static Point m_DownCoords{ 111L, 306L };
	inline constexpr static Point m_UpCoords{ 111L, 239L };
	inline constexpr static Point m_RightCoords{ 138L, 252L };
	inline constexpr static Point m_PadsCoords{ 80L, 168L };
	inline constexpr static Point m_StickyCoords{ 110L, 220L };
	inline constexpr static Point m_RotateRight{ 275L, 252L };
	inline constexpr static Point m_RotateLeft{ 246L, 252L };
	inline constexpr static Point m_VirtualPadWindowCoord{ 324L, 369L };

	inline constexpr static Point m_NextStart{ 1273L, 550L };
	inline constexpr static Point m_NextEnd{ 1419L, 550L };

	/* Algorithm Parameters */
	inline constexpr static float m_ClearLineWeight{ 0.760666f };
	inline constexpr static float m_HoleWeight{ -0.65663f };
	inline constexpr static float m_BumpinessWeight{ -0.184483f };
	inline constexpr static float m_AggregateHeightWeight{ -0.710066f };
	inline constexpr static float m_MoveWeight{ -0.05f };

	/* Program Information */
	bool m_IsBestMoveCalculated;
	class Board* m_pBoard;
	MoveToExecute m_BestMove;
	bool m_IsExecutingBestMove;
	std::queue<Point> m_ClicksToExecute;
	Timepoint m_ClickStart;
	bool m_Cooldown;
	Timepoint m_CooldownStart;

	/* Tetris Information */
	Tetromino m_CurrentPiece;
	uint64_t m_NrOfPieces;
	bool m_HasNewPieceSpawned;
};