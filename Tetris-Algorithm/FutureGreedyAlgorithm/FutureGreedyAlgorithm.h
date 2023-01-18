#pragma once

#include "../Config.h"
#include "../TetrisAlgorithm/TetrisAlgorithm.h"

#include <array>

class FutureGreedyAlgorithm : public TetrisAlgorithm
{
public:
	FutureGreedyAlgorithm(Board* pBoard);

	virtual void CalculateBestMove() override;
	virtual bool NeedsNextPiece() const override { return true; }

private:
	virtual void OnNewPieceSpawnedImpl() override;
	virtual void OnBestMoveExecuted() override;

	float CalculateNextBestMove(const std::array<Tetris::Point, g_MaxNrOfBlocks>& points) const;

	inline constexpr static float m_FutureMoveWeight{ 0.4f };

	Tetromino m_NextPiece;
};