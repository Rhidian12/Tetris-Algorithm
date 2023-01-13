#include "Board.h"

#include "../ScreenGrabber/ScreenGrabber.h"

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
#include <vfw.h>

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

void Board::Remove(const std::array<Tetris::Point, g_MaxNrOfBlocks>& points)
{
	for (const Tetris::Point& p : points)
		m_BoardState[p.y][p.x] = false;
}

void Board::Add(const std::array<Tetris::Point, g_MaxNrOfBlocks>& points)
{
	for (const Tetris::Point& p : points)
		m_BoardState[p.y][p.x] = true;
}

BOOL SaveHBITMAPToFile(HBITMAP hBitmap, LPCTSTR lpszFileName)
{
	HDC hDC;
	int iBits;
	WORD wBitCount;
	DWORD dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
	BITMAP Bitmap0;
	BITMAPFILEHEADER bmfHdr;
	BITMAPINFOHEADER bi;
	LPBITMAPINFOHEADER lpbi;
	HANDLE fh, hDib, hPal, hOldPal2 = NULL;
	hDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
		wBitCount = 1;
	else if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else
		wBitCount = 24;
	GetObject(hBitmap, sizeof(Bitmap0), (LPSTR)&Bitmap0);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap0.bmWidth;
	bi.biHeight = -Bitmap0.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 256;
	dwBmBitsSize = ((Bitmap0.bmWidth * wBitCount + 31) & ~31) / 8
		* Bitmap0.bmHeight;
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = GetDC(NULL);
		hOldPal2 = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}


	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap0.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
		+ dwPaletteSize, (BITMAPINFO*)lpbi, DIB_RGB_COLORS);

	if (hOldPal2)
	{
		SelectPalette(hDC, (HPALETTE)hOldPal2, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
	}

	fh = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (fh == INVALID_HANDLE_VALUE)
		return FALSE;

	bmfHdr.bfType = 0x4D42; // "BM"
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
	return TRUE;
}

void Board::SetBoardState()
{
	for (int r{}; r < m_BoardSize.y; ++r)
		for (int c{}; c < m_BoardSize.x; ++c)
			m_BoardState[r][c] = false;

	// get screen dimensions
	const int x1{ GetSystemMetrics(SM_XVIRTUALSCREEN) };
	const int y1{ GetSystemMetrics(SM_YVIRTUALSCREEN) };
	const int x2{ GetSystemMetrics(SM_CXVIRTUALSCREEN) };
	const int y2{ GetSystemMetrics(SM_CYVIRTUALSCREEN) };
	const int w{ x2 - x1 };
	const int h{ y2 - y1 };

	// copy screen to bitmap
	HDC     hScreen = GetDC(NULL); // get the handle to the device context for the entire screen 
	HDC     hDC = CreateCompatibleDC(hScreen); // create a memory device context compatible with the screen 
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, w, h); // create a bitmap compatible with the screen
	HGDIOBJ old_obj = SelectObject(hDC, hBitmap); // select the bitmap into the memory device context
	BOOL    bRet = BitBlt(hDC, 0, 0, w, h, hScreen, x1, y1, SRCCOPY); // copy the colour data from the screen to the memory device context

	for (int r{}; r < m_BoardSize.y - 2; ++r)
	{
		for (int c{}; c < m_BoardSize.x; ++c)
		{
			const int x{ ((c * (m_BlockSize.x + m_BlockOffset.x)) + m_ScreenStart.x) };
			const int y{ (m_ScreenStart.y - (r * (m_BlockSize.y + m_BlockOffset.y))) };

			COLORREF color = GetPixel(hDC, x, y);

			/* If the pixel is not black we want to save it */
			if (GetRValue(color) != 0 || GetGValue(color) != 0 || GetBValue(color) != 0)
				m_BoardState[r][c] = true;
		}
	}

	// SaveHBITMAPToFile(hBitmap, L"Test.bmp");

	// clean up
	SelectObject(hDC, old_obj);
	DeleteDC(hDC);
	ReleaseDC(NULL, hScreen);
	DeleteObject(hBitmap);

	if (DoesRowContainPieces(m_BoardSize.y - 3) || DoesRowContainPieces(m_BoardSize.y - 4))
		m_OnNewPieceSpawned.Invoke();
}

bool Board::IsCoordinateOccupied(const Tetris::Point& coord) const
{
	if (coord.y < 0)
		return false;

	return m_BoardState[coord.y][coord.x];
}

bool Board::IsAnyRowComplete(const std::array<Tetris::Point, g_MaxNrOfBlocks>& points)
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

int Board::GetNewNrOfHoles(const std::array<Tetris::Point, g_MaxNrOfBlocks>& points)
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

int Board::GetNewBumpiness(const std::array<Tetris::Point, g_MaxNrOfBlocks>& points)
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

int Board::GetNewAggregateHeight(const std::array<Tetris::Point, g_MaxNrOfBlocks>& points)
{
	//const int oldHeight{ GetAggregateHeight() };

	//Add(points);

	//const int newHeight{ GetAggregateHeight() };

	//Remove(points);

	//return newHeight /*- oldHeight*/;

	const int oldHighestRow{ GetHighestRow() };
	// const int oldNrOfPieces{ GetNrOfPiecesInRow(oldHighestRow) };

	Add(points);

	const int newHighestRow{ GetHighestRow() };
	int newNrOfPieces{};

	for (int i{ newHighestRow }; i > oldHighestRow; --i)
		newNrOfPieces += GetNrOfPiecesInRow(i);

	// newNrOfPieces += GetNrOfPiecesInRow(oldHighestRow) - oldNrOfPieces;

	Remove(points);

	return newNrOfPieces;
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

int Board::GetNewNrOfCompletedLines(const std::array<Tetris::Point, g_MaxNrOfBlocks>& points)
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

int Board::GetHighestRow() const
{
	for (int r{ m_BoardSize.y - 3 }; r >= 0; --r)
		for (int c{}; c < m_BoardSize.x; ++c)
			if (m_BoardState[r][c])
				return r;

	return 0;
}

int Board::GetNrOfPiecesInRow(const int row) const
{
	__ASSERT(row >= 0 && row < m_BoardSize.y - 2);

	int nrOfPieces{};
	for (int c{}; c < m_BoardSize.x; ++c)
		if (m_BoardState[row][c])
			++nrOfPieces;

	return nrOfPieces;
}