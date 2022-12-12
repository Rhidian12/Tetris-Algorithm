#pragma once

#include "../Utils/Utils.h"

#include <iostream>

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
	Tetromino();
	Tetromino(const uint8_t nrOfEqualRowIndices, const uint8_t nrOfEqualColIndices, uint8_t* rowIndices, uint8_t* colIndices);

	__NODISCARD TetrominoShape GetShape() const;

private:
	constexpr static uint8_t m_MaxNrOfBlocks{ 4u };

	TetrominoShape m_Shape;
	uint8_t m_RowIndices[m_MaxNrOfBlocks];
	uint8_t m_ColIndices[m_MaxNrOfBlocks];
};