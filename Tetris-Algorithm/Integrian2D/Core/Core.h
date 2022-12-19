#pragma once

#include "../Integrian2D_API.h"

#include <string>

namespace Integrian2D
{
	class Scene;
	class Window;

	class Core final
	{
	public:
		/* Get the earlier created Engine instance */
		INTEGRIAN2D_API static Core* const GetInstance() noexcept;

		/* Create an instance of the engine, this initializes the libraries used by the engine and creates a window
		   Call this function only ONCE */
		INTEGRIAN2D_API static Core* const CreateEngine(const int windowWidth, const int windowHeight, const std::string& windowTitle) noexcept;

		/* Delete the earlier created Engine instance */
		INTEGRIAN2D_API static void Cleanup() noexcept;

		INTEGRIAN2D_API ~Core();

		/* This function starts the Engine and should be called ONCE */
		INTEGRIAN2D_API void Run();

		/* Is the Engine running? */
		INTEGRIAN2D_API bool IsEngineRunning() const noexcept;

		/* Get the window width */
		INTEGRIAN2D_API int GetWindowWidth() const noexcept;

		/* Get the window height */
		INTEGRIAN2D_API int GetWindowHeight() const noexcept;

		Core(const Core&) = delete;
		Core(Core&&) = delete;
		Core& operator=(const Core&) = delete;
		Core& operator=(Core&&) = delete;

	private:
		Core(const int windowWidth, const int windowHeight, const std::string& windowTitle);
		void InitializeLibraries(const int windowWidth, const int windowHeight, const std::string& windowTitle) noexcept;
		void ShutdownLibraries() noexcept;

		bool m_IsEngineRunning;

		Window* m_pWindow;
		int m_WindowWidth;
		int m_WindowHeight;

		inline static Core* m_pInstance{};
	};
}
