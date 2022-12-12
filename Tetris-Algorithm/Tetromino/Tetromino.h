#pragma once

#include "../Utils/Utils.h"

#include <iostream> /* std::ostream, std::cout, ... */
#include <vector> /* std::vector */

enum class TetrominoShape
{
	I,
	O,
	L,
	J,
	T,
	Z,
	S,
	NONE
};

inline std::ostream& operator<<(std::ostream& os, const TetrominoShape shape)
{
	switch (shape)
	{
	case TetrominoShape::I:
		return os << "Piece is I";
	case TetrominoShape::O:
		return os << "Piece is O";
	case TetrominoShape::L:
		return os << "Piece is L";
	case TetrominoShape::J:
		return os << "Piece is J";
	case TetrominoShape::T:
		return os << "Piece is T";
	case TetrominoShape::Z:
		return os << "Piece is Z";
	case TetrominoShape::S:
		return os << "Piece is S";
	case TetrominoShape::NONE:
	default:
		return os << "Piece not found!";
	}
}

class Tetromino final
{
public:
	enum class Rotation : uint8_t
	{
		Clockwise = 0,
		CounterClockwise = 1,

		CW = 0,
		CCW = 1
	};

	enum class Direction : uint8_t
	{
		Left = 0,
		Right = 1,
		Down = 2
	};

public:
	Tetromino();
	Tetromino(const uint8_t nrOfEqualRowIndices, const uint8_t nrOfEqualColIndices, uint8_t* rowIndices, uint8_t* colIndices);

	void Rotate(const Rotation rot);
	void Move(const Direction dir);

	void Invalidate();

	__NODISCARD TetrominoShape GetShape() const;

private:
	constexpr static uint8_t m_MaxNrOfBlocks{ 4u };
	void Rotate(const Rotation rot, const Point& pivot);

	TetrominoShape m_Shape;
	std::vector<Point> m_Points;
	bool m_HasRotated; /* Only used for I piece */
};