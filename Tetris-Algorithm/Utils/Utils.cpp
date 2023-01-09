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

	void ConvertNDCToScreenCoords(LONG& x, LONG& y, const volatile Tetris::Point& wantedCoords)
	{
		x = static_cast<LONG>(std::numeric_limits<uint16_t>::max() / (float)g_ScreenWidth * (float)wantedCoords.x);
		y = static_cast<LONG>(std::numeric_limits<uint16_t>::max() / (float)g_ScreenHeight * (float)wantedCoords.y);
	}

	void TakeScreenshot(HDC& hdc, HDC& hDest, HBITMAP& hbDesktop)
	{
		/* reference: https://stackoverflow.com/questions/5069104/fastest-method-of-screen-capturing-on-windows */

		hdc = GetDC(NULL); // get the desktop device context
		hDest = CreateCompatibleDC(hdc); // create a device context to use yourself

		// get the height and width of the screen
		int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);

		// create a bitmap
		hbDesktop = CreateCompatibleBitmap(hdc, width, height);

		// use the previously created device context with the bitmap
		SelectObject(hDest, hbDesktop);

		// copy from the desktop device context to the bitmap device context
		// call this once per 'frame'
		BitBlt(hDest, 0, 0, width, height, hdc, 0, 0, SRCCOPY);
	}

	void CleanupBitMap(HDC& hdc, HDC& hDest, HBITMAP& hbDesktop)
	{
		// after the recording is done, release the desktop context you got..
		ReleaseDC(NULL, hdc);

		// ..delete the bitmap you were using to capture frames..
		DeleteObject(hbDesktop);

		// ..and delete the context you created
		DeleteDC(hDest);
	}
}