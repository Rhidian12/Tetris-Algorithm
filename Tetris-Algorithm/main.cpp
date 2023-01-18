#include "Utils/Utils.h"
#include "TetrisAlgorithm/TetrisAlgorithm.h"
#include "GreedyAlgorithm/GreedyAlgorithm.h"
#include "FutureGreedyAlgorithm/FutureGreedyAlgorithm.h"
#include "Board/Board.h"
#include "ScreenGrabber/ScreenGrabber.h"
#include "Timer/Timer.h"

#include <limits> /* std::numeric_limits */
#include <stdint.h> /* uint16_t, ... */
#include <string> /* std::string */
#include <fstream> /* std::fstream */

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
	TetrisAlgorithm* pAlgorithm{ new GreedyAlgorithm{ &board } };
	// TetrisAlgorithm* pAlgorithm{ new FutureGreedyAlgorithm{ &board } };

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

	board.GetOnNewPieceSpawned().Bind([pAlgorithm]()->void
		{
			pAlgorithm->OnNewPieceSpawned();
		});

	while (true)
	{
		if (GetKeyState(VK_ESCAPE) & 0x8000)
			break;

		Timer::GetInstance().Update();

		const double currentTime{ Timer::Now().Count<TimeLength::MilliSeconds>() };

		if (!pAlgorithm->IsExecutingBestMove())
		{
			// grabber.Update();
			board.Update(pAlgorithm->NeedsNextPiece());
		}

		pAlgorithm->Update();

		const auto elapsedTime =
			currentTime +
			Timer::GetInstance().GetFixedElapsedTime<TimeLength::MilliSeconds>() -
			Timer::Now().Count<TimeLength::MilliSeconds>();

		if (elapsedTime > 0)
			Sleep(static_cast<DWORD>(Timer::GetInstance().GetTimePerFrame() * SecToMilli - elapsedTime + 1));
	}

	delete pAlgorithm;

	return 0;
}