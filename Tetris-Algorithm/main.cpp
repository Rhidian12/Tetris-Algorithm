#include "Utils/Utils.h"
#include "TetrisAlgorithm/TetrisAlgorithm.h"
#include "Board/Board.h"
#include "ScreenGrabber/ScreenGrabber.h"
#include "Timer/Timer.h"

#include <limits> /* std::numeric_limits */
#include <stdint.h> /* uint16_t, ... */
#include <string> /* std::string */

#ifdef max
#undef max
#endif

int main()
{
	/* NOTE: This does NOT start the emulator, or load the Tetris ROM, those must be started by the user! */
	/* Emulator download link: https://www.emulator-zone.com/misc/bizhawk */
	/* Rom download link: https://www.emulatorgames.net/roms/nintendo/tetris/ */
	 Utils::InitializeTetris();

	 Sleep(1000);

	ScreenGrabber grabber{};
	Board board{ &grabber };
	TetrisAlgorithm algorithm{ &board };

	//{
	//	Tetromino i{ TetrominoShape::I, Point{ 0,0 }, &board };
	//	board.Add(i.GetCurrentPosition());
	//	board.UpdatePrevious();

	//	Tetromino t{ TetrominoShape::T, Point{ 5,19 }, &board };

	//	algorithm.SetCurrentPiece(t);
	//	algorithm.CalculateBestMove();
	//	algorithm.ApplyBestMove();
	//	board.UpdatePrevious();

	//	algorithm.SetCurrentPiece(t);
	//	algorithm.CalculateBestMove();
	//}

	board.GetOnNewPieceSpawned().Bind([&algorithm]()->void
		{
			algorithm.OnNewPieceSpawned();
		});

	uint64_t currentFrame{};
	while (true)
	{
		if (GetKeyState(VK_ESCAPE) & 0x8000)
			break;

		Timer::GetInstance().Update();

		if (!algorithm.IsExecutingBestMove())
		{
			grabber.Update(currentFrame);
			board.Update(currentFrame);
		}

		algorithm.Update(currentFrame);

		++currentFrame;
	}

	return 0;
}