#include "TetrisAlgorithm.h"
#include "../Utils/Utils.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void TetrisAlgorithm::SendMousePress(const Point& coords)
{
	INPUT input{};

	input.type = INPUT_MOUSE;
	Utils::ConvertNDCToScreenCoords(input.mi.dx, input.mi.dy, coords);
	input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

	SendInput(1, &input, sizeof(INPUT));
}