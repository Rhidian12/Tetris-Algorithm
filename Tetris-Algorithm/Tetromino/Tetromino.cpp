#include "Tetromino.h"

#include <stdint.h>
#include <limits>

#ifdef max
#undef max
#endif

Tetromino::Tetromino()
	: m_Shape{ TetrominoShape::NONE }
	, m_Points{}
	, m_HasRotated{}
{}

Tetromino::Tetromino(const uint8_t nrOfEqualRowIndices, const uint8_t nrOfEqualColIndices, uint8_t* rowIndices, uint8_t* colIndices)
	: m_Shape{ TetrominoShape::NONE }
	, m_Points{}
	, m_HasRotated{}
{
	for (uint8_t i{}; i < m_MaxNrOfBlocks; ++i)
	{
		m_Points.push_back({ rowIndices[i], colIndices[i] });
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

void Tetromino::Rotate(const Rotation rot)
{
	__ASSERT(m_Shape != TetrominoShape::NONE);

	if (m_Shape == TetrominoShape::O)
		return;

	Point pivot{};

	switch (m_Shape)
	{
	case TetrominoShape::I:
		pivot = m_Points[2];
		break;
	case TetrominoShape::L:
		pivot = m_Points[1];
		break;
	case TetrominoShape::J:
		pivot = m_Points[2];
		break;
	case TetrominoShape::T:
		pivot = m_Points[1];
		break;
	case TetrominoShape::Z:
		pivot = m_Points[1];
		break;
	case TetrominoShape::S:
		pivot = m_Points[2];
		break;
	}

	Rotate(rot, pivot);

	if (m_HasRotated && m_Shape == TetrominoShape::I)
	{
		/* Because we're using a block as pivot instead of a coordinate, we need to rotate a full 360 degrees for
		 the I rotation to be correct */
		Rotate(rot, pivot);
		Rotate(rot, pivot);
		Rotate(rot, pivot);
	}

	m_HasRotated = !m_HasRotated;
}

void Tetromino::Move(const Direction dir)
{
	Point direction{};

	switch (dir)
	{
	case Direction::Left:
		direction.x = -1;
		break;
	case Direction::Right:
		direction.x = 1;
		break;
	case Direction::Down:
		direction.y = -1;
		break;
	}

	bool isIllegalMove{};
	int lastIndex{ static_cast<int>(m_Points.size() - 1) };
	for (int i{}; i <= lastIndex; ++i)
	{
		Point& point = m_Points[i];

		if (!isIllegalMove)
		{
			point += direction;
			if (point.x < 0 || point.x >= m_BoardSize.x)
			{
				isIllegalMove = true;
				i = -1; /* gets incremented to 0 */
				lastIndex = i;
			}
		}
		else
			point -= direction;
	}
}

TetrominoShape Tetromino::GetShape() const
{
	return m_Shape;
}

void Tetromino::Rotate(const Rotation rot, const Point& pivot)
{
	float s{ static_cast<uint8_t>(rot) == static_cast<uint8_t>(Rotation::Clockwise) ? -1.f : 1.f };
	/* constexpr float c{ 0.f }; */

	for (Point& point : m_Points)
	{
		if (point == pivot)
			continue;

		// translate point back to origin:
		point.x -= pivot.x;
		point.y -= pivot.y;

		// rotate point
		float xNew = /* point.x * c */ -point.y * s;
		float yNew = point.x * s /* + point.y * c */;

		point.x = xNew + pivot.x;
		point.y = yNew + pivot.y;
	}
}