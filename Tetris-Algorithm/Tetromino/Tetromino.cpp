#include "Tetromino.h"

#include <stdint.h>
#include <limits>

#ifdef max
#undef max
#endif

Tetromino::Tetromino()
	: m_Shape{ TetrominoShape::NONE }
	, m_RowIndices{}
	, m_ColIndices{}
{}

Tetromino::Tetromino(const uint8_t nrOfEqualRowIndices, const uint8_t nrOfEqualColIndices, uint8_t* rowIndices, uint8_t* colIndices)
	: m_Shape{ TetrominoShape::NONE }
	, m_RowIndices{}
	, m_ColIndices{}
{
	for (uint8_t i{}; i < m_MaxNrOfBlocks; ++i)
	{
		m_RowIndices[i] = rowIndices[i];
		m_ColIndices[i] = colIndices[i];
	}

	if (nrOfEqualRowIndices == m_MaxNrOfBlocks && nrOfEqualColIndices == 0)
		m_Shape = TetrominoShape::I;
	else if (nrOfEqualRowIndices == 4 && nrOfEqualColIndices == 4)
		m_Shape = TetrominoShape::O;
	else if (nrOfEqualRowIndices == 3 && nrOfEqualColIndices == 2)
	{
		//  J     L     T
		// x   |   x | xxx
		// xxx | xxx |  x

		/* Find the duplicate column and the first non-duplicate column */
		uint8_t duplicateColIndex{ std::numeric_limits<uint8_t>::max() };
		uint8_t firstUniqueColIndex{}, lastUniqueColIndex{};

		for (uint8_t i{}; i < m_MaxNrOfBlocks; ++i)
		{
			for (uint8_t j{ i + 1u }; j < m_MaxNrOfBlocks; ++j)
			{
				if (colIndices[i] == colIndices[j])
				{
					duplicateColIndex = colIndices[i];
					break;
				}
			}

			if (duplicateColIndex != std::numeric_limits<uint8_t>::max())
				break;
		}

		for (uint8_t i{}; i < m_MaxNrOfBlocks; ++i)
		{
			if (colIndices[i] != duplicateColIndex)
			{
				firstUniqueColIndex = colIndices[i];
				break;
			}
		}

		for (int8_t i{ m_MaxNrOfBlocks - 1 }; i >= 0; --i)
		{
			if (colIndices[i] != duplicateColIndex)
			{
				lastUniqueColIndex = colIndices[i];
				break;
			}
		}

		if (duplicateColIndex < firstUniqueColIndex && duplicateColIndex < lastUniqueColIndex)
			m_Shape = TetrominoShape::L;
		else if (duplicateColIndex > firstUniqueColIndex && duplicateColIndex < lastUniqueColIndex)
			m_Shape = TetrominoShape::T;
		else
			m_Shape = TetrominoShape::J;
	}
	else if (nrOfEqualRowIndices == 4 && nrOfEqualColIndices == 2)
	{
		//  Z     S
		// xx  |  xx
		//  xx | xx

		/* find the smallest col and its corresponding row, and the biggest row in general */
		uint8_t smallestCol{ std::numeric_limits<uint8_t>::max() }, smallestColRow{ std::numeric_limits<uint8_t>::max() };
		uint8_t biggestRow{};
		for (uint8_t i{}; i < m_MaxNrOfBlocks; ++i)
		{
			if (colIndices[i] < smallestCol)
			{
				smallestCol = colIndices[i];
				smallestColRow = rowIndices[i];
			}

			if (rowIndices[i] > biggestRow)
				biggestRow = rowIndices[i];
		}

		if (smallestColRow < biggestRow)
			m_Shape = TetrominoShape::Z;
		else if (smallestColRow == biggestRow)
			m_Shape = TetrominoShape::S;
	}
}

__NODISCARD TetrominoShape Tetromino::GetShape() const
{
	return m_Shape;
}
