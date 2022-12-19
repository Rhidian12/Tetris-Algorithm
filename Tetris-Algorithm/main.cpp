#include "Utils/Utils.h"
#include "../TetrisScene/TetrisScene.h"

#include <Core/Core.h>
#include <SceneManager/SceneManager.h>

#include <limits> /* std::numeric_limits */
#include <stdint.h> /* uint16_t, ... */
#include <string> /* std::string */

#ifdef max
#undef max
#endif

int main()
{
	using namespace Integrian2D;

	Core* const pEngine{ Core::CreateEngine(g_ScreenWidth,g_ScreenHeight,"Tetris") };
	SceneManager* const pSceneManager{ SceneManager::GetInstance() };

	pSceneManager->AddScene(new TetrisScene{ "Tetris" });

	pEngine->Run();

	//uint64_t currentFrame{};
	//while (true)
	//{
	//	if (GetKeyState(VK_ESCAPE) & 0x8000)
	//		break;

	//	if (!algorithm.IsExecutingBestMove())
	//	{
	//		grabber.Update(currentFrame);
	//		board.Update(currentFrame);
	//	}

	//	algorithm.Update(currentFrame);

	//	++currentFrame;
	//}

	return 0;
}