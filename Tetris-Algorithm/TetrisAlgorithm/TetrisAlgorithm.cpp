#include "TetrisAlgorithm.h"
#include "../Utils/Utils.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void TetrisAlgorithm::Start()
{
	/* Take a screenshot so we can analyze our NES emulator */
	TakeScreenshot();

	/* Analyze the image with STB */
}

void TetrisAlgorithm::SendMousePress(const Point& coords)
{
	INPUT input{};

	input.type = INPUT_MOUSE;
	Utils::ConvertNDCToScreenCoords(input.mi.dx, input.mi.dy, coords);
	input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

	SendInput(1, &input, sizeof(INPUT));
}

void TetrisAlgorithm::TakeScreenshot()
{
	/* [Cringe]: Cache the module name */

	/* Get file we're running from */
	char buffer[MAX_PATH]{};
	GetModuleFileNameA(NULL, buffer, MAX_PATH);

	/* Parse the path out of the .exe */
	std::string path{ buffer };
	path = path.substr(0, path.find_last_of("\\"));

	/* Run our Python screenshot code */
	/* [TODO]: Make this portable! I added the python .exe manually, add the python script and run that instead! */
	system
	(
		((("cd ") + path) + "&& " +
			"\"Python\\main.exe\" Screenshot").c_str()
	);
}