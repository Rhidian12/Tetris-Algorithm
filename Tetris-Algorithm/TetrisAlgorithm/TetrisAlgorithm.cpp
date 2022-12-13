#include "TetrisAlgorithm.h"
#include "../Utils/Utils.h"

#include <filesystem> /* std::filesystem */

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
	}
	else
	{

	}
}

void TetrisAlgorithm::SendMousePress(const Point& coords) const
{
	INPUT input{};

	input.type = INPUT_MOUSE;
	Utils::ConvertNDCToScreenCoords(input.mi.dx, input.mi.dy, coords);
	input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

	SendInput(1, &input, sizeof(INPUT));
}

void TetrisAlgorithm::Initialize()
{
	HWND tetris{ FindWindowA(NULL, "NES - Tetris") };
	__ASSERT(tetris != nullptr);

	HWND virtualPads{ FindWindowA(NULL, "Virtual Pads") };
	__ASSERT(virtualPads != nullptr);

	SetForegroundWindow(tetris);
	SetForegroundWindow(virtualPads);

	Sleep(1000);
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

	/* match the current piece in every possible spot */


}

uint8_t TetrisAlgorithm::GetRowIndex(const uint8_t index) const
{
	return static_cast<uint8_t>(index / m_BoardSize.x);
}

uint8_t TetrisAlgorithm::GetColumnIndex(const uint8_t index) const
{
	return static_cast<uint8_t>(index % m_BoardSize.x);
}