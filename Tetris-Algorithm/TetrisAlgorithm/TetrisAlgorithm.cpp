#include "TetrisAlgorithm.h"
#include "../Utils/Utils.h"

#ifdef _DEBUG
#include <iostream> 
#endif

#include "../Board/Board.h"
#include "../Timer/Timer.h"

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

TetrisAlgorithm::TetrisAlgorithm(Board* pBoard)
	: m_IsBestMoveCalculated{}
	, m_pBoard{ pBoard }
	, m_CurrentPiece{}
	, m_BestMove{}
	, m_IsExecutingBestMove{}
	, m_ClickStart{}
	, m_NrOfPieces{}
	, m_HasNewPieceSpawned{}
	, m_Cooldown{}
	, m_CooldownStart{}
	, m_NrOfLinesCleared{}
	, m_Level{}
	, m_LevelSpeeds{}
{
	constexpr int nrOfLevels{ 30 };
	const int speeds[nrOfLevels]{ 48,43,38,33,28,23,18,13,8,6,5,5,5,4,4,4,3,3,3,2,2,2,2,2,2,2,2,2,2,1 };

	for (int i{}; i < nrOfLevels; ++i)
		m_LevelSpeeds.insert(std::make_pair(i, speeds[i]));
}

void TetrisAlgorithm::OnNewPieceSpawned()
{
	FindCurrentPiece();

	if (m_CurrentPiece.IsInvalid())
		return;

	CalculateBestMove();
	CalculateClicksToExecute();

	m_IsBestMoveCalculated = true;
	m_IsExecutingBestMove = true;
}

void TetrisAlgorithm::Update()
{
	if (m_CurrentPiece.IsInvalid())
		return;

	if (m_IsBestMoveCalculated)
		ExecuteBestMove();
}

int TetrisAlgorithm::GetSpeed() const
{
	return m_LevelSpeeds.at(m_Level);
}

void TetrisAlgorithm::ApplyBestMove()
{
	m_pBoard->Add(m_BestMove.TargetPos);
}

void TetrisAlgorithm::SetCurrentPiece(const Tetromino& tetromino)
{
	m_CurrentPiece = tetromino;
}

bool TetrisAlgorithm::IsExecutingBestMove() const
{
	return m_IsExecutingBestMove;
}

void TetrisAlgorithm::SendMousePress(const volatile Tetris::Point& coords) const
{
	INPUT input{};

	input.type = INPUT_MOUSE;
	Utils::ConvertNDCToScreenCoords(input.mi.dx, input.mi.dy, coords);
	input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

	SendInput(1, &input, sizeof(INPUT));
}

void TetrisAlgorithm::FindCurrentPiece()
{
	constexpr static int maxNrOfBlocks{ 4 };

	/* Get the current piece */
	int rowIndices[maxNrOfBlocks]{};
	int colIndices[maxNrOfBlocks]{};
	int indexCounter{};

	const auto& board{ m_pBoard->GetBoardState() };
	for (int r{ m_BoardSize.y - 3 }; r >= 0; --r)
	{
		for (int c{}; c < m_BoardSize.x; ++c)
		{
			if (board[r][c])
			{
				rowIndices[indexCounter] = r;
				colIndices[indexCounter++] = c;
			}

			if (indexCounter >= maxNrOfBlocks)
				break;
		}

		if (indexCounter >= maxNrOfBlocks)
			break;
	}

	if (indexCounter != maxNrOfBlocks)
		return;

	int nrOfEqualRowIndices{}, nrOfEqualColIndices{};
	int rowCounter{}, colCounter{};

	/* First find the duplicate elements */
	int duplicateRowIndices[maxNrOfBlocks / 2u]{ std::numeric_limits<int>::max(), std::numeric_limits<int>::max() };
	int duplicateColIndices[maxNrOfBlocks / 2u]{ std::numeric_limits<int>::max(), std::numeric_limits<int>::max() };

	for (int i{}; i < maxNrOfBlocks; ++i)
	{
		for (int j{ i + 1 }; j < maxNrOfBlocks; ++j)
		{
			if (rowIndices[i] == rowIndices[j])
			{
				bool isUnique = true;
				for (int k{}; k < maxNrOfBlocks / 2; ++k)
					if (duplicateRowIndices[k] == rowIndices[i])
						isUnique = false;

				if (isUnique)
					duplicateRowIndices[rowCounter++] = rowIndices[i];
			}

			if (colIndices[i] == colIndices[j])
			{
				bool isUnique = true;
				for (int k{}; k < maxNrOfBlocks / 2; ++k)
					if (duplicateColIndices[k] == colIndices[i])
						isUnique = false;

				if (isUnique)
					duplicateColIndices[colCounter++] = colIndices[i];
			}
		}
	}

	/* Check how often each duplicate element appears */
	for (int i{}; i < maxNrOfBlocks / 2; ++i)
	{
		for (int j{}; j < maxNrOfBlocks; ++j)
		{
			if (duplicateRowIndices[i] == rowIndices[j])
				++nrOfEqualRowIndices;

			if (duplicateColIndices[i] == colIndices[j])
				++nrOfEqualColIndices;
		}
	}

	m_HasNewPieceSpawned = false;

	m_CurrentPiece = Tetromino{ nrOfEqualRowIndices, nrOfEqualColIndices, rowIndices, colIndices, m_pBoard };
}

void TetrisAlgorithm::CalculateBestMove()
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

float TetrisAlgorithm::EvaluatePosition(const std::array<Tetris::Point, 4>& points) const
{
	float score{};

	score += m_AggregateHeightWeight * m_pBoard->GetNewAggregateHeight(points);
	score += m_ClearLineWeight * m_pBoard->GetNewNrOfCompletedLines(points);
	score += m_HoleWeight * m_pBoard->GetNewNrOfHoles(points);
	score += m_BumpinessWeight * m_pBoard->GetNewBumpiness(points);

#ifdef _DEBUG
	//m_pBoard->Add(points);
	//m_pBoard->DebugBoardState();
	//m_pBoard->Remove(points);
#endif

	return score;
}

void TetrisAlgorithm::ExecuteBestMove()
{
	__ASSERT(m_IsBestMoveCalculated);

	const Timer& timer{ Timer::GetInstance() };

	if (m_ClicksToExecute.empty())
	{
		m_IsExecutingBestMove = false;
		m_IsBestMoveCalculated = false;
		m_CurrentPiece.Invalidate();

		// m_Cooldown = true;
		// m_CooldownStart = timer.Now();
		return;
	}

	if (Utils::AreEqual(m_ClickStart.Count(), 0.0))
	{
		m_ClickStart = timer.Now();
		return;
	}

	if (const double dTime{ (timer.Now() - m_ClickStart).Count() };
		dTime >= m_ClicksToExecute.front().Delay)
	{
		SendMousePress(m_ClicksToExecute.front().ClickCoord);
		m_ClicksToExecute.pop();

		m_ClickStart = Timepoint{};
	}
}

int TetrisAlgorithm::GetRowIndex(const int index) const
{
	return static_cast<int>(index / m_BoardSize.x);
}

void TetrisAlgorithm::CalculateClicksToExecute()
{
	/* How many times should we rotate? */
	/* m_CurrentPiece.GetRotation() is guaranteed to be 0 when this function is called */

	__ASSERT(m_CurrentPiece.GetRotation() == 0);

	if (m_CurrentPiece.GetShape() == TetrominoShape::I)
	{
		__ASSERT(m_CurrentPiece.GetUtmostLeftPiece().x == 3);
	}
	else
	{
		__ASSERT(m_CurrentPiece.GetUtmostLeftPiece().x == 4);
	}

	const int rotDistance{ (m_BestMove.TargetRotation - m_CurrentPiece.GetRotation()) };

	for (int i{}; i < rotDistance; ++i)
	{
		m_ClicksToExecute.push(Click{ m_VirtualPadWindowCoord, 0.0 });
		m_ClicksToExecute.push(Click{ m_RotateRight, 0.0 });

		m_CurrentPiece.Rotate(Tetromino::Rotation::CW);
	}

	const auto getUtmostLeftPiece = [](const std::array<Tetris::Point, 4>& arr)->int
	{
		return std::min_element(arr.cbegin(), arr.cend(), [](const auto& a, const auto& b)
			{
				return a.x < b.x;
			})->x;
	};
	const int bestMoveLeft{ getUtmostLeftPiece(m_BestMove.TargetPos) };

	int counter{};
	if (bestMoveLeft < m_CurrentPiece.GetUtmostLeftPiece().x)
	{
		while (bestMoveLeft != m_CurrentPiece.GetUtmostLeftPiece().x)
		{
			m_ClicksToExecute.push(Click{ m_VirtualPadWindowCoord, 0.0 });
			m_ClicksToExecute.push(Click{ m_LeftCoords, 0.0 });
			m_CurrentPiece.Move(Tetromino::Direction::Left);

			if (++counter > 5) // safety check
				break;
		}
	}
	else if (bestMoveLeft > m_CurrentPiece.GetUtmostLeftPiece().x)
	{
		while (bestMoveLeft != m_CurrentPiece.GetUtmostLeftPiece().x)
		{
			m_ClicksToExecute.push(Click{ m_VirtualPadWindowCoord,0.0 });
			m_ClicksToExecute.push(Click{ m_RightCoords,0.0 });
			m_CurrentPiece.Move(Tetromino::Direction::Right);

			if (++counter > 5) // safety check
				break;
		}
	}

	m_ClicksToExecute.push(Click{ m_PadsCoords,0.0 });
	m_ClicksToExecute.push(Click{ m_StickyCoords,0.0 });
	m_ClicksToExecute.push(Click{ m_DownCoords,0.0 });

	if (m_Level <= 5)
		m_ClicksToExecute.push(Click{ m_VirtualPadWindowCoord, Timer::GetInstance().GetTimePerFrame() * (15.0 - m_Level) });

	m_ClicksToExecute.push(Click{ m_PadsCoords,0.0 });
	m_ClicksToExecute.push(Click{ m_StickyCoords,0.0 });
}

int TetrisAlgorithm::GetColumnIndex(const int index) const
{
	return static_cast<int>(index % m_BoardSize.x);
}