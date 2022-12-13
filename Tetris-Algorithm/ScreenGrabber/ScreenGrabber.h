#pragma once

#include <string> /* std::string */
#include <string_view> /* std::string_view */

class ScreenGrabber final
{
public:
	void Update(const uint64_t currentFrame);

	std::string_view GetPathToScreenshot() const;

private:
	void TakeScreenshot();

	std::string m_ModuleName;
};