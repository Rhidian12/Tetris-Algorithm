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
	, m_OnNewPieceSpawned{}
{}

void Board::UpdatePrevious()
{
	m_PreviousBoardState = m_BoardState;
}

void Board::Update()
{
	/* Get the current board state from the screenshot we took */
	SetBoardState();

#ifdef _DEBUG
	// DebugBoardState();
#endif
}

void Board::Remove(const std::array<Point, g_MaxNrOfBlocks>& points)
{
	for (const Point& p : points)
		m_BoardState[p.y][p.x] = false;
}

void Board::Add(const std::array<Point, g_MaxNrOfBlocks>& points)
{
	for (const Point& p : points)
		m_BoardState[p.y][p.x] = true;
}

void Board::SetBoardState()
{
	Utils::ResetArray(m_BoardState);

	for (int r{}; r < m_BoardSize.y; ++r)
		for (int c{}; c < m_BoardSize.x; ++c)
			__ASSERT(m_BoardState[r][c] == false);

	/* Analyze the image with STB */
	int width{};
	int height{};
	int bytesPerPixel{};
	unsigned char* pData = stbi_load(m_pScreenGrabber->GetPathToScreenshot().data(), &width, &height, &bytesPerPixel, 3);

	for (int r{}; r < m_BoardSize.y - 2; ++r)
	{
		for (int c{}; c < m_BoardSize.x; ++c)
		{
			const int x{ ((c * (m_BlockSize.x + m_BlockOffset.x)) + m_ScreenStart.x) };
			const int y{ (m_ScreenStart.y - (r * (m_BlockSize.y + m_BlockOffset.y))) };

			const unsigned char* pPixel{ pData + (x + width * y) * bytesPerPixel };
			const unsigned char red = pPixel[0];
			const unsigned char green = pPixel[1];
			const unsigned char blue = pPixel[2];

			/* If the pixel is not black we want to save it */
			if (red != 0 || green != 0 || blue != 0)
				m_BoardState[r][c] = true;
		}
	}

	stbi_image_free(pData);

	if (DoesRowContainPieces(m_BoardSize.y - 3) || DoesRowContainPieces(m_BoardSize.y - 4))
		m_OnNewPieceSpawned.Invoke();
}

bool Board::IsCoordinateOccupied(const Point& coord) const
{
	if (coord.y < 0)
		return false;

	return m_BoardState[coord.y][coord.x];
}

bool Board::IsAnyRowComplete(const std::array<Point, g_MaxNrOfBlocks>& points)
{
	Add(points);

	for (int r{}; r < m_BoardSize.y - 2; ++r)
	{
		bool isRowComplete{ true };
		for (int c{}; c < m_BoardSize.x; ++c)
		{
			if (!m_BoardState[r][c])
			{
				isRowComplete = false;
				break;
			}
		}

		if (isRowComplete)
		{
			Remove(points);

			return true;
		}
	}

	Remove(points);

	return false;
}

bool Board::IsRowComplete(const int row) const
{
	__ASSERT(row < m_BoardSize.y - 2 && row >= 0);

	bool isRowComplete{ true };
	for (int c{}; c < m_BoardSize.x; ++c)
	{
		if (!m_BoardState[row][c])
		{
			isRowComplete = false;
			break;
		}
	}

	return isRowComplete;
}

int Board::GetNewNrOfHoles(const std::array<Point, g_MaxNrOfBlocks>& points)
{
	const int oldNrOfHoles{ GetNrOfHoles() };

	Add(points);

	const int newNrOfHoles{ GetNrOfHoles() };

	Remove(points);

	return newNrOfHoles - oldNrOfHoles;
}

int Board::GetBumpiness() const
{
	int totalBumpiness{};

	for (int i{}; i < m_BoardSize.x - 1; ++i)
	{
		totalBumpiness += abs(GetColHeight(i) - GetColHeight(i + 1));
	}

	return totalBumpiness;
}

