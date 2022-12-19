#pragma once

#include "../../Integrian2D_API.h"
#include "../Component/Component.h"
#include "../../Math/TypeDefines.h"

namespace Integrian2D
{
	class TransformManager;

	class INTEGRIAN2D_API TransformComponent final : public Component
	{
	public:
		TransformComponent(GameObject* pOwner);

		/* See Component::Clone() for documentation */
		virtual Component* Clone(GameObject* pOwner) noexcept override;

		virtual void FixedUpdate() override;

		/* Return the GameObject's position in World Space */
		const Point2f& GetWorldPosition() const noexcept;

		/* Return the GameObject's position in its Local Space */
		const Point2f GetLocalPosition() const noexcept;

		/* Return the GameObject's scale in its World Space */
		const Point2f& GetWorldScale() const noexcept;

		/* Return the GameObject's angle in its World Space */
		const float GetWorldAngle() const noexcept;

		/* Translates the GameObject's position by velocity multiplied by Delta Time. This does not trigger a matrix recalculation */
		void Translate(const Vector2f& velocity) noexcept;

		/* Rotates the GameObject by angleRadians. This triggers a matrix recalculation */
		void Rotate(const float angleRadians) noexcept;

		/* Scales the GameObject by scale. This triggers a matrix recalculation */
		void Scale(const Point2f& scale) noexcept;

		/* Sets the GameObject's local position. This neither translates the GameObject nor triggers a matrix recalculation 
			However, if the Transform Component is part of a bigger tree, it will lead to the tree being recalculated */
		void SetPosition(const Point2f& position) noexcept;

		/* Sets the GameObject's local scale. This triggers a matrix recalculation */
		void SetScale(const Point2f& scale) noexcept;

		/* Sets the GameObject's local angle. This triggers a matrix recalculation */
		void SetAngle(const float angle) noexcept;

		/* This function should NOT be called manually. It is called internally and calling this manually will only waste CPU cycles. 
		   This function checks if its parent local position has changed. If it has, it must recalculate this component's world position */
		void RecalculateWorldData() noexcept;

		/* This function can be called manually, however it is not advised since this happens automatically whenever 
		   SetPosition() or Translate() get called 
		   This function makes sure to set the flag that the GameObject's position has been moved,
		   thus the world position should be recalculated */
		void SetHasMoved(const bool hasMoved) noexcept;

		/* Get the GameObject's local scale */
		const Point2f& GetLocalScale() const noexcept;

		/* Get the GameObject's local angle */
		const float GetLocalAngle() const noexcept;

		/* Get whether or not the GameObject has been moved this frame */
		const bool GetHasMoved() const noexcept;

		/* The Transform Manager this Transform Component is attached to */
		TransformManager* m_pTransformManager;

	private:
		void RecalculateTransformationMatrix() noexcept;

		bool m_TransformChanged;
		bool m_HasWorldDataChanged;

		Matrix3x3 m_TransformationMatrix;

		Point2f m_WorldPosition;
		Point2f m_WorldScale;
		float m_WorldAngle;
		
		Point2f m_LocalScale;
		float m_LocalAngle; // Radians
	};
}

