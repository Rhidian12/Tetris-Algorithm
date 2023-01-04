#include "ScreenGrabber.h"

#include <filesystem> /* std::filesystem */

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void ScreenGrabber::Update(const uint64_t currentFrame)
{
	if (currentFrame == 0)
		return;
	
	TakeScreenshot();
}

std::string ScreenGrabber::GetPathToScreenshot() const
{
	return std::filesystem::absolute(std::filesystem::path{ m_ModuleName + "\\Screenshot.png" }).string();
}

void ScreenGrabber::TakeScreenshot()
{
	if (m_ModuleName.empty())
	{
		/* Get file we're running from */
		char buffer[MAX_PATH]{};
		GetModuleFileNameA(NULL, buffer, MAX_PATH);

		/* Parse the path out of the .exe */
		const std::string path{ buffer };
		m_ModuleName = path.substr(0, path.find_last_of("\\"));
	}

	/* Run our Python screenshot code */
	system
	(
		((("cd ") + m_ModuleName) + " && " +
			"python main.py Screenshot").c_str()
	);
}