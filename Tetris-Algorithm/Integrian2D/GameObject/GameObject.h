#pragma once

#include "../Integrian2D_API.h"
#include "../Math/TypeDefines.h"
#include "../Utils/Utils.h"

#include <vector>
#include <string>
#include <typeinfo>
#include <algorithm>

namespace Integrian2D
{
	class Component;
	class TransformComponent;

	/* The GameObject is the class that holds a list of Components, and calls their overridable functions, such as Render()
	   The GameObjects get added to the Scene via Scene::AddGameObject */
	class GameObject final
	{
	public:
		/* Every GameObject has a TransformComponent by default! */
		INTEGRIAN2D_API GameObject();
		INTEGRIAN2D_API ~GameObject();

		/* This function calls the GameObject's Component's Start(). Do NOT call this manually */
		INTEGRIAN2D_API void Start();

		/* This function calls the GameObject's Components' Update(). Do NOT call this manually */
		INTEGRIAN2D_API void Update();

		/* This function calls the GameObject's Components' FixedUpdate(). Do NOT call this manually */
		INTEGRIAN2D_API void FixedUpdate();

		/* This function calls the GameObject's Components' LateUpdate(). Do NOT call this manually */
		INTEGRIAN2D_API void LateUpdate();

		/* This function calls the GameObject's Components' Render(). Do NOT call this manually */
		INTEGRIAN2D_API void Render() const;

		/* This function returns the Component requested by the User. This function will cause a compilation error
		   if the requested Type is class not derived from Component
		   If the requested Type is present, it will return a pointer to the requested component
		   Otherwise this function returns a nullptr */
		template<typename Type, typename = std::enable_if_t<std::is_base_of_v<Component, Type>>>
		Type* const GetComponentByType() const noexcept;

		/* This function returns all of the Components requested by the user */
		template<typename Type, typename = std::enable_if_t<std::is_base_of_v<Component, Type>>>
		std::vector<Type*> GetAllComponentsByType() const noexcept;

		/* Adds a Component to the GameObject, if it not already present
		   Duplicate Component Types are allowed, but not the same Component twice */
		INTEGRIAN2D_API void AddComponent(Component* const pComponent) noexcept;

		/* Removes a component from the GameObject
			This function will remove ALL Components of the provided type
			This function does NOT destroy Components! */
		template<typename Type, typename = std::enable_if_t<std::is_base_of_v<Component, Type>>>
		void RemoveAllComponentsByType(std::vector<Component*>* pComponents) noexcept;

		/* Removes a component from the GameObject
			This function will remove the specific component
			This function does NOT destroy Components! */
		INTEGRIAN2D_API void RemoveComponentByValue(Component* const pComponent) noexcept;

		/* Removes AND deletes a component from the GameObject 
			This function will delete ALL Components of the provided type */
		template<typename Type, typename = std::enable_if_t<std::is_base_of_v<Component, Type>>>
		void DeleteAllComponentsByType() noexcept;

		/* Removes AND deletes a component from the GameObject */
		INTEGRIAN2D_API void DeleteComponentByValue(Component* const pComponent) noexcept;

		/* Sets another GameObject as this GameObject's Child
		   The GameObject set as Child its parent is set as this GameObject */
		INTEGRIAN2D_API void AddChild(GameObject* const pChild) noexcept;

		/* Set a GameObject's parent
		   It is unnecessary to call this in conjunction with AddChild()
		   Since AddChild() sets the parent automatically */
		INTEGRIAN2D_API void SetParent(GameObject* const pParent) noexcept;

		/* Set the GameObject's tag */
		INTEGRIAN2D_API void SetTag(const std::string& tag) noexcept;

		/* Get this GameObject's parent
		   If the GameObject has no parent, this function returns a nullptr */
		INTEGRIAN2D_API GameObject* const GetParent() const noexcept;

		/* Get the GameObject's Children
		   If the GameObject has no Children, this functions returns an empty std::vector */
		INTEGRIAN2D_API const std::vector<GameObject*>& GetChildren() const noexcept;

		/* Get the GameObject's tag */
		INTEGRIAN2D_API const std::string& GetTag() const noexcept;

		/* Get all components attached to this GameObject */
		INTEGRIAN2D_API const std::vector<Component*>& GetComponents() const noexcept;

		/* The GameObject's Transform Component */
		TransformComponent* pTransform;

		INTEGRIAN2D_API GameObject(const GameObject& other) noexcept;
		INTEGRIAN2D_API GameObject(GameObject&& other) noexcept;
		INTEGRIAN2D_API GameObject& operator=(const GameObject& other) noexcept;
		INTEGRIAN2D_API GameObject& operator=(GameObject&& other) noexcept;

	private:
		std::vector<Component*> m_pComponents;
		std::vector<GameObject*> m_pChildren;
		GameObject* m_pParent;

		std::string m_Tag;
	};

	template<typename Type, typename>
	Type* const GameObject::GetComponentByType() const noexcept
	{
		const std::type_info& typeInfo{ typeid(Type) };

		for (Component* pC : m_pComponents)
			if (typeid(*pC) == typeInfo)
				return static_cast<Type*>(pC);

		return nullptr;
	}

	template<typename Type, typename>
	std::vector<Type*> GameObject::GetAllComponentsByType() const noexcept
	{
		std::vector<Type*> components{};
		const std::type_info& typeInfo{ typeid(Type) };

		for (Component* pC : m_pComponents)
			if (typeid(*pC) == typeInfo)
				components.push_back(static_cast<Type*>(pC));

		return components;
	}

	template<typename Type, typename>
	void GameObject::RemoveAllComponentsByType(std::vector<Component*>* pComponents) noexcept
	{
		const std::type_info& typeInfo{ typeid(Type) };

		if (pComponents)
		{
			m_pComponents.erase(std::remove_if(m_pComponents.begin(), m_pComponents.end(), [&typeInfo, &pComponents](Component* const pC)->bool
				{
					if (typeid(*pC) == typeInfo)
					{
						(*pComponents).push_back(pC);
						return true;
					}
					else
						return false;
				}), m_pComponents.end());
		}
		else
		{
			m_pComponents.erase(std::remove_if(m_pComponents.begin(), m_pComponents.end(), [&typeInfo](Component* const pC)->bool
				{
					return typeid(*pC) == typeInfo;
				}), m_pComponents.end());
		}
	}

	template<typename Type, typename>
	void GameObject::DeleteAllComponentsByType() noexcept
	{
		const std::type_info& typeInfo{ typeid(Type) };

		for (Component*& pC : m_pComponents)
			if (typeid(*pC) == typeInfo)
				Utils::SafeDelete(pC);

		m_pComponents.erase(std::remove(m_pComponents.begin(), m_pComponents.end(), nullptr), m_pComponents.end());
	}
}

