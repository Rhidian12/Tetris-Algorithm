#include "Utils.h"
#include "../Board/Board.h"

#ifdef max
#undef max
#endif

namespace Utils
{
	void InitializeTetris()
	{
		SetProcessDPIAware();
		GetDesktopResolution(g_ScreenWidth, g_ScreenHeight);

		HWND tetris{ FindWindowA(NULL, g_TetrisWindow) };
		__ASSERT(tetris != nullptr);

		HWND tetrisVirtualPad{ FindWindowA(NULL, g_VirtualPadsWindow) };
		__ASSERT(tetrisVirtualPad != nullptr);

		SetForegroundWindow(tetris);
		SetForegroundWindow(tetrisVirtualPad);
	}

	void GetDesktopResolution(volatile LONG& x, volatile LONG& y)
	{
		RECT desktop{};

		// Get a handle to the desktop window
		const HWND hDesktop = GetDesktopWindow();

		// Get the size of screen to the variable desktop
		GetWindowRect(hDesktop, &desktop);

		// The top left corner will have coordinates (0,0)
		// and the bottom right corner will have coordinates
		// (horizontal, vertical)
		x = desktop.right;
		y = desktop.bottom;
	}

	void ConvertNDCToScreenCoords(LONG& x, LONG& y, const volatile Tetris::Point& wantedCoords)
	{
		x = static_cast<LONG>(std::numeric_limits<uint16_t>::max() / (float)g_ScreenWidth * (float)wantedCoords.x);
		y = static_cast<LONG>(std::numeric_limits<uint16_t>::max() / (float)g_ScreenHeight * (float)wantedCoords.y);
	}

	void SendMousePress(const volatile Tetris::Point& coords)
	{
		INPUT input{};

		input.type = INPUT_MOUSE;
		ConvertNDCToScreenCoords(input.mi.dx, input.mi.dy, coords);
		input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

		SendInput(1, &input, sizeof(INPUT));
	}

	Tetromino FindCurrentPiece(Board* pBoard)
	{
		constexpr static int maxNrOfBlocks{ 4 };

		/* Get the current piece */
		int rowIndices[maxNrOfBlocks]{};
		int colIndices[maxNrOfBlocks]{};
		int indexCounter{};

		const auto& board{ pBoard->GetBoardState() };
		for (int r{ m_BoardSize.y - 3 }; r >= 0; --r)
		{
			for (int c{}; c < m_BoardSize.x; ++c)
			{
				if (board[r][c])
				{
					rowIndices[indexCounter] = r;
					colIndices[indexCounter++] = c;
				}

				if (indexCounter >= maxNrOfBlocks)
					break;
			}

			if (indexCounter >= maxNrOfBlocks)
				break;
		}

		if (indexCounter != maxNrOfBlocks)
			return Tetromino{};

		int nrOfEqualRowIndices{}, nrOfEqualColIndices{};
		int rowCounter{}, colCounter{};

		/* First find the duplicate elements */
		int duplicateRowIndices[maxNrOfBlocks / 2u]{ std::numeric_limits<int>::max(), std::numeric_limits<int>::max() };
		int duplicateColIndices[maxNrOfBlocks / 2u]{ std::numeric_limits<int>::max(), std::numeric_limits<int>::max() };

		for (int i{}; i < maxNrOfBlocks; ++i)
		{
			for (int j{ i + 1 }; j < maxNrOfBlocks; ++j)
			{
				if (rowIndices[i] == rowIndices[j])
				{
					bool isUnique = true;
					for (int k{}; k < maxNrOfBlocks / 2; ++k)
						if (duplicateRowIndices[k] == rowIndices[i])
							isUnique = false;

					if (isUnique)
						duplicateRowIndices[rowCounter++] = rowIndices[i];
				}

				if (colIndices[i] == colIndices[j])
				{
					bool isUnique = true;
					for (int k{}; k < maxNrOfBlocks / 2; ++k)
						if (duplicateColIndices[k] == colIndices[i])
							isUnique = false;

					if (isUnique)
						duplicateColIndices[colCounter++] = colIndices[i];
				}
			}
		}

		/* Check how often each duplicate element appears */
		for (int i{}; i < maxNrOfBlocks / 2; ++i)
		{
			for (int j{}; j < maxNrOfBlocks; ++j)
			{
				if (duplicateRowIndices[i] == rowIndices[j])
					++nrOfEqualRowIndices;

				if (duplicateColIndices[i] == colIndices[j])
					++nrOfEqualColIndices;
			}
		}

		return Tetromino{ nrOfEqualRowIndices, nrOfEqualColIndices, rowIndices, colIndices, pBoard };
	}
}