int Board::GetNewBumpiness(const std::array<Point, g_MaxNrOfBlocks>& points)
{
	const int oldBumpiness{ GetBumpiness() };

	Add(points);

	const int newBumpiness{ GetBumpiness() };

	Remove(points);

	return newBumpiness - oldBumpiness;
}

int Board::GetColHeight(const int col) const
{
	__ASSERT(col >= 0 && col < m_BoardSize.x);

	int colHeight{};
	for (int r{ m_BoardSize.y - 3 }; r >= 0; --r)
	{
		if (m_BoardState[r][col])
			return r;
	}

	return 0;
}

int Board::GetAggregateHeight() const
{
	int aggregrateHeight{};
	for (int c{}; c < m_BoardSize.x; ++c)
		aggregrateHeight += GetColHeight(c);

	return aggregrateHeight;
}

int Board::GetNewAggregateHeight(const std::array<Point, g_MaxNrOfBlocks>& points)
{
	const int oldHeight{ GetAggregateHeight() };

	Add(points);

	const int newHeight{ GetAggregateHeight() };

	Remove(points);

	return newHeight /*- oldHeight*/;
}

bool Board::DoesRowContainPieces(const int row) const
{
	__ASSERT(row >= 0 && row <= m_BoardSize.y - 2);

	for (int c{}; c < m_BoardSize.x; ++c)
		if (m_BoardState[row][c])
			return true;

	return false;
}

const Board::BoardStorage& Board::GetPreviousBoardState() const
{
	return m_PreviousBoardState;
}

const Board::BoardStorage& Board::GetBoardState() const
{
	return m_BoardState;
}

Delegate<>& Board::GetOnNewPieceSpawned()
{
	return m_OnNewPieceSpawned;
}

int Board::GetNrOfCompletedLines() const
{
	int nrOfLinesComplete{};

	for (int r{}; r < m_BoardSize.y - 2; ++r)
		if (IsRowComplete(r))
			++nrOfLinesComplete;

	return nrOfLinesComplete;
}

int Board::GetNewNrOfCompletedLines(const std::array<Point, g_MaxNrOfBlocks>& points)
{
	Add(points);

	const int newNrOfLinesCompleted{ GetNrOfCompletedLines() };

	Remove(points);

	return newNrOfLinesCompleted;
}

#ifdef _DEBUG
void Board::DebugBoardState() const
{
	/* Clear console */
	system("cls");

	std::cout << "  ";
	for (int c{}; c < m_BoardSize.x; ++c)
		std::cout << "- ";

	std::cout << "\t\t";

	std::cout << "  ";
	for (int c{}; c < m_BoardSize.x; ++c)
		std::cout << "- ";

	std::cout << "\n";

	for (int r{ m_BoardSize.y - 3 }; r >= 0; --r)
	{
		std::cout << "| ";
		for (int c{}; c < m_BoardSize.x; ++c)
		{
			std::cout << (m_BoardState[r][c] == true ? "x " : "  ");
		}
		std::cout << "|";

		std::cout << "\t\t";

		std::cout << "| ";
		for (uint8_t c{}; c < m_BoardSize.x; ++c)
		{
			std::cout << (m_PreviousBoardState[r][c] == true ? "x " : "  ");
		}
		std::cout << "|\n";
	}

	std::cout << "  ";
	for (uint8_t c{}; c < m_BoardSize.x; ++c)
		std::cout << "- ";

	std::cout << "\t\t";

	std::cout << "  ";
	for (uint8_t c{}; c < m_BoardSize.x; ++c)
		std::cout << "- ";

	std::cout << "\n\n";

	std::cout << "Nr of Holes: " << static_cast<int>(GetNrOfHoles()) << "\n";
}
#endif

int Board::GetNrOfHoles() const
{
	int nrOfHoles{};

	for (int c{}; c < m_BoardSize.x; ++c)
	{
		bool doesColContainBlock{};
		for (int r{ m_BoardSize.y - 3 }; r >= 0; --r)
		{
			if (m_BoardState[r][c])
				doesColContainBlock = true;
			else if (!m_BoardState[r][c] && doesColContainBlock)
				++nrOfHoles;
		}
	}

	return nrOfHoles;
}