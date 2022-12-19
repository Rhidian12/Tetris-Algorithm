#pragma once

#include "../../Integrian2D_API.h"
#include "../Component/Component.h"
#include "../../AI/BehaviourState.h"

namespace Integrian2D
{
	class Blackboard;
	class BaseDecisionMaking;

	class AIComponent final : public Component
	{
	public:
		INTEGRIAN2D_API AIComponent(GameObject* const pOwner, Blackboard* const pBlackboard, BaseDecisionMaking* const pAI);
		INTEGRIAN2D_API ~AIComponent();

		AIComponent(const AIComponent& other) noexcept;
		AIComponent(AIComponent&& other) noexcept;
		AIComponent& operator=(const AIComponent& other) noexcept;
		AIComponent& operator=(AIComponent&& other) noexcept;

		INTEGRIAN2D_API virtual Component* Clone(GameObject* pOwner) noexcept override;

		INTEGRIAN2D_API virtual void Update() override;

		INTEGRIAN2D_API Blackboard* const GetBlackboard() const noexcept;
		INTEGRIAN2D_API BaseDecisionMaking* const GetDecisionMaking() const noexcept;
		INTEGRIAN2D_API BehaviourState GetCurrentState() const noexcept;

	private:
		Blackboard* m_pBlackboard;
		BaseDecisionMaking* m_pDecisionMaking;
	};
}