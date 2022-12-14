#include "Board.h"

#include "../ScreenGrabber/ScreenGrabber.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../STB_Image/stb_image.h"

#include <iostream> /* std::cout */

#ifdef max
#undef max
#endif

Board::Board(ScreenGrabber* pScreenGrabber)
	: m_PreviousBoardState{}
	, m_BoardState{}
	, m_IsPreviousBoardStateSet{}
	, m_pScreenGrabber{ pScreenGrabber }
{}

void Board::Update(const uint64_t currentFrame)
{
	if (currentFrame == 0)
		return;

	if (currentFrame % 53 == 0)
	{
		if (!m_IsPreviousBoardStateSet)
			m_PreviousBoardState = m_BoardState;
		else
		{
			/* Get the current board state from the screenshot we took */
			SetBoardState();

#ifdef _DEBUG
			DebugBoardState();
#endif
		}

		m_IsPreviousBoardStateSet = !m_IsPreviousBoardStateSet;
	}
}

void Board::SetBoardState()
{
	Utils::ResetArray(m_BoardState);

	/* Analyze the image with STB */
	int width{};
	int height{};
	int bitsPerPixel{};
	unsigned char* pData = stbi_load(m_pScreenGrabber->GetPathToScreenshot().data(), &width, &height, &bitsPerPixel, 0);

	uint8_t i{};
	for (long y{}; y < m_BoardSize.y; ++y)
	{
		for (long x{}; x < m_BoardSize.x; ++x)
		{
			const unsigned char* pPixel
			{
				pData + bitsPerPixel *
				(
					(m_ScreenStart.y + (m_BlockSize.y + m_BlockOffset.y) * y) *
					width +
					(m_ScreenStart.x + (m_BlockSize.x + m_BlockOffset.x) * x)
				)
			};
			const unsigned char red = pPixel[0];
			const unsigned char green = pPixel[1];
			const unsigned char blue = pPixel[2];

			/* If the pixel is not black we want to save it */
			if (red != 0 || green != 0 || blue != 0)
				m_BoardState[i] = true;

			++i;
		}
	}

	stbi_image_free(pData);
}

bool Board::IsCoordinateOccupied(const Point& coord) const
{
	return m_BoardState[coord.x + coord.y * m_BoardSize.x];
}

bool Board::IsAnyRowComplete(const std::array<Point, g_MaxNrOfBlocks>& points)
{
	for (const Point& p : points)
		m_BoardState[p.x + p.y * m_BoardSize.x] = true;

	for (int y{ static_cast<int>(m_BoardSize.y - 1) }; y >= 0; --y)
	{
		bool isRowComplete{ true };
		for (uint8_t x{}; x < m_BoardSize.x; ++x)
		{
			if (!m_BoardState[x + y * m_BoardSize.x])
			{
				isRowComplete = false;
				break;
			}
		}

		if (isRowComplete)
		{
			for (const Point& p : points)
				m_BoardState[p.x + p.y * m_BoardSize.x] = false;

			return true;
		}
	}

	for (const Point& p : points)
		m_BoardState[p.x + p.y * m_BoardSize.x] = false;

	return false;
}

bool Board::IsRowComplete(const uint8_t row) const
{
	__ASSERT(row < m_BoardSize.y);

	bool isRowComplete{ true };
	for (uint8_t i{}; i < m_BoardSize.x; ++i)
	{
		if (!m_BoardState[i + row * m_BoardSize.x])
		{
			isRowComplete = false;
			break;
		}
	}

	return isRowComplete;
}

uint8_t Board::GetNewNrOfHoles(const std::array<Point, g_MaxNrOfBlocks>& points)
{
	const uint8_t origNrOfHoles{ GetNrOfHoles() };

	for (const Point& p : points)
		m_BoardState[p.x + p.y * m_BoardSize.x] = true;

	const uint8_t newNrOfHoles{ GetNrOfHoles() };

	for (const Point& p : points)
		m_BoardState[p.x + p.y * m_BoardSize.x] = false;

	return newNrOfHoles - origNrOfHoles;
}

uint8_t Board::GetBumpiness() const
{
	uint8_t totalBumpiness{};

	for (int8_t i{}; i < m_BoardSize.x; i += 2u)
	{
		if (i + 1 >= m_BoardSize.x)
			break;

		totalBumpiness += abs(GetColHeight(i) - GetColHeight(i + 1u));
	}

	return totalBumpiness;
}

uint8_t Board::GetNewBumpiness(const std::array<Point, g_MaxNrOfBlocks>& points)
{
	const uint8_t bumpiness{ GetBumpiness() };

	for (const Point& p : points)
		m_BoardState[p.x + p.y * m_BoardSize.x] = true;

	const uint8_t newBumpiness{ GetBumpiness() };

	for (const Point& p : points)
		m_BoardState[p.x + p.y * m_BoardSize.x] = false;

	return newBumpiness - bumpiness;
}

uint8_t Board::GetColHeight(const uint8_t col) const
{
	uint8_t colHeight{};
	for (int j{ static_cast<int>(m_BoardSize.y) - 1 }; j >= 0; --j)
		if (m_BoardState[col + j * m_BoardSize.x])
			++colHeight;

	return colHeight;
}

uint8_t Board::GetAggregateHeight() const
{
	uint8_t aggregrateHeight{};
	for (uint8_t i{}; i < m_BoardSize.x; ++i)
		aggregrateHeight += GetColHeight(i);

	return aggregrateHeight;
}

uint8_t Board::GetNewAggregateHeight(const std::array<Point, g_MaxNrOfBlocks>& points)
{
	const uint8_t height{ GetAggregateHeight() };

	for (const Point& p : points)
		m_BoardState[p.x + p.y * m_BoardSize.x] = true;

	const uint8_t newHeight{ GetAggregateHeight() };

	for (const Point& p : points)
		m_BoardState[p.x + p.y * m_BoardSize.x] = false;

	return newHeight - height;
}

const std::array<bool, m_BoardSize.x* m_BoardSize.y>& Board::GetPreviousBoardState() const
{
	return m_PreviousBoardState;
}

const std::array<bool, m_BoardSize.x* m_BoardSize.y>& Board::GetBoardState() const
{
	return m_BoardState;
}

#ifdef _DEBUG
void Board::DebugBoardState() const
{
	/* Clear console */
	system("cls");

	std::cout << "  ";
	for (uint8_t i{}; i < m_BoardSize.x; ++i)
		std::cout << "- ";

	std::cout << "\t\t";

	std::cout << "  ";
	for (uint8_t i{}; i < m_BoardSize.x; ++i)
		std::cout << "- ";

	std::cout << "\n";

	for (uint8_t y{}; y < m_BoardSize.y; ++y)
	{
		std::cout << "| ";
		for (uint8_t x{}; x < m_BoardSize.x; ++x)
		{
			std::cout << (m_BoardState[x + y * m_BoardSize.x] == true ? "x " : "  ");
		}
		std::cout << "|";

		std::cout << "\t\t";

		std::cout << "| ";
		for (uint8_t x{}; x < m_BoardSize.x; ++x)
		{
			std::cout << (m_PreviousBoardState[x + y * m_BoardSize.x] == true ? "x " : "  ");
		}
		std::cout << "|\n";
	}

	std::cout << "  ";
	for (uint8_t i{}; i < m_BoardSize.x; ++i)
		std::cout << "- ";

	std::cout << "\t\t";

	std::cout << "  ";
	for (uint8_t i{}; i < m_BoardSize.x; ++i)
		std::cout << "- ";

	std::cout << "\n\n";
}
#endif

uint8_t Board::GetNrOfHoles() const
{
	uint8_t nrOfHoles{};

	for (int y{ static_cast<int>(m_BoardSize.y - 1) }; y >= 0; --y)
	{
		for (uint8_t x{}; x < m_BoardSize.x; ++x)
		{
			if (!m_BoardState[x + y * m_BoardSize.x])
			{
				const bool isLeftAv{ x > 0u };
				const bool isDownAv{ y > 0 };
				const bool isRightAv{ x < m_BoardSize.x - 1 };
				const bool isUpAv{ y < static_cast<int>(m_BoardSize.y) - 1 };

				bool isHole = true;

				if (isHole && isLeftAv)
					if (!m_BoardState[(x - 1u) + y * m_BoardSize.x])
						isHole = false;

				if (isHole && isDownAv)
					if (!m_BoardState[x + (y + 1) * m_BoardSize.x])
						isHole = false;

				if (isHole && isRightAv)
					if (!m_BoardState[(x + 1u) + y * m_BoardSize.x])
						isHole = false;

				if (isHole && isUpAv)
					if (!m_BoardState[x + (y - 1) * m_BoardSize.x])
						isHole = false;

				if (isHole)
					++nrOfHoles;
			}
		}
	}

	return nrOfHoles;
}