#pragma once

#include "../Integrian2D_API.h"

#include <string>
#include <unordered_map>

namespace Integrian2D
{
	class Texture;

	class TextureManager final
	{
	public:
		/* Get a TextureManager instance */
		INTEGRIAN2D_API static TextureManager* GetInstance() noexcept;

		/* Internal Usage
		   Do NOT call this function manually */
		static void Cleanup() noexcept;

		INTEGRIAN2D_API ~TextureManager();

		/* Add a Texture to the Texture Manager
		   Textures with duplicate names do not get added */
		INTEGRIAN2D_API void AddTexture(const std::string& textureID, Texture* const pTexture) noexcept;

		/* Get a previously added Texture to the TextureManager 
		   If no Texture with identifier textureID has been added, this function will return a nullptr */
		INTEGRIAN2D_API Texture* const GetTexture(const std::string& textureID) const noexcept;

		/* Get all previously added Textures 
		   If no Textures have been added, this function returns an empty map */
		INTEGRIAN2D_API const std::unordered_map<std::string, Texture*>& GetTextures() const noexcept;

		TextureManager(const TextureManager&) = delete;
		TextureManager(TextureManager&&) = delete;
		TextureManager& operator=(const TextureManager&) = delete;
		TextureManager& operator=(TextureManager&&) = delete;

	private:
		TextureManager();

		std::unordered_map<std::string, Texture*> m_pTextures;

		inline static TextureManager* m_pInstance{};
	};
}