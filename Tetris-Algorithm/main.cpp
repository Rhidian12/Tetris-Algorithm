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
	//	Tetromino s{ TetrominoShape::S, Point{ 4,4 }, &board };
	//	board.Add(s.GetCurrentPosition());
	//	board.UpdatePrevious();
	//	board.Remove(s.GetCurrentPosition());

	//	s.Rotate(Tetromino::Rotation::CW);
	//	board.Add(s.GetCurrentPosition());
	//	board.DebugBoardState();

	//	board.DebugBoardState();
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
			if (currentFrame % algorithm.GetSpeed() == 0)
			{
				grabber.Update(currentFrame);
				board.Update(currentFrame);
			}
		}

		algorithm.Update();

		++currentFrame;
	}

	return 0;
}