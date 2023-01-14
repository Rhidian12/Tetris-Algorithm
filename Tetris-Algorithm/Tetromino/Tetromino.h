#pragma once

#include "../Config.h"

#include <iostream> /* std::ostream, std::cout, ... */
#include <array> /* std::array */

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
		Down = 2,
		Up = 3
	};

public:
	Tetromino();
	Tetromino(const int nrOfEqualRowIndices, const int nrOfEqualColIndices, int* rowIndices, int* colIndices,
		class Board* pBoard);
	Tetromino(const TetrominoShape shape, const Tetris::Point& start, class Board* pBoard);

	bool Rotate(const Rotation rot);
	bool Move(const Direction dir);

	void Invalidate();

	__NODISCARD TetrominoShape GetShape() const;
	__NODISCARD bool IsInvalid() const;
	__NODISCARD const std::array<Tetris::Point, 4>& GetCurrentPosition() const;
	__NODISCARD uint8_t GetRotation() const;
	__NODISCARD uint8_t MaxNrOfRotations() const;
	__NODISCARD const Tetris::Point& GetUtmostLeftPiece() const;

private:
	constexpr static uint8_t m_MaxNrOfBlocks{ 4u };
	void Rotate(const Rotation rot, const Tetris::Point& pivot);
	__NODISCARD bool IsMoveIllegal(const Direction dir, const Tetris::Point& point) const;
	void FillPoints(const Tetris::Point& start);

	TetrominoShape m_Shape;
	std::array<Tetris::Point, 4> m_Points;
	int8_t m_Rotation;
	class Board* m_pBoard;
};