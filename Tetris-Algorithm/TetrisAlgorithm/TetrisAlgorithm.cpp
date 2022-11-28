#include "TetrisAlgorithm.h"
#include "../Utils/Utils.h"

#include <iostream>
#include <filesystem>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

std::string GetLastErrorAsString()
{
	//Get the error message ID, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0) {
		return std::string(); //No error message has been recorded
	}

	LPSTR messageBuffer = nullptr;

	//Ask Win32 to give us the string version of that message ID.
	//The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	//Copy the error message into a std::string.
	std::string message(messageBuffer, size);

	//Free the Win32's string's buffer.
	LocalFree(messageBuffer);

	return message;
}

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