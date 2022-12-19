#pragma once

#include "../../Integrian2D_API.h"
#include "../Component/Component.h"
#include "../../Math/TypeDefines.h"

#include <string>

namespace Integrian2D
{
	class Texture;

	/* A Component to render text to the screen */
	class TextComponent final : public Component
	{
	public:
		/* If no Font is passed to the TextComponent, the default fonts gets used */
		INTEGRIAN2D_API TextComponent(GameObject* const pParent, const std::string& text);
		INTEGRIAN2D_API TextComponent(GameObject* const pParent, const std::string& text, const int textSize);
		INTEGRIAN2D_API TextComponent(GameObject* const pParent, const std::string& text, const int textSize, const RGBColour& colour);
		INTEGRIAN2D_API TextComponent(GameObject* const pParent, const std::string& text, const int textSize, Texture* const pFont);
		INTEGRIAN2D_API TextComponent(GameObject* const pParent, const std::string& text, const int textSize, Texture* const pFont, const RGBColour& colour);

		INTEGRIAN2D_API virtual Component* Clone(GameObject* pParent) noexcept override;

		INTEGRIAN2D_API virtual void Render() const override;

		/* Set the text to be rendered to the screen */
		INTEGRIAN2D_API void SetTextToRender(const std::string& text) noexcept;

		/* Set the size of the text to be rendered 
			Note that this might require a new texture for the text to be initialized */
		INTEGRIAN2D_API void SetTextSize(const int textSize) noexcept;

		/* Set the colour of the text to be rendered
			Note that this might require a new texture for the text to be initialized */
		INTEGRIAN2D_API void SetTextColour(const RGBColour& textColour) noexcept;

		/* Set the font of the text to be rendered
			Note that this might require a new texture for the text to be initialized */
		INTEGRIAN2D_API void SetTextFont(Texture* const pFont) noexcept;

		/* Get the text that is being rendered to the screen */
		INTEGRIAN2D_API const std::string& GetTextToRender() const noexcept;

		/* Get the width of the text being rendered to the screen */
		INTEGRIAN2D_API float GetTextWidth() const noexcept;

		/* Get the height of the text being rendered to the screen */
		INTEGRIAN2D_API float GetTextHeight() const noexcept;

	private:
		std::string m_TextToRender;
		int m_TextSize;
		RGBColour m_TextColour;
		Texture* m_pFont;
	};
}