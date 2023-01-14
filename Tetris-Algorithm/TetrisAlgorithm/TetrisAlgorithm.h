#pragma once

#include "../Macros/Macros.h"
#include "../Point/Point.h"
#include "../Tetromino/Tetromino.h"
#include "../Timer/Timepoint/Timepoint.h"

#include <string> /* std::string */
#include <array> /* std::array */
#include <queue> /* std::queue */
#include <unordered_map> /* std::unordered_map */

class TetrisAlgorithm
{
public:
	TetrisAlgorithm(class Board* pBoard);
	virtual ~TetrisAlgorithm() = default;

	virtual void CalculateBestMove() = 0;
	virtual bool NeedsNextPiece() const = 0;

	void OnNewPieceSpawned();
	void Update();
	int GetSpeed() const;

#pragma region DEBUGGING
	void ApplyBestMove();
	void SetCurrentPiece(const Tetromino& tetromino);
#pragma endregion

	__NODISCARD bool IsExecutingBestMove() const;

protected:
	struct MoveToExecute final
	{
		std::array<Tetris::Point, 4> TargetPos;
		int TargetRotation;
	};
	struct Click final
	{
		volatile Tetris::Point ClickCoord;
		volatile double Delay;
	};

	void ExecuteBestMove();
	void CalculateClicksToExecute();
	__NODISCARD float EvaluatePosition(const std::array<Tetris::Point, 4>& points) const;

	__NODISCARD int GetRowIndex(const int index) const;
	__NODISCARD int GetColumnIndex(const int index) const;

	inline constexpr volatile static Tetris::Point m_LeftCoords{ 84L, 252L };
	inline constexpr volatile static Tetris::Point m_DownCoords{ 111L, 260L };
	inline constexpr volatile static Tetris::Point m_UpCoords{ 111L, 239L };
	inline constexpr volatile static Tetris::Point m_RightCoords{ 138L, 252L };
	inline constexpr volatile static Tetris::Point m_PadsCoords{ 80L, 168L };
	inline constexpr volatile static Tetris::Point m_StickyCoords{ 110L, 220L };
	inline constexpr volatile static Tetris::Point m_RotateRight{ 275L, 252L };
	inline constexpr volatile static Tetris::Point m_RotateLeft{ 246L, 252L };
	inline constexpr volatile static Tetris::Point m_VirtualPadWindowCoord{ 324L, 369L };

	/* Algorithm Parameters */
	inline constexpr static volatile float m_ClearLineWeight{ 1.5f };
	inline constexpr static volatile float m_HoleWeight{ -0.85f };
	inline constexpr static volatile float m_BumpinessWeight{ -0.10f };
	inline constexpr static volatile float m_AggregateHeightWeight{ -0.35f };
	inline constexpr static volatile float m_MoveWeight{ -0.05f };

	/* Program Information */
	bool m_IsBestMoveCalculated;
	class Board* m_pBoard;
	MoveToExecute m_BestMove;
	bool m_IsExecutingBestMove;
	std::queue<Click> m_ClicksToExecute;
	Timepoint m_ClickStart;
	bool m_Cooldown;
	Timepoint m_CooldownStart;

	/* Tetris Information */
	Tetromino m_CurrentPiece;
	uint64_t m_NrOfPieces;
	int m_NrOfLinesCleared;
	int m_Level;
	std::unordered_map<int, int> m_LevelSpeeds;
};