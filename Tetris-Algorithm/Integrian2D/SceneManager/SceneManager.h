#pragma once

#include "../Integrian2D_API.h"

#include <unordered_map>

namespace Integrian2D
{
	class Scene;

	class SceneManager final
	{
	public:
		/* Get a SceneManager instance */
		INTEGRIAN2D_API static SceneManager* const GetInstance() noexcept;

		/* Interal Usage
		   Do NOT call this manually */
		static void Cleanup() noexcept;

		INTEGRIAN2D_API ~SceneManager();

		/* Adds a Scene to the SceneManager 
		   Duplicate Scenes do not get added 
		   The first scene to be added gets automatically set as the active scene */
		INTEGRIAN2D_API void AddScene(Scene* const pScene) noexcept;

		/* Sets a previously added Scene as the active scene 
		   If the Scene with sceneName has not been added to the SceneManager,
		   then this function will not replace the current active scene */
		INTEGRIAN2D_API void SetActiveScene(const std::string& sceneName) noexcept;

		/* Sets all scene to inactive */
		INTEGRIAN2D_API void DeactivateAllScenes() noexcept;
		
		/* Get the currently active scene 
		   If no scene is marked as the active scene, this will return a nullptr */
		INTEGRIAN2D_API Scene* const GetActiveScene() const noexcept;

		/* Get the Scene with the corresponding Scene Name */
		INTEGRIAN2D_API Scene* const GetScene(const std::string& sceneName) const noexcept;

		/* Get all added Scenes 
		   If no Scenes have been added, this will return an empty map */
		INTEGRIAN2D_API const std::unordered_map<std::string, Scene*>& GetScenes() const noexcept;

	private:
		SceneManager();

		inline static SceneManager* m_pInstance{};

		Scene* m_pActiveScene;
		std::unordered_map<std::string, Scene*> m_pScenes;
	};
}
