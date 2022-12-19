#pragma once

#include "../../Integrian2D_API.h"
#include "../../Math/TypeDefines.h"

namespace Integrian2D
{
	class GameObject;

	class INTEGRIAN2D_API Component
	{
	public:
		Component(GameObject* pOwner);
		virtual ~Component() = default;

		Component(const Component& other) noexcept;
		Component(Component&& other) noexcept;
		Component& operator=(const Component& other) noexcept;
		Component& operator=(Component&& other) noexcept;

		/* This function must return a Component * andmust be overriden in any derived component
		   This function gets used interally whenever a Component gets copied by its owner */
		virtual Component* Clone(GameObject* pOwner) noexcept = 0;
		/* 
			Example from the TextureComponent:
	
			Component* TextureComponent::Clone(GameObject* pOwner) noexcept
			{
				return new TextureComponent{ pOwner, m_pTexture };
			}
		*/

		/* Set the new owner of this component */
		void SetOwner(GameObject* const pOwner) noexcept;

		/* Get the current owner of this component */
		GameObject* const GetOwner() const noexcept;

		/* This function gets called once right before the first frame update */
		virtual void Start() {}

		/* All of the functions below can be overriden if desired 
		   On their own, they do nothing
		   Do NOT call these functions manually, once this component gets added to a GameObject,
		   these functions get called automatically */
		virtual void Update() {}
		virtual void FixedUpdate() {}
		virtual void LateUpdate() {}

		virtual void Render() const {}

	protected:
		GameObject* m_pOwner;

	private:
		friend class GameObject;

		virtual void RootStart() noexcept {};
	};
}
