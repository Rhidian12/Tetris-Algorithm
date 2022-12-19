#pragma once

#include "../Integrian2D_API.h"
#include "PossibleInputs.h"
#include "../Utils/Utils.h"

namespace Integrian2D
{
	enum class State
	{
		OnHeld = 1,
		OnRelease = 2,
		OnPress = 3,

		NotPressed = 0 // internal usage
	};

	class Command;

	/* Used by the InputManager to represent user input, either controller, mouse or keyboard input */
	struct INTEGRIAN2D_API GameInput final
	{
		explicit GameInput() = default;
		explicit GameInput(const ControllerInput controllerInput);
		explicit GameInput(const KeyboardInput keyboardInput);
		explicit GameInput(const MouseButton mouseButton);

		ControllerInput controllerInput{ ControllerInput::INVALID };
		KeyboardInput keyboardInput{ KeyboardInput::INVALID };
		MouseButton mouseButton{ MouseButton::INVALID };

		/* Internal Usage, TODO: Make sure the ID is never the same */
		uint64_t id{ Utils::RandomNumber(uint64_t(0), std::numeric_limits<uint64_t>::max()) };
	};

	/* Interal Usage */
	struct CommandAndButton final
	{
		CommandAndButton(Command* const pCommand, const State wantedKeystate, const GameInput gameInput, const bool _isActive = true);

		Command* pCommand;
		State wantedKeystate;
		State previousKeystate;
		GameInput gameInput;
		bool isActive;
	};

	inline bool operator==(const GameInput& a, const GameInput& b) noexcept
	{
		return a.id == b.id;
	}

	inline bool operator!=(const GameInput& a, const GameInput& b) noexcept
	{
		return a.id != b.id;
	}
}
