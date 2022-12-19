#pragma once

#include "../../Math/TypeDefines.h"
#include "../../Texture/Texture.h"
#include "../../Renderer/Renderer.h"

#include <string>

namespace Integrian2D
{
	namespace Printer
	{
		/* Standard layout of a TTF file */
		inline static const std::string possibleCharacters{ "abcdefghijklmnopqrstuvvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ,.?!\n" };
		inline static const size_t amountOfCharacters{ possibleCharacters.size() };

		inline Rectf CalculateSourceRect(const char c, const float charWidth, const float charHeight)
		{
			const size_t location{ possibleCharacters.find(c) };

			ASSERT(location != std::string::npos, "Printer::CalculateSourceRect() > The character: "_s + c + " is not supported to be rendered!");

			return Rectf{ charWidth * location,0.f,charWidth,charHeight };
		}

		inline void RenderString(Texture* const pFont, const Point2f& location, const float angle, const Point2f& scale, const std::string& textToRender) noexcept
		{
			Point2f currentPosition{ location };
			const float charWidth{ pFont->GetWidth() / amountOfCharacters };
			const float charHeight{ pFont->GetHeight() };

			for (size_t i{}; i < textToRender.size(); ++i)
			{
				if (i != 0)
					currentPosition.x += charWidth;

				if (textToRender[i] == '\n')
				{
					currentPosition.y -= charHeight;
					currentPosition.x = location.x - charWidth; /* next loop, + m_CharWidth offsets this */
				}

				Renderer::GetInstance()->RenderTexture(pFont,
					PRectf{ currentPosition,charWidth, charHeight, angle, scale },
					CalculateSourceRect(textToRender[i], charWidth, charHeight));
			}
		}
	}
}