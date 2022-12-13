#include "Board.h"

#include "../ScreenGrabber/ScreenGrabber.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../STB_Image/stb_image.h"

#include <iostream> /* std::cout */

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