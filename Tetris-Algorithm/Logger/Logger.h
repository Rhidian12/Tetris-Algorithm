#pragma once

#include <string> /* std::string */
#include <string_view> /* std::string_view */
#include <memory> /* std::unique_ptr */

namespace Debug
{
#ifdef _WIN32
	enum class MessageColour : uint8_t
	{
		Black = 0,
		IntenseBlue = 1,
		Green = 2,
		LightBlue = 3,
		Red = 4,
		Purple = 5,
		Yellow = 6,
		White = 7,
		Grey = 8,
		Blue = 9,
		LightGreen = 10,
		VeryLightBlue = 11,
		LightRed = 12,
		LightPurple = 13,
		VeryLightYellow = 14,
		IntenseWhite = 15
	};
#endif

	class Logger final
	{
	public:
		static Logger& GetInstance();

		void LogMessage(
			const std::string_view message,
			[[maybe_unused]] const int lineNumber,
			[[maybe_unused]] const std::string_view file,
			const bool bVerbose = false
		);

		void LogWarning(
			const std::string_view message,
			[[maybe_unused]] const int lineNumber,
			[[maybe_unused]] const std::string_view file,
			const bool bVerbose = false
		);

		void LogError(
			const std::string_view message,
			[[maybe_unused]] const int lineNumber,
			[[maybe_unused]] const std::string_view file,
			const bool bVerbose = false
		);

		void LogCustomMessage(
			const std::string_view message,
			[[maybe_unused]] const int lineNumber,
			[[maybe_unused]] const std::string_view file,
			const MessageColour colour,
			const bool bVerbose = false
		);

		void LogAssertion(
			const std::string_view message,
			[[maybe_unused]] const int lineNumber,
			[[maybe_unused]] const std::string_view file,
			const MessageColour colour,
			const bool bVerbose = false
		);

	private:
		Logger();

		inline static std::unique_ptr<Logger> m_pInstance{};

		/* typedef void* HANDLE, ergo void* == HANDLE */
		void* ConsoleHandle;
	};
}