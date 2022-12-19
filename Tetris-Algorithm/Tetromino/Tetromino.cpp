#include "Tetromino.h"

#include "../Board/Board.h"

#include <Utils/Utils.h>
#include <GameObject/GameObject.h>
#include <Components/TextureComponent/TextureComponent.h>
#include <TextureManager/TextureManager.h>
#include <Renderer/Renderer.h>
#include <SceneManager/SceneManager.h>
#include <Scene/Scene.h>

#include <stdint.h>
#include <limits>

#ifdef max
#undef max
#endif

Tetromino::Tetromino(Integrian2D::GameObject* pOwner)
	: Component{ pOwner }
	, m_Shape{ TetrominoShape::NONE }
	, m_Points{}
	, m_Rotation{}
	, m_pBoard{}
{}

Tetromino::Tetromino(Integrian2D::GameObject* pOwner, const TetrominoShape shape, const Integrian2D::Point2f& pivotStart)
	: Component{ pOwner }
	, m_Shape{ shape }
	, m_Points{}
	, m_Rotation{}
	, m_pBoard{}
{
	using namespace Integrian2D;

	m_Points.resize(4);

	std::cout << "Spawned shape: " << shape << "\n";

	switch (shape)
	{
	case TetrominoShape::I:
		m_Points[0] = Point2f{ pivotStart.x - (g_BlockSize.x + g_BlockOffset.x) * 2.f, pivotStart.y };
		m_Points[1] = Point2f{ pivotStart.x - (g_BlockSize.x + g_BlockOffset.x), pivotStart.y };
		m_Points[2] = pivotStart;
		m_Points[3] = Point2f{ pivotStart.x + (g_BlockSize.x + g_BlockOffset.x), pivotStart.y };
		break;
	case TetrominoShape::L:
		m_Points[0] = Point2f{ pivotStart.x - (g_BlockSize.x + g_BlockOffset.x), pivotStart.y - (g_BlockSize.y + g_BlockOffset.y) };
		m_Points[1] = Point2f{ pivotStart.x - (g_BlockSize.x + g_BlockOffset.x), pivotStart.y };
		m_Points[2] = pivotStart;
		m_Points[3] = Point2f{ pivotStart.x + (g_BlockSize.x + g_BlockOffset.x), pivotStart.y };
		break;
	case TetrominoShape::J:
		m_Points[0] = Point2f{ pivotStart.x - (g_BlockSize.x + g_BlockOffset.x), pivotStart.y };
		m_Points[1] = pivotStart;
		m_Points[2] = Point2f{ pivotStart.x + (g_BlockSize.x + g_BlockOffset.x), pivotStart.y };
		m_Points[3] = Point2f{ pivotStart.x + (g_BlockSize.x + g_BlockOffset.x), pivotStart.y - (g_BlockSize.y + g_BlockOffset.y) };
		break;
	case TetrominoShape::T:
		m_Points[0] = Point2f{ pivotStart.x - (g_BlockSize.x + g_BlockOffset.x), pivotStart.y };
		m_Points[1] = pivotStart;
		m_Points[2] = Point2f{ pivotStart.x, pivotStart.y - (g_BlockSize.y + g_BlockOffset.y) };
		m_Points[3] = Point2f{ pivotStart.x + (g_BlockSize.x + g_BlockOffset.x), pivotStart.y };
		break;
	case TetrominoShape::Z:
		m_Points[0] = Point2f{ pivotStart.x - (g_BlockSize.x + g_BlockOffset.x), pivotStart.y };
		m_Points[1] = pivotStart;
		m_Points[2] = Point2f{ pivotStart.x, pivotStart.y - (g_BlockSize.y + g_BlockOffset.y) };
		m_Points[3] = Point2f{ pivotStart.x + (g_BlockSize.x + g_BlockOffset.x), pivotStart.y - (g_BlockSize.y + g_BlockOffset.y) };
		break;
	case TetrominoShape::S:
		m_Points[0] = Point2f{ pivotStart.x - (g_BlockSize.x + g_BlockOffset.x), pivotStart.y - (g_BlockSize.y + g_BlockOffset.y) };
		m_Points[1] = Point2f{ pivotStart.x, pivotStart.y - (g_BlockSize.y + g_BlockOffset.y) };
		m_Points[2] = pivotStart;
		m_Points[3] = Point2f{ pivotStart.x + (g_BlockSize.x + g_BlockOffset.x), pivotStart.y };
		break;
	case TetrominoShape::O:
		m_Points[0] = pivotStart;
		m_Points[1] = Point2f{ pivotStart.x + (g_BlockSize.x + g_BlockOffset.x), pivotStart.y };
		m_Points[2] = Point2f{ pivotStart.x, pivotStart.y - (g_BlockSize.y + g_BlockOffset.y) };
		m_Points[3] = Point2f{ pivotStart.x + (g_BlockSize.x + g_BlockOffset.x), pivotStart.y - (g_BlockSize.y + g_BlockOffset.y) };
		break;
	}

	m_pBoard = SceneManager::GetInstance()->GetActiveScene()->GetGameObject("Board")->GetComponentByType<Board>();
}

Integrian2D::Component* Tetromino::Clone(Integrian2D::GameObject* pOwner) noexcept
{
	using namespace Integrian2D;

	Point2f pivot{};

	switch (m_Shape)
	{
	case TetrominoShape::I:
		pivot = m_Points[1];
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

	auto tet = new Tetromino{ pOwner, m_Shape, pivot };
	tet->m_Points = m_Points;

	return tet;
}

void Tetromino::Start()
{
	using namespace Integrian2D;

	//Texture* pTex{};
	//switch (m_Shape)
	//{
	//case TetrominoShape::I:
	//	pTex = TextureManager::GetInstance()->GetTexture("I-Tetromino");
	//	break;
	//case TetrominoShape::L:
	//	pTex = TextureManager::GetInstance()->GetTexture("L-Tetromino");
	//	break;
	//case TetrominoShape::J:
	//	pTex = TextureManager::GetInstance()->GetTexture("J-Tetromino");
	//	break;
	//case TetrominoShape::T:
	//	pTex = TextureManager::GetInstance()->GetTexture("T-Tetromino");
	//	break;
	//case TetrominoShape::Z:
	//	pTex = TextureManager::GetInstance()->GetTexture("Z-Tetromino");
	//	break;
	//case TetrominoShape::S:
	//	pTex = TextureManager::GetInstance()->GetTexture("S-Tetromino");
	//	break;
	//case TetrominoShape::O:
	//	pTex = TextureManager::GetInstance()->GetTexture("O-Tetromino");
	//	break;
	//}

	//m_pOwner->AddComponent(new TextureComponent{ m_pOwner, pTex });
}

void Tetromino::Render() const
{
	using namespace Integrian2D;

	RGBColour colour{};
	switch (m_Shape)
	{
	case TetrominoShape::I:
		colour = RGBColour{ 0u, 228u, 255u };
		break;
	case TetrominoShape::L:
		colour = RGBColour{ 255u, 141u, 0u };
		break;
	case TetrominoShape::J:
		colour = RGBColour{ 255u, 81u, 188u };
		break;
	case TetrominoShape::T:
		colour = RGBColour{ 159u, 0u, 150u };
		break;
	case TetrominoShape::Z:
		colour = RGBColour{ 105u, 182u, 37u };
		break;
	case TetrominoShape::S:
		colour = RGBColour{ 246u, 0u, 0u };
		break;
	case TetrominoShape::O:
		colour = RGBColour{ 250u, 255u, 0u };
		break;
	}

	for (const Point2f& p : m_Points)
		Renderer::GetInstance()->RenderFilledRectangle(
			Rectf
			{
				Point2f{ p.x + g_BlockOffset.x / 2.f, p.y + g_BlockOffset.y / 2.f},
				g_BlockSize.x, g_BlockSize.y
			},
			colour);
}

bool Tetromino::Rotate(const Rotation rot)
{
	using namespace Integrian2D;

	__ASSERT(m_Shape != TetrominoShape::NONE);
	__ASSERT(m_pBoard != nullptr);

	if (m_Shape == TetrominoShape::O)
		return true;

	Point2f pivot{};

	switch (m_Shape)
	{
	case TetrominoShape::I:
		pivot = m_Points[2];
		break;
	case TetrominoShape::L:
		pivot = m_Points[2];
		break;
	case TetrominoShape::J:
		pivot = m_Points[1];
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

	bool isIllegalMove{};
	for (const Point2f& point : m_Points)
	{
		if (point.x < 0 || point.x >= g_BoardSize.x ||
			point.y < 0 || point.y >= g_BoardSize.y ||
			m_pBoard->IsCoordinateOccupied(point))
		{
			isIllegalMove = true;
			break;
		}
	}

	if (isIllegalMove)
	{
		if (static_cast<uint8_t>(rot) == static_cast<uint8_t>(Rotation::Clockwise))
			Rotate(Rotation::CCW, pivot);
		else
			Rotate(Rotation::CW, pivot);
	}
	else
	{
		if (m_Shape == TetrominoShape::I && m_Rotation > 0u)
		{
			/* Because we're using a block as pivot instead of a coordinate, we need to rotate a full 360 degrees for
			 the I rotation to be correct */
			Rotate(rot, pivot);
			Rotate(rot, pivot);
			Rotate(rot, pivot);

			m_Rotation = 0u;
		}
		else
		{
			if (++m_Rotation > 3u)
				m_Rotation = 0u;
		}
	}

	return !isIllegalMove;
}

bool Tetromino::Move(const Direction dir)
{
	using namespace Integrian2D;

	__ASSERT(m_Shape != TetrominoShape::NONE);
	__ASSERT(m_pBoard != nullptr);

	Point2f direction{};

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

	m_pBoard->Remove(m_Points);

	bool isIllegalMove{};
	int lastIndex{ static_cast<int>(m_Points.size() - 1) };
	for (int i{}; i <= lastIndex; ++i)
	{
		Point2f& point = m_Points[i];

		if (!isIllegalMove)
		{
			point.x += direction.x * g_BlockSize.x * g_BlockOffset.x;
			point.y += direction.y * g_BlockSize.y * g_BlockOffset.y;
			if (point.x < 0 || point.x >= g_BoardSize.x ||
				point.y < 0 || point.y >= g_BoardSize.y ||
				m_pBoard->IsCoordinateOccupied(point))
			{
				isIllegalMove = true;
				lastIndex = i;
				i = -1; /* gets incremented to 0 */
			}
		}
		else
		{
			point.x -= direction.x * g_BlockSize.x * g_BlockOffset.x;
			point.y -= direction.y * g_BlockSize.y * g_BlockOffset.y;
		}
	}

	m_pBoard->Add(m_Points);

	return !isIllegalMove;
}

void Tetromino::Invalidate()
{
	m_Shape = TetrominoShape::NONE;
	m_Rotation = 0u;
	m_Points.clear();
}

TetrominoShape Tetromino::GetShape() const
{
	return m_Shape;
}

bool Tetromino::IsInvalid() const
{
	return m_Shape == TetrominoShape::NONE;
}

const std::vector<Integrian2D::Point2f>& Tetromino::GetCurrentPosition() const
{
	return m_Points;
}

uint8_t Tetromino::GetRotation() const
{
	return m_Rotation;
}

void Tetromino::Rotate(const Rotation rot, const Integrian2D::Point2f& pivot)
{
	using namespace Integrian2D;

	float s{ static_cast<uint8_t>(rot) == static_cast<uint8_t>(Rotation::Clockwise) ? -1.f : 1.f };
	/* constexpr float c{ 0.f }; */

	for (Point2f& point : m_Points)
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