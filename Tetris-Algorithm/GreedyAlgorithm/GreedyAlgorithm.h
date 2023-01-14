#pragma once

#include "../TetrisAlgorithm/TetrisAlgorithm.h"

class GreedyAlgorithm : public TetrisAlgorithm
{
public:
	GreedyAlgorithm(Board* pBoard);

	virtual void CalculateBestMove() override;
	virtual bool NeedsNextPiece() const override { return false; }
};