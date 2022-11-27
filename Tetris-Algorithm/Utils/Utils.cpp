#include "Utils.h"

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

	void ConvertNDCToScreenCoords(LONG& x, LONG& y, const Point& wantedCoords)
	{
		x = static_cast<LONG>(std::numeric_limits<uint16_t>::max() / (float)g_ScreenWidth * (float)wantedCoords.x);
		y = static_cast<LONG>(std::numeric_limits<uint16_t>::max() / (float)g_ScreenHeight * (float)wantedCoords.y);
	}
}