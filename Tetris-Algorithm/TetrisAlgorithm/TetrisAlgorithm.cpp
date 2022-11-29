#include "TetrisAlgorithm.h"
#include "../Utils/Utils.h"

#include <filesystem> /* std::filesystem */

#define STB_IMAGE_IMPLEMENTATION
#include "../STB_Image/stb_image.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void TetrisAlgorithm::Start()
{
	while (true)
	{
		m_PreviousBoardState = m_BoardState;

		/* Take a screenshot so we can analyze our NES emulator */
		TakeScreenshot();

		/* Get the current board state from the screenshot we took */
		GetBoardState();

		/* Calculate our next move */
		CalculateNextMove();
		
		++m_CurrentFrame;
	}
}

void TetrisAlgorithm::SendMousePress(const Point& coords) const
{
	INPUT input{};

	input.type = INPUT_MOUSE;
	Utils::ConvertNDCToScreenCoords(input.mi.dx, input.mi.dy, coords);
	input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

	SendInput(1, &input, sizeof(INPUT));
}

void TetrisAlgorithm::Initialize()
{
	HWND tetris{ FindWindowA(NULL, "NES - Tetris") };
	__ASSERT(tetris != nullptr);

	HWND virtualPads{ FindWindowA(NULL, "Virtual Pads") };
	__ASSERT(virtualPads != nullptr);

	SetForegroundWindow(tetris);
	SetForegroundWindow(virtualPads);

	Sleep(1000);
}

void TetrisAlgorithm::TakeScreenshot()
{
	if (m_ModuleName.empty())
	{
		/* Get file we're running from */
		char buffer[MAX_PATH]{};
		GetModuleFileNameA(NULL, buffer, MAX_PATH);

		/* Parse the path out of the .exe */
		const std::string path{ buffer };
		m_ModuleName = path.substr(0, path.find_last_of("\\"));
	}

	/* Run our Python screenshot code */
	system
	(
		((("cd ") + m_ModuleName) + " && " +
			"python main.py Screenshot").c_str()
	);
}

void TetrisAlgorithm::GetBoardState()
{
	Utils::ResetArray(m_BoardState);

	/* Analyze the image with STB */
	int width{};
	int height{};
	int bitsPerPixel{};
	const std::string path{ std::filesystem::absolute(std::filesystem::path{m_ModuleName + "\\Screenshot.png"}).string() };
	unsigned char* pData = stbi_load(path.c_str(), &width, &height, &bitsPerPixel, 0);

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

void TetrisAlgorithm::CalculateNextMove()
{
	if (m_CurrentFrame == 0 || m_CurrentFrame % 2 != 0)
		return;

	constexpr static uint8_t maxNrOfBlocks{ 4 };

	/* Get the current piece */
	uint8_t blockIndices[maxNrOfBlocks]{};
	uint8_t indexCounter{};

	for (uint8_t i{}; i < m_PreviousBoardState.size(); ++i)
	{
		if (m_PreviousBoardState[i] == m_BoardState[i])
			continue;

		blockIndices[indexCounter++] = i;
	}

	if (indexCounter != maxNrOfBlocks)
		return;

	uint8_t rowIndices[maxNrOfBlocks]{};
	uint8_t colIndices[maxNrOfBlocks]{};

	for (uint8_t i{}; i < maxNrOfBlocks; ++i)
	{
		rowIndices[i] = GetRowIndex(blockIndices[i]);
		colIndices[i] = GetColumnIndex(blockIndices[i]);
	}

	uint8_t nrOfEqualRowIndices{};
	uint8_t nrOfEqualColIndices{};

	for (uint8_t i{}; i < maxNrOfBlocks; ++i)
	{
		for (uint8_t j{ i + 1u }; j < maxNrOfBlocks; ++j)
		{
			if (rowIndices[i] == rowIndices[j])
				++nrOfEqualRowIndices;

			if (colIndices[i] == colIndices[j])
				++nrOfEqualColIndices;
		}
	}

	if (nrOfEqualRowIndices == maxNrOfBlocks && nrOfEqualColIndices == 0)
		m_CurrentPiece = PieceShape::I;
	else if (nrOfEqualRowIndices == 2 && nrOfEqualColIndices == 2)
		m_CurrentPiece = PieceShape::O;
	else if (nrOfEqualRowIndices == 3 && nrOfEqualColIndices == 1)
	{
		// J, L or T

	}
	else if (nrOfEqualRowIndices == 2 && nrOfEqualColIndices == 1)
	{
		// Z or S
	}
}

uint8_t TetrisAlgorithm::GetRowIndex(const uint8_t index) const
{
	return static_cast<uint8_t>(index / m_BoardSize.x);
}

uint8_t TetrisAlgorithm::GetColumnIndex(const uint8_t index) const
{
	return static_cast<uint8_t>(index % m_BoardSize.x);
}