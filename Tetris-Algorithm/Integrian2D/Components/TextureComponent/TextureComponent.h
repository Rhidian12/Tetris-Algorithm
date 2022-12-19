#pragma once

#include "../../Integrian2D_API.h"
#include "../Component/Component.h"
#include "../../Math/TypeDefines.h"

namespace Integrian2D
{
	class Texture;

	class INTEGRIAN2D_API TextureComponent final : public Component
	{
	public:
		TextureComponent(GameObject* pOwner);
		TextureComponent(GameObject* pOwner, Texture* const pTexture);

		/*
		The destination rect is set to the respective parameter.
		*/
		TextureComponent(GameObject* pOwner, Texture* const pTexture, const Rectf& destRect);

		/* This function should NOT get called manually. If this component is added to a GameObject,
		the GameObject in question will call this function internally */
		virtual void Render() const override;

		/* See Component::Clone() for documentation */
		virtual Component* Clone(GameObject* pOwner) noexcept override;

		/* Set the Texture to be rendered */
		void SetTexture(Texture* const pTexture) noexcept;

		/* Get the current Texture being rendered */
		Texture* const GetTexture() const noexcept;

		/* The Source Rect is the area of the Texture that gets rendered
		   By default this is the entire Texture, but this can be changed to whatever the User sees fit */
		Rectf m_SourceRect;

		/*
			The destination rect (the area where the Texture gets rendered) is set to its default,
			which is the GameObject's position, Texture's width and Texture's height */
		Rectf m_DestRect;

	private:
		Texture* m_pTexture;
	};
}