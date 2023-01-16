#pragma once

#include "../Config.h"
#include "../TetrisAlgorithm/TetrisAlgorithm.h"

class FutureGreedyAlgorithm : public TetrisAlgorithm
{
public:
	FutureGreedyAlgorithm(Board* pBoard);

	virtual void CalculateBestMove() override;
	virtual bool NeedsNextPiece() const override { return true; }
};