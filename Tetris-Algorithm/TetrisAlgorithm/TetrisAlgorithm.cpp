#include "TetrisAlgorithm.h"
#include "../Utils/Utils.h"

#ifdef _DEBUG
#include <iostream> 
#endif

#include "../Board/Board.h"

#ifdef max
#undef max
#endif

TetrisAlgorithm::TetrisAlgorithm(Board* pBoard)
	: m_IsBestMoveCalculated{}
	, m_pBoard{ pBoard }
	, m_CurrentPiece{}
	, m_BestMove{}
{}

void TetrisAlgorithm::Update(const uint64_t currentFrame)
{
	if (m_CurrentPiece.IsInvalid())
	{
		/* Find our current moving piece */
		FindCurrentPiece();
	}

	if (!m_IsBestMoveCalculated)
	{
		CalculateBestMove();
		m_IsBestMoveCalculated = true;
	}

	ExecuteBestMove();
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
	uint8_t blockIndices[maxNrOfBlocks]{};
	uint8_t indexCounter{};

	const auto& previousBoard{ m_pBoard->GetPreviousBoardState() };
	const auto& board{ m_pBoard->GetBoardState() };

	for (uint8_t i{}; i < previousBoard.size(); ++i)
	{
		if (previousBoard[i] == board[i])
			continue;

		if (indexCounter >= maxNrOfBlocks)
			return;

		blockIndices[indexCounter++] = i;
	}

#ifdef _DEBUG
	std::cout << "\nFound Unique Indices: " << static_cast<int>(indexCounter);
#endif

	if (indexCounter != maxNrOfBlocks)
		return;

	uint8_t rowIndices[maxNrOfBlocks]{};
	uint8_t colIndices[maxNrOfBlocks]{};

	for (uint8_t i{}; i < maxNrOfBlocks; ++i)
	{
		rowIndices[i] = GetRowIndex(blockIndices[i]);
		colIndices[i] = GetColumnIndex(blockIndices[i]);
	}

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

	m_CurrentPiece = Tetromino{ nrOfEqualRowIndices, nrOfEqualColIndices, rowIndices, colIndices, m_pBoard };
}

void TetrisAlgorithm::CalculateBestMove()
{
	__ASSERT(!m_CurrentPiece.IsInvalid() && m_CurrentPiece.GetShape() != TetrominoShape::NONE);

	Tetromino tempPiece{ m_CurrentPiece };

	/* Move the piece to the left as long as it's possible */
	while (tempPiece.Move(Tetromino::Direction::Left)) {}

	MoveToExecute move{};
	uint8_t max{};

	for (uint8_t i{}; i < g_MaxNrOfBlocks; ++i)
	{
		for (uint8_t j{}; j < m_BoardSize.x; ++j)
		{
			/* Move the piece as much down as possible */
			while (tempPiece.Move(Tetromino::Direction::Down)) {}

			const int8_t score{ EvaluatePosition(tempPiece.GetCurrentPosition()) };

			if (score > max)
			{
				max = score;
				move.TargetPos = tempPiece.GetCurrentPosition();
				move.TargetRotation = tempPiece.GetRotation();
			}
		}

		tempPiece.Rotate(Tetromino::Rotation::CW);
	}

	m_BestMove = move;
}

int8_t TetrisAlgorithm::EvaluatePosition(const std::array<Point, 4>& points) const
{
	int8_t score{};

	/* Greedy Approach: Does it clear any line? */
	if (m_pBoard->IsAnyRowComplete(points))
		score += m_ClearLineScore; /* excellent move, and we really want to do it */

	/* Check for holes */
	if (m_pBoard->GetNewNrOfHoles(points) > 0u)
		score += m_HoleScore;

	/* Check for bumpiness */
	if (m_pBoard->GetNewBumpiness(points) > 0u)
		score += m_BumpinessScore;

	/* Check for the aggregrate height */
	if (m_pBoard->GetNewAggregateHeight(points) > 0u)
		score += m_AggregateHeightScore;

	/* [TODO]: Add the total inputs required for this position to the final score */

	return score;
}

void TetrisAlgorithm::ExecuteBestMove()
{
	__ASSERT(m_IsBestMoveCalculated);

	const uint8_t currentRot{ m_CurrentPiece.GetRotation() };
	const auto& currentPos{ m_CurrentPiece.GetCurrentPosition() };
}

uint8_t TetrisAlgorithm::GetRowIndex(const uint8_t index) const
{
	return static_cast<uint8_t>(index / m_BoardSize.x);
}

uint8_t TetrisAlgorithm::GetColumnIndex(const uint8_t index) const
{
	return static_cast<uint8_t>(index % m_BoardSize.x);
}