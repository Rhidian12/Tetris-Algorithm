#pragma once

#include "../../GameInput/GameInput.h" // GameInput

#include <vector> // std::vector
#include <array>

struct _SDL_GameController;

namespace Integrian2D
{
	class Command;
	class Scene;

	class GameController final
	{
	public:
		static GameController* CreateGameController(const uint8_t index) noexcept;
		static void Cleanup(const uint8_t index) noexcept;
		~GameController();

		void Activate(Scene* const pScene, const uint8_t index) noexcept;
		void Deactivate(Scene* const pScene, const uint8_t index) noexcept;

		void AddCommand(const ControllerInput controllerInput, const State keyState, Command* const pCommand) noexcept;
		void ExecuteCommands(const uint8_t index) noexcept;

		bool IsPressed(const ControllerInput controllerInput, const uint8_t index) const noexcept;
		bool WasPressed(const State previousState) const noexcept;
		State GetKeystate(const ControllerInput controllerInput, const State previousState, const uint8_t index) const noexcept;
		double GetJoystickMovement(const ControllerInput axis, const uint8_t index) const noexcept;
		double GetTriggerMovement(const ControllerInput axis, const uint8_t index) const noexcept;
		const std::vector<CommandAndButton>& GetCommands() const noexcept;

		void RemoveCommand(Command* const pCommand) noexcept;
		void RemoveAllCommands() noexcept;

		void ResetInputs() noexcept;

	private:
		GameController(const uint8_t index);
		GameController(const GameController&) = delete;
		GameController(GameController&&) = delete;
		GameController& operator=(const GameController&) = delete;
		GameController& operator=(GameController&&) = delete;

		inline static constexpr double m_MaxJoystickValue{ 32767.0 }; // double instead of integer to prevent typecasting
		inline static constexpr uint8_t m_MaxAmountOfJoysticks{ 4 }; /* arbitrary value tbh */

		std::vector<CommandAndButton> m_Commands{};
		inline static std::array<_SDL_GameController*, m_MaxAmountOfJoysticks> m_pSDLGameControllers{};
		inline static std::array<GameController*, m_MaxAmountOfJoysticks> m_pInstances{};
	};
}

