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
	//if (m_Cooldown)
	//	return;

	//FindCurrentPiece();

	//CalculateBestMove();
	//CalculateClicksToExecute();

	//m_IsBestMoveCalculated = true;
	//m_IsExecutingBestMove = true;
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
	constexpr static uint8_t maxNrOfBlocks{ 4 };

	/* Get the current piece */
	uint8_t rowIndices[maxNrOfBlocks]{};
	uint8_t colIndices[maxNrOfBlocks]{};
	uint8_t indexCounter{};

	const auto& board{ m_pBoard->GetBoardState() };
	for (int r{ m_BoardSize.y - 2 }; r >= 0; --r)
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
	}

	if (indexCounter != maxNrOfBlocks)
		return;

	uint8_t nrOfEqualRowIndices{}, nrOfEqualColIndices{};
	uint8_t rowCounter{}, colCounter{};

	/* First find the duplicate elements */
	uint8_t duplicateRowIndices[maxNrOfBlocks / 2u]{ std::numeric_limits<uint8_t>::max(), std::numeric_limits<uint8_t>::max() };
	uint8_t duplicateColIndices[maxNrOfBlocks / 2u]{ std::numeric_limits<uint8_t>::max(), std::numeric_limits<uint8_t>::max() };

	for (uint8_t i{}; i < maxNrOfBlocks; ++i)
	{
		for (uint8_t j{ i + 1u }; j < maxNrOfBlocks; ++j)
		{
			if (rowIndices[i] == rowIndices[j])
			{
				bool isUnique = true;
				for (uint8_t k{}; k < maxNrOfBlocks / 2u; ++k)
					if (duplicateRowIndices[k] == rowIndices[i])
						isUnique = false;

				if (isUnique)
					duplicateRowIndices[rowCounter++] = rowIndices[i];
			}

			if (colIndices[i] == colIndices[j])
			{
				bool isUnique = true;
				for (uint8_t k{}; k < maxNrOfBlocks / 2u; ++k)
					if (duplicateColIndices[k] == colIndices[i])
						isUnique = false;

				if (isUnique)
					duplicateColIndices[colCounter++] = colIndices[i];
			}
		}
	}

	/* Check how often each duplicate element appears */
	for (uint8_t i{}; i < maxNrOfBlocks / 2u; ++i)
	{
		for (uint8_t j{}; j < maxNrOfBlocks; ++j)
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

	Tetromino tempPiece{ m_CurrentPiece };

	/* Move the piece to the left as long as it's possible */
	while (tempPiece.Move(Tetromino::Direction::Left)) {}

	MoveToExecute move{};
	float max{ -1000.f };

	/* This for-loop handles going right */
	for (uint8_t i{}; i < m_BoardSize.x; ++i)
	{
		Tetromino workingPiece{ tempPiece };

		/* This for-loop handles rotations */
		for (uint8_t j{}; j < workingPiece.MaxNrOfRotations(); ++j)
		{
			/* Move the piece as much down as possible */
			while (workingPiece.Move(Tetromino::Direction::Down)) {}

			float score{ EvaluatePosition(workingPiece.GetCurrentPosition()) };
			// score += workingPiece.GetRotation() * m_MoveWeight;
			// score += abs(workingPiece.GetCurrentPosition()[0].x - m_CurrentPiece.GetCurrentPosition()[0].x) * m_MoveWeight;

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

	///* Greedy Approach: Does it clear any line? */
	//if (m_pBoard->IsAnyRowComplete(points))
	//	score += m_ClearLineScore; /* excellent move, and we really want to do it */

	///* Check for holes */
	//if (m_pBoard->GetNewNrOfHoles(points) > 0u)
	//	score += m_HoleScore;

	///* Check for bumpiness */
	//if (m_pBoard->GetNewBumpiness(points) > 0u)
	//	score += m_BumpinessScore;

	///* Check for the aggregrate height */
	//if (m_pBoard->GetNewAggregateHeight(points) > 0u)
	//	score += m_AggregateHeightScore;

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

uint8_t TetrisAlgorithm::GetRowIndex(const uint8_t index) const
{
	return static_cast<uint8_t>(index / m_BoardSize.x);
}

void TetrisAlgorithm::CalculateClicksToExecute()
{
	/* How many times should we rotate? */
	/* m_CurrentPiece.GetRotation() is guaranteed to be 0 when this function is called */

	__ASSERT(m_CurrentPiece.GetRotation() == 0);

	const int8_t rotDistance{ (m_BestMove.TargetRotation - m_CurrentPiece.GetRotation()) };

	for (int8_t i{}; i < rotDistance; ++i)
	{
		m_ClicksToExecute.push(m_VirtualPadWindowCoord);
		m_ClicksToExecute.push(m_RotateRight);
	}

	/* how far we should move horizontally? */
	const int8_t horDistance{ static_cast<int8_t>((m_BestMove.TargetPos[0] - m_CurrentPiece.GetCurrentPosition()[0]).x) };

	if (horDistance < 0)
	{
		for (int8_t i{}; i < abs(horDistance); ++i)
		{
			m_ClicksToExecute.push(m_VirtualPadWindowCoord);
			m_ClicksToExecute.push(m_LeftCoords);
		}
	}
	else if (horDistance > 0)
	{
		for (int8_t i{}; i < horDistance; ++i)
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

uint8_t TetrisAlgorithm::GetColumnIndex(const uint8_t index) const
{
	return static_cast<uint8_t>(index % m_BoardSize.x);
}