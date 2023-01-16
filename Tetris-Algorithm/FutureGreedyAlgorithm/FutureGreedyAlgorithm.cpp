#include "FutureGreedyAlgorithm.h"

FutureGreedyAlgorithm::FutureGreedyAlgorithm(Board* pBoard)
	: TetrisAlgorithm{ pBoard }
{}

void FutureGreedyAlgorithm::CalculateBestMove()
{
	__ASSERT(!m_CurrentPiece.IsInvalid() && !m_NextPiece.IsInvalid());

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

			const float score{ EvaluatePosition(workingPiece.GetCurrentPosition()) + CalculateNextBestMove(workingPiece.GetCurrentPosition()) };

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

	m_BestMove = move;
}

void FutureGreedyAlgorithm::OnNewPieceSpawnedImpl()
{
	m_NextPiece = Utils::FindNextPiece(m_pBoard);
	m_NextPiece.SetToDefaultPos();
}

void FutureGreedyAlgorithm::OnBestMoveExecuted()
{
	m_NextPiece.Invalidate();
}

float FutureGreedyAlgorithm::CalculateNextBestMove(const std::array<Tetris::Point, g_MaxNrOfBlocks>& points) const
{
	m_pBoard->Add(points);

	Tetromino tempPiece{ m_NextPiece };

	/* Move the piece to the left as long as it's possible */
	while (tempPiece.Move(Tetromino::Direction::Left)) {}

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

			const float score{ EvaluatePosition(workingPiece.GetCurrentPosition()) };

			if (score > max)
				max = score;

			/* Move the piece as much up as possible */
			while (workingPiece.Move(Tetromino::Direction::Up)) {}

			if (!workingPiece.Rotate(Tetromino::Rotation::CW))
				break;
		}

		tempPiece.Move(Tetromino::Direction::Right);
	}

	m_pBoard->Remove(points);

	return max;
}