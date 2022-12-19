#pragma once
// ===========================
// Bart Uyttenhove, professor at DAE created this class
// I took it, with permission, and changed it to suit this engine
// ===========================

#include "../Integrian2D_API.h"

#include "../Math/TypeDefines.h"

#include <string> // std::string

struct SDL_Surface;
struct _TTF_Font;

namespace Integrian2D
{
	class Texture final
	{
	public:
		INTEGRIAN2D_API explicit Texture(const std::string& imagePath);
		INTEGRIAN2D_API explicit Texture(const std::string& text, const std::string& fontPath, const int ptSize, const RGBColour& textColor);
		INTEGRIAN2D_API ~Texture();

		/* Returns the Texture width */
		INTEGRIAN2D_API float GetWidth() const noexcept;

		/* Returns the Texture height */
		INTEGRIAN2D_API float GetHeight() const noexcept;

		/* Returns whether the creation of the Texture succeeded */
		INTEGRIAN2D_API bool IsCreationOk() const noexcept;

		/* Get the ID assigned to the Texture */
		INTEGRIAN2D_API unsigned int GetTextureID() const noexcept;

		INTEGRIAN2D_API Texture(const Texture& other) noexcept;
		INTEGRIAN2D_API Texture(Texture&& other) noexcept;
		INTEGRIAN2D_API Texture& operator=(const Texture& other) noexcept;
		INTEGRIAN2D_API Texture& operator=(Texture&& other) noexcept;

	private:
		//DATA MEMBERS
		unsigned int m_Id;
		float m_Width;
		float m_Height;
		bool m_CreationOk;

		// FUNCTIONS
		void CreateFromImage(const std::string& path) noexcept;

		/* Function copies RGBColour by value because it calls MaxToOne */
		void CreateFromString(const std::string& text, _TTF_Font* const pFont, RGBColour textColor) noexcept;
		void CreateFromString(const std::string& text, const std::string& fontPath, int ptSize, const RGBColour& textColor) noexcept;
		void CreateFromSurface(SDL_Surface* const pSurface) noexcept;
		void DrawFilledRect(const Rectf& dstRect) const noexcept;
	};
}