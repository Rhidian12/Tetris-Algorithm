#pragma once

#include "../../../Integrian2D_API.h"
#include "../../BehaviourState.h"

namespace Integrian2D
{
	class AIComponent;
	class Blackboard;

	class INTEGRIAN2D_API BaseDecisionMaking
	{
	public:
		BaseDecisionMaking(Blackboard* const pBlackboard);
		virtual ~BaseDecisionMaking();

		virtual BaseDecisionMaking* Clone() noexcept = 0;

		virtual BehaviourState Update() = 0;

		BehaviourState GetCurrentState() const noexcept;
		Blackboard* const GetBlackboard() const noexcept;

	protected:
		BehaviourState m_CurrentState;
		Blackboard* m_pBlackboard;
	};
}