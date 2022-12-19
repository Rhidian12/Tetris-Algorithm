#pragma once

#include "../Integrian2D_API.h"

#include "../EventQueue/EventQueue.h"

#include <vector>

namespace Integrian2D
{
	class GameObject;

	struct GameObjectInformation final
	{
		const char* pName;
		GameObject* pGameObject;
	};

	/* This class is supposed to be inherited from */
	class INTEGRIAN2D_API Scene
	{
	public:
		Scene(const char* pSceneName);
		virtual ~Scene();

		/* This function has to be overridden
		   This is where all of the Scene initisialition should start! */
		virtual void Start() = 0;

		/* An optionally overridable Scene-specific Update
		   This function should not try to update the GameObject's Update
		   This happens internally */
		virtual void Update() {}

		/* An optionally overridable Scene-specific FixedUpdate
		   This function should not try to update the GameObject's FixedUpdate
		   This happens internally */
		virtual void FixedUpdate() {}

		/* An optionally overridable Scene-specific LateUpdate
		   This function should not try to update the GameObject's LateUpdate
		   This happens internally */
		virtual void LateUpdate() {}

		/* An optionally overridable Scene-specific Render
		   This function should not try to update the GameObject's Render
		   This happens internally */
		virtual void Render() const {}

		/* An optionally overridable Scene-specific function
			This function gets called when the scene gets set as active */
		virtual void OnSceneEnter() noexcept {}

		/* An optionally overridable Scene-specific function
			This function gets called when the scene is active, and gets replaced by another scene as the active scene */
		virtual void OnSceneExit() noexcept {}

		/* Use this function to add GameObjects to the Scene!
		   GameObjects with duplicate GameObject names are not allowed, except if the shouldAlwaysAdd parameter is set to true
		   If this boolean is set to true and there is a duplicate GameObject name, the name will be the same, with (N) added to it
		   where N is the amount of times this name has been added - 1 */
		void AddGameObject(const char* pGameObjectName, GameObject* const pGameObject, const bool shouldAlwaysAdd = true) noexcept;

		/* Set the Scene's Name */
		void SetSceneName(const char* pSceneName) noexcept;

		/* Get a previously added GameObject by its name
		   This function will return a nullptr if the GameObject is not present */
		GameObject* const GetGameObject(const char* pGameObjectName) const noexcept;

		/* Get all previously added GameObjects to the Scene
		   This function will return an empty map if there have been no added GameObjects */
		const std::vector<GameObjectInformation>& GetGameObjects() const noexcept;

		/* Get the Scene's name */
		const char* GetSceneName() const noexcept;

		/* Is the Scene active? */
		bool IsSceneActive() const noexcept;

		/* Set whether the scene is active */
		void SetIsSceneActive(const bool isActive) noexcept;

	private:
		friend class Core; // Make sure that only Core can access the Root functions
		friend class SceneManager; /* The SceneManager needs access to the RootOnSceneEnter and RootOnSceneExit */

		struct SceneImpl;

		void RootStart();
		void RootUpdate();
		void RootFixedUpdate();
		void RootLateUpdate();
		void RootRender() const;
		void RootOnSceneEnter() noexcept;
		void RootOnSceneExit() noexcept;

		SceneImpl* m_pSceneImpl;
	};
}

