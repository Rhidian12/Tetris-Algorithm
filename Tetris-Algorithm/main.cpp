#include "Point/Point.h"
#include "Utils/Utils.h"

#include <limits> /* std::numeric_limits */
#include <stdint.h> /* uint16_t, ... */
#include <string> /* std::string */
#include <Windows.h> /* Windows stuff */

#ifdef max
#undef max
#endif

void InitializeTetris();
void GetDesktopResolution(LONG& x, LONG& y);
void ConvertNDCToScreenCoords(LONG& x, LONG& y, const Point& wantedCoords, const uint32_t screenX, const uint32_t screenY);
void SendMousePress(const Point& coords);

inline constexpr Point g_LeftCoords{ 84L, 252L };
inline constexpr Point g_DownCoords{ 111L, 306L };
inline constexpr Point g_UpCoords{ 111L, 239L };
inline constexpr Point g_RightCoords{ 138L, 252L };

inline constexpr const char g_TetrisWindow[13]{ "NES - Tetris" };
inline constexpr const char g_VirtualPadsWindow[13]{ "Virtual Pads" };

inline LONG g_ScreenWidth{};
inline LONG g_ScreenHeight{};

int main()
{
	/* NOTE: This does NOT start the emulator, or load the Tetris ROM, those must be started by the user! */
	/* Emulator download link: https://www.emulator-zone.com/misc/bizhawk */
	/* Rom download link: https://www.emulatorgames.net/roms/nintendo/tetris/ */
	InitializeTetris();

	Sleep(1000);

	return 0;
}

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

void SendMousePress(const Point& coords)
{
	INPUT input{};

	input.type = INPUT_MOUSE;
	ConvertNDCToScreenCoords(input.mi.dx, input.mi.dy, coords, g_ScreenWidth, g_ScreenHeight);
	input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

	SendInput(1, &input, sizeof(INPUT));
}