#include "Point/Point.h"
#include "Utils/Utils.h"

#include <limits> /* std::numeric_limits */
#include <stdint.h> /* uint16_t, ... */
#include <string> /* std::string */
#include <Windows.h> /* Windows stuff */

#ifdef max
#undef max
#endif

void GetDesktopResolution(LONG& x, LONG& y)
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

void ConvertNDCToScreenCoords(LONG& x, LONG& y, const Point& wantedCoords, const uint32_t screenX, const uint32_t screenY)
{
	x = static_cast<LONG>(std::numeric_limits<uint16_t>::max() / (float)screenX * (float)wantedCoords.x);
	y = static_cast<LONG>(std::numeric_limits<uint16_t>::max() / (float)screenY * (float)wantedCoords.y);
}

inline constexpr Point g_LeftCoords{ 84L, 252L };
inline constexpr Point g_DownCoords{ 111L, 306L };
inline constexpr Point g_UpCoords{ 111L, 239L };
inline constexpr Point g_RightCoords{ 138L, 252L };

inline constexpr const char g_TetrisWindow[13]{ "NES - Tetris" };
inline constexpr const char g_VirtualPadsWindow[13]{ "Virtual Pads" };

int main()
{
	SetProcessDPIAware();

	LONG x{};
	LONG y{};
	GetDesktopResolution(x, y);

	HWND tetris{ FindWindowA(NULL, g_TetrisWindow) };
	__ASSERT(tetris != nullptr);

	HWND tetrisVirtualPad{ FindWindowA(NULL, g_VirtualPadsWindow) };
	__ASSERT(tetrisVirtualPad != nullptr);

	SetForegroundWindow(tetris);
	Sleep(1000);
	SetForegroundWindow(tetrisVirtualPad);
	Sleep(1000);

	INPUT input{};

	input.type = INPUT_MOUSE;
	ConvertNDCToScreenCoords(input.mi.dx, input.mi.dy, g_LeftCoords, x, y);
	input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

	SendInput(1, &input, sizeof(INPUT));

	return 0;
}