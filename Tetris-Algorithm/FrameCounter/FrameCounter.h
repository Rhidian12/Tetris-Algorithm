#pragma once

#include <Components/Component/Component.h>

#include "../Delegate/Delegate.h"

class FrameCounter final : public Integrian2D::Component
{
public:
	FrameCounter(Integrian2D::GameObject* pOwner);

	virtual Integrian2D::Component* Clone(Integrian2D::GameObject* pOwner) noexcept override;
	virtual void Update() override;

	Delegate<uint64_t>& GetDelegate();

private:
	uint64_t m_CurrentFrame;
	Delegate<uint64_t> m_OnFrameUpdate;
};