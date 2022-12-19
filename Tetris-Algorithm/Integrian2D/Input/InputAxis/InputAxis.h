#pragma once

#include "../../Integrian2D_API.h"
#include "../../GameInput/GameInput.h"

#include <string>

namespace Integrian2D
{
	struct INTEGRIAN2D_API InputAxis final
	{
		const char* axis{};
		GameInput positiveInput{ KeyboardInput::INVALID };
		GameInput negativeInput{ KeyboardInput::INVALID };
	};

	INTEGRIAN2D_API inline bool operator==(const InputAxis& a, const InputAxis& b) noexcept
	{
		return a.axis == b.axis && a.positiveInput == b.positiveInput && a.negativeInput == b.negativeInput;
	}

	INTEGRIAN2D_API inline bool operator!=(const InputAxis& a, const InputAxis& b) noexcept
	{
		return !(a == b);
	}
}