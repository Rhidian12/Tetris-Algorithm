#pragma once

#include "../Utils/Utils.h"

#include <Components/Component/Component.h> /* Integrian2D::Component */
#include <iostream> /* std::ostream, std::cout, ... */
#include <vector> /* std::vector */

enum class TetrominoShape
{
	I = 0,
	O = 1,
	L = 2,
	J = 3,
	T = 4,
	Z = 5,
	S = 6,
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

class Tetromino final : public Integrian2D::Component
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
	Tetromino(Integrian2D::GameObject* pOwner);
	Tetromino(Integrian2D::GameObject* pOwner, const TetrominoShape shape, const Integrian2D::Point2f& pivotStart);

	virtual Component* Clone(Integrian2D::GameObject* pOwner) noexcept override;

	virtual void Start() override;
	virtual void Render() const override;

	bool Rotate(const Rotation rot);
	bool Move(const Direction dir);

	void Invalidate();

	__NODISCARD TetrominoShape GetShape() const;
	__NODISCARD bool IsInvalid() const;
	__NODISCARD const std::vector<Integrian2D::Point2f>& GetCurrentPosition() const;
	__NODISCARD uint8_t GetRotation() const;

private:
	constexpr static uint8_t m_MaxNrOfBlocks{ 4u };
	void Rotate(const Rotation rot, const Integrian2D::Point2f& pivot);

	TetrominoShape m_Shape;
	std::vector<Integrian2D::Point2f> m_Points;
	uint8_t m_Rotation;
	class Board* m_pBoard;
};