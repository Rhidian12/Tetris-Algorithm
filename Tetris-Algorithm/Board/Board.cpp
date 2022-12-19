#include "Board.h"

#include <Renderer/Renderer.h>
#include <GameObject/GameObject.h>
#include <SceneManager/SceneManager.h>
#include <Scene/Scene.h>
#ifdef _DEBUG
#include <iostream> /* std::cout */
#endif

#include "../Tetromino/Tetromino.h"
#include "../FrameCounter/FrameCounter.h"

#ifdef max
#undef max
#endif

Board::Board(Integrian2D::GameObject* pOwner)
	: Component{ pOwner }
	, m_BoardState{}
	, m_Tetrominos{}
	, m_pCurrentPiece{}
{}

Integrian2D::Component* Board::Clone(Integrian2D::GameObject* pOwner) noexcept
{
	return new Board{ pOwner };
}

void Board::Start()
{
	Integrian2D::SceneManager::GetInstance()->GetActiveScene()->GetGameObject("FrameCounter")->
		GetComponentByType<FrameCounter>()->GetDelegate().Bind(this, &Board::OnFrameUpdate);
}

#ifdef _DEBUG
void Board::Render() const
{
	using namespace Integrian2D;

	Renderer* const pRenderer{ Renderer::GetInstance() };

	constexpr float horOffset{ (g_ScreenWidth - 400u) / 2.f };
	for (int x{}; x < g_BoardSize.x + 1; ++x)
	{
		pRenderer->RenderLine(
			PLinef
			{
				Point2f{ horOffset + x * (g_BlockSize.x + g_BlockOffset.x), 0.f },
				Point2f{ horOffset + x * (g_BlockSize.x + g_BlockOffset.x), 680.f }
			},
			2.f);
	}

	for (int y{}; y < g_BoardSize.y - 1; ++y)
	{
		pRenderer->RenderLine(
			PLinef
			{
				Point2f{ horOffset, y * (g_BlockSize.y + g_BlockOffset.y) },
				Point2f{ g_ScreenWidth - horOffset, y * (g_BlockSize.y + g_BlockOffset.y) }
			},
			2.f);
	}
}
#endif

#ifdef _DEBUG
void Board::Update()
{
	DebugBoardState();
}
#endif

void Board::OnFrameUpdate(const uint64_t currentFrame)
{
	if (m_pCurrentPiece)
		m_pCurrentPiece->Move(Tetromino::Direction::Down);

	SetBoardState();
}

void Board::Remove(const std::vector<Integrian2D::Point2f>& points)
{
	for (const Integrian2D::Point2f& p : points)
		m_BoardState[ConvertPosToIndex(p)] = false;
}

void Board::Add(const std::vector<Integrian2D::Point2f>& points)
{
	for (const Integrian2D::Point2f& p : points)
		m_BoardState[ConvertPosToIndex(p)] = true;
}

void Board::SetPiece(Tetromino* pPiece)
{
	__ASSERT(pPiece != nullptr);

	m_pCurrentPiece = pPiece;

	Add(pPiece->GetCurrentPosition());
	m_Tetrominos.push_back(pPiece);
}

void Board::SetBoardState()
{
	Utils::ResetArray(m_BoardState);

	for (const Tetromino* const pTetromino : m_Tetrominos)
		for (const Integrian2D::Point2f& p : pTetromino->GetCurrentPosition())
			m_BoardState[ConvertPosToIndex(p)] = true;
}

uint8_t Board::ConvertPosToIndex(Integrian2D::Point2f p) const
{
	constexpr float horOffset{ (g_ScreenWidth - 400u) / 2.f };
	constexpr float verSize{ 680.f };

	p.x -= horOffset;
	p.y = fabs(p.y - verSize);

	return static_cast<uint8_t>((p.x / (g_BlockSize.x + g_BlockOffset.x) + 1) + 
		(p.y / (g_BlockSize.y + g_BlockOffset.y) - 1) * g_BoardSize.x);
}

bool Board::IsCoordinateOccupied(const Integrian2D::Point2f& coord) const
{
	return m_BoardState[ConvertPosToIndex(coord)];
}

bool Board::IsAnyRowComplete(const std::vector<Integrian2D::Point2f>& points)
{
	for (const Integrian2D::Point2f& p : points)
		m_BoardState[ConvertPosToIndex(p)] = true;

	for (int y{ static_cast<int>(g_BoardSize.y - 1) }; y >= 0; --y)
	{
		bool isRowComplete{ true };
		for (uint8_t x{}; x < g_BoardSize.x; ++x)
		{
			if (!m_BoardState[static_cast<uint32_t>(x + y * g_BoardSize.x)])
			{
				isRowComplete = false;
				break;
			}
		}

		if (isRowComplete)
		{
			for (const Integrian2D::Point2f& p : points)
				m_BoardState[ConvertPosToIndex(p)] = false;

			return true;
		}
	}

	for (const Integrian2D::Point2f& p : points)
		m_BoardState[ConvertPosToIndex(p)] = false;

	return false;
}

bool Board::IsRowComplete(const uint8_t row) const
{
	__ASSERT(row < g_BoardSize.y);

	bool isRowComplete{ true };
	for (uint8_t i{}; i < g_BoardSize.x; ++i)
	{
		if (!m_BoardState[static_cast<uint32_t>(i + row * g_BoardSize.x)])
		{
			isRowComplete = false;
			break;
		}
	}

	return isRowComplete;
}

uint8_t Board::GetNewNrOfHoles(const std::vector<Integrian2D::Point2f>& points)
{
	const uint8_t origNrOfHoles{ GetNrOfHoles() };

	for (const Integrian2D::Point2f& p : points)
		m_BoardState[ConvertPosToIndex(p)] = true;

	const uint8_t newNrOfHoles{ GetNrOfHoles() };

	for (const Integrian2D::Point2f& p : points)
		m_BoardState[ConvertPosToIndex(p)] = false;

	return newNrOfHoles - origNrOfHoles;
}

uint8_t Board::GetBumpiness() const
{
	uint8_t totalBumpiness{};

	for (int8_t i{}; i < g_BoardSize.x; i += 2u)
	{
		if (i + 1 >= g_BoardSize.x)
			break;

		totalBumpiness += abs(GetColHeight(i) - GetColHeight(i + 1u));
	}

	return totalBumpiness;
}

uint8_t Board::GetNewBumpiness(const std::vector<Integrian2D::Point2f>& points)
{
	const uint8_t bumpiness{ GetBumpiness() };

	for (const Integrian2D::Point2f& p : points)
		m_BoardState[ConvertPosToIndex(p)] = true;

	const uint8_t newBumpiness{ GetBumpiness() };

	for (const Integrian2D::Point2f& p : points)
		m_BoardState[ConvertPosToIndex(p)] = false;

	return newBumpiness - bumpiness;
}

uint8_t Board::GetColHeight(const uint8_t col) const
{
	uint8_t colHeight{};
	for (int j{ static_cast<int>(g_BoardSize.y) - 1 }; j >= 0; --j)
		if (m_BoardState[static_cast<uint32_t>(col + j * g_BoardSize.x)])
			++colHeight;

	return colHeight;
}

uint8_t Board::GetAggregateHeight() const
{
	uint8_t aggregrateHeight{};
	for (uint8_t i{}; i < g_BoardSize.x; ++i)
		aggregrateHeight += GetColHeight(i);

	return aggregrateHeight;
}

uint8_t Board::GetNewAggregateHeight(const std::vector<Integrian2D::Point2f>& points)
{
	const uint8_t height{ GetAggregateHeight() };

	for (const Integrian2D::Point2f& p : points)
		m_BoardState[ConvertPosToIndex(p)] = true;

	const uint8_t newHeight{ GetAggregateHeight() };

	for (const Integrian2D::Point2f& p : points)
		m_BoardState[ConvertPosToIndex(p)] = false;

	return newHeight - height;
}

const std::array<bool, g_NrOfBoard>& Board::GetBoardState() const
{
	return m_BoardState;
}

Tetromino* const Board::GetCurrentPiece() const
{
	return m_pCurrentPiece;
}

Integrian2D::Point2f Board::GetStartPos() const
{
	constexpr float horOffset{ (g_ScreenWidth - 400u) / 2.f };

	return Integrian2D::Point2f{ horOffset + (g_BlockSize.x + g_BlockOffset.x) * g_BoardSize.x / 2.f - 1,
		(g_BoardSize.y - 3) * (g_BlockSize.y + g_BlockOffset.y) };
}

#ifdef _DEBUG
void Board::DebugBoardState() const
{
	/* Clear console */
	system("cls");

	std::cout << "  ";
	for (uint8_t i{}; i < g_BoardSize.x; ++i)
		std::cout << "- ";

	std::cout << "\n";

	for (uint8_t y{}; y < g_BoardSize.y; ++y)
	{
		std::cout << "| ";
		for (uint8_t x{}; x < g_BoardSize.x; ++x)
		{
			std::cout << (m_BoardState[static_cast<uint32_t>(x + y * g_BoardSize.x)] == true ? "x " : "  ");
		}
		std::cout << "|\n";
	}

	std::cout << "  ";
	for (uint8_t i{}; i < g_BoardSize.x; ++i)
		std::cout << "- ";

	std::cout << "\n\n";
}
#endif

uint8_t Board::GetNrOfHoles() const
{
	uint8_t nrOfHoles{};

	for (int y{ static_cast<int>(g_BoardSize.y - 1) }; y >= 0; --y)
	{
		for (uint8_t x{}; x < g_BoardSize.x; ++x)
		{
			if (!m_BoardState[static_cast<uint32_t>(x + y * g_BoardSize.x)])
			{
				const bool isLeftAv{ x > 0u };
				const bool isDownAv{ y < g_BoardSize.y - 1 };
				const bool isRightAv{ x < g_BoardSize.x - 1 };
				const bool isUpAv{ y > 0 };

				bool isHole = true;

				if (isHole && isLeftAv)
					if (!m_BoardState[static_cast<uint32_t>((x - 1u) + y * g_BoardSize.x)])
						isHole = false;

				if (isHole && isDownAv)
					if (!m_BoardState[static_cast<uint32_t>(x + (y + 1) * g_BoardSize.x)])
						isHole = false;

				if (isHole && isRightAv)
					if (!m_BoardState[static_cast<uint32_t>((x + 1u) + y * g_BoardSize.x)])
						isHole = false;

				if (isHole && isUpAv)
					if (!m_BoardState[static_cast<uint32_t>(x + (y - 1) * g_BoardSize.x)])
						isHole = false;

				if (isHole)
					++nrOfHoles;
			}
		}
	}

	return nrOfHoles;
}