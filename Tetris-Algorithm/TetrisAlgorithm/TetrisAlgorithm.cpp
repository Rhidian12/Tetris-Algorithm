#include "TetrisAlgorithm.h"
#include "../Utils/Utils.h"

#include <iostream>
#include <filesystem>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h>

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
	char buffer[MAX_PATH]{};

	GetModuleFileNameA(NULL, buffer, MAX_PATH);

	std::string path{ buffer };
	path = path.substr(0, path.find_last_of("\\") + 1) + "screenshot";

	std::filesystem::directory_entry entry{ "../Python/main.exe" };
	auto absolutePath = std::filesystem::absolute(entry.path());

	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	//auto error = CreateProcessA(
	//	absolutePath.string().c_str(),
	//	path.data(),
	//	NULL,
	//	NULL,
	//	FALSE,
	//	0,
	//	NULL,
	//	NULL,
	//	&si,
	//	&pi);

	//if (error == 0)
	//	std::cout << GetLastError() << "\n";

	//CloseHandle(pi.hProcess);
	//CloseHandle(pi.hThread);

	//auto error = ShellExecuteA(NULL, "open", absolutePath.string().c_str(), path.data(), NULL, SW_SHOW);

	//if ((INT_PTR)error != 32)
	//	std::cout << GetLastErrorAsString() << "\n";
	
	std::string aaa{ "D:\\000 Documents\\000 Gradwork\\ScreenGrabber\\main.py" };
	std::string command{  };

	system("python \"D:\\000 Documents\\000 Gradwork\\ScreenGrabber\\main.py\" \"D:\\Github\\Tetris-Algorithm\\Tetris-Algorithm\\x64\\Debug\\Screenshot\"");
}

void TetrisAlgorithm::SendMousePress(const Point& coords)
{
	INPUT input{};

	input.type = INPUT_MOUSE;
	Utils::ConvertNDCToScreenCoords(input.mi.dx, input.mi.dy, coords);
	input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

	SendInput(1, &input, sizeof(INPUT));
}