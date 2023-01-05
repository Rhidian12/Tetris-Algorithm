#pragma once

#include <string> /* std::string */

class ScreenGrabber final
{
public:
	void Update();

	std::string GetPathToScreenshot() const;

private:
	void TakeScreenshot();

	std::string m_ModuleName;
};