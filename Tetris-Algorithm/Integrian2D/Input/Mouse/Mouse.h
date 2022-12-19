#pragma once

#include "../../GameInput/GameInput.h" // GameInput
#include <vector> // std::vector

namespace Integrian2D
{
	class Scene;

	class Mouse final
	{
	public:
		static Mouse* const CreateMouse() noexcept;
		static void Cleanup() noexcept;
		~Mouse();

		void Activate(Scene* const pScene) noexcept;
		void Deactivate(Scene* const pScene) noexcept;

		void AddCommand(const MouseButton mouseButton, const State keyState, Command* const pCommand) noexcept;
		void ExecuteCommands() noexcept;

		bool IsPressed(const MouseButton gameInput) const noexcept;
		bool WasPressed(const State previousState) const noexcept;
		State GetKeystate(const MouseButton mouseButton, const State previousState) const noexcept;
		const std::vector<CommandAndButton>& GetCommands() const noexcept;

		void RemoveCommand(Command* const pCommand) noexcept;
		void RemoveAllCommands() noexcept;

		void ResetInputs() noexcept;

	private:
		Mouse() = default;
		Mouse(const Mouse&) = delete;
		Mouse(Mouse&& other) = delete;
		Mouse& operator=(const Mouse&) = delete;
		Mouse& operator=(Mouse&& other) = delete;

		inline static Mouse* m_pInstance{};

		bool m_IsActive{ true };
		std::vector<CommandAndButton> m_MouseCommands{};
	};
}
