#include "GreedyAlgorithm.h"

GreedyAlgorithm::GreedyAlgorithm(Board* pBoard)
	: TetrisAlgorithm{ pBoard }
{}

void GreedyAlgorithm::CalculateBestMove()
{
	__ASSERT(!m_CurrentPiece.IsInvalid() && m_CurrentPiece.GetShape() != TetrominoShape::NONE);

	/* Remove the current piece from the board */
	m_pBoard->Remove(m_CurrentPiece.GetCurrentPosition());

	Tetromino tempPiece{ m_CurrentPiece };

	/* Move the piece to the left as long as it's possible */
	while (tempPiece.Move(Tetromino::Direction::Left)) {}

	MoveToExecute move{};
	float max{ -1000.f };

	/* This for-loop handles going right */
	for (int i{}; i < m_BoardSize.x; ++i)
	{
		Tetromino workingPiece{ tempPiece };

		/* This for-loop handles rotations */
		for (int j{}; j < workingPiece.MaxNrOfRotations(); ++j)
		{
			/* Move the piece to the position we're checking */
			const int currentPos{ workingPiece.GetUtmostLeftPiece().x };
			if (currentPos != i)
			{
				if (currentPos > i)
				{
					for (int k{}; k < currentPos - i; ++k)
						if (!workingPiece.Move(Tetromino::Direction::Left))
							break;
				}
				else
				{
					for (int k{}; k < i - currentPos; ++k)
						if (!workingPiece.Move(Tetromino::Direction::Right))
							break;
				}
			}

			/* Move the piece as much down as possible */
			while (workingPiece.Move(Tetromino::Direction::Down)) {}

			// m_pBoard->Add(workingPiece.GetCurrentPosition());

			float score{ EvaluatePosition(workingPiece.GetCurrentPosition()) };
			// score += workingPiece.GetRotation() * m_MoveWeight;
			// score += abs(workingPiece.GetCurrentPosition()[0].x - m_CurrentPiece.GetCurrentPosition()[0].x) * m_MoveWeight;

			// m_pBoard->Remove(workingPiece.GetCurrentPosition());

			if (score > max)
			{
				max = score;
				move.TargetPos = workingPiece.GetCurrentPosition();
				move.TargetRotation = workingPiece.GetRotation();
			}

			/* Move the piece as much up as possible */
			while (workingPiece.Move(Tetromino::Direction::Up)) {}

			if (!workingPiece.Rotate(Tetromino::Rotation::CW))
				break;
		}

		tempPiece.Move(Tetromino::Direction::Right);
	}

	//m_pBoard->DebugBoardState();
	//m_pBoard->Add(move.TargetPos);
	//m_pBoard->DebugBoardState();
	//m_pBoard->Remove(move.TargetPos);

	if (m_Level <= 28)
	{
		constexpr int nrOfLinesPerLevel{ 10 };
		if (const int nrOfLines{ m_pBoard->GetNewNrOfCompletedLines(move.TargetPos) }; nrOfLines > 0)
		{
			m_NrOfLinesCleared += nrOfLines;

			if (m_NrOfLinesCleared >= nrOfLinesPerLevel)
			{
				++m_Level;
				m_NrOfLinesCleared -= nrOfLinesPerLevel;
			}
		}
	}

	// std::cout << "Score: " << max << "\n";
	m_BestMove = move;
}
