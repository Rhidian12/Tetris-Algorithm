#pragma once

#include "../../Integrian2D_API.h"
#include "../Component/Component.h"

namespace Integrian2D
{
	class TextComponent;

	/* A Component that renders the current FPS to the screen */
	class FPSComponent final : public Component
	{
	public:
		/* The FPSComponent does NOT take ownership of the passed TextComponent
			Both components should be added to the same GameObject */
		INTEGRIAN2D_API FPSComponent(GameObject* const pOwner, TextComponent* const pTextComponent);

		INTEGRIAN2D_API virtual Component* Clone(GameObject* pOwner) noexcept override;

		INTEGRIAN2D_API virtual void Update() override;

		/* Get the provided text component */
		INTEGRIAN2D_API TextComponent* const GetTextComponent() const noexcept;

	private:
		TextComponent* m_pTextComponent;
	};
}