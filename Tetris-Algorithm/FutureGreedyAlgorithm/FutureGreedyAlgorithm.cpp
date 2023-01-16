#include "FutureGreedyAlgorithm.h"

FutureGreedyAlgorithm::FutureGreedyAlgorithm(Board* pBoard)
	: TetrisAlgorithm{ pBoard }
{}

void FutureGreedyAlgorithm::CalculateBestMove()
{
	std::cout << Utils::FindNextPiece(m_pBoard).GetShape() << "\n";

	m_CurrentPiece.Invalidate();
}