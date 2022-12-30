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
{}

void TetrisAlgorithm::OnNewPieceSpawned()
{
	if (m_Cooldown)
		return;

	FindCurrentPiece();

	CalculateBestMove();
	CalculateClicksToExecute();

	m_IsBestMoveCalculated = true;
	m_IsExecutingBestMove = true;
}

void TetrisAlgorithm::Update(const uint64_t currentFrame)
{
	constexpr double maxCD{ 3.0 };
	if (m_Cooldown)
		if ((Timer::Now() - m_CooldownStart).Count() >= maxCD)
			m_Cooldown = false;

	if (currentFrame == 0u || m_CurrentPiece.IsInvalid())
		return;

	if (m_IsBestMoveCalculated)
		ExecuteBestMove();
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

void TetrisAlgorithm::SendMousePress(const Point& coords) const
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

	m_pBoard->DebugBoardState();
	m_pBoard->Add(move.TargetPos);
	m_pBoard->DebugBoardState();
	m_pBoard->Remove(move.TargetPos);

	std::cout << "Score: " << max << "\n";
	m_BestMove = move;
}

float TetrisAlgorithm::EvaluatePosition(const std::array<Point, 4>& points) const
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

		m_Cooldown = true;
		m_CooldownStart = timer.Now();
		return;
	}

	if (Utils::AreEqual(m_ClickStart.Count(), 0.0))
	{
		m_ClickStart = timer.Now();
		return;
	}

	if ((timer.Now() - m_ClickStart).Count() >= (timer.GetTimePerFrame() * 3.0))
	{
		SendMousePress(m_ClicksToExecute.front());
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

	const int rotDistance{ (m_BestMove.TargetRotation - m_CurrentPiece.GetRotation()) };

	for (int i{}; i < rotDistance; ++i)
	{
		m_ClicksToExecute.push(m_VirtualPadWindowCoord);
		m_ClicksToExecute.push(m_RotateRight);

		m_CurrentPiece.Rotate(Tetromino::Rotation::CW);
	}

	/* how far we should move horizontally? */
	const int horDistance{ std::min_element(m_BestMove.TargetPos.cbegin(), m_BestMove.TargetPos.cend(), [](const auto& a, const auto& b)
		{
			return a.x < b.x;
		})->x - m_CurrentPiece.GetUtmostLeftPiece().x};

	if (horDistance < 0)
	{
		for (int i{}; i < abs(horDistance); ++i)
		{
			m_ClicksToExecute.push(m_VirtualPadWindowCoord);
			m_ClicksToExecute.push(m_LeftCoords);
		}
	}
	else if (horDistance > 0)
	{
		for (int i{}; i < horDistance; ++i)
		{
			m_ClicksToExecute.push(m_VirtualPadWindowCoord);
			m_ClicksToExecute.push(m_RightCoords);
		}
	}

	//m_ClicksToExecute.push(m_PadsCoords);
	//m_ClicksToExecute.push(m_StickyCoords);
	//m_ClicksToExecute.push(m_DownCoords);
	//m_ClicksToExecute.push(m_PadsCoords);
	//m_ClicksToExecute.push(m_StickyCoords);
}

int TetrisAlgorithm::GetColumnIndex(const int index) const
{
	return static_cast<int>(index % m_BoardSize.x);
}