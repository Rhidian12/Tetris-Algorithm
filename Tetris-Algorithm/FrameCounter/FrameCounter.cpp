#include "FrameCounter.h"

FrameCounter::FrameCounter(Integrian2D::GameObject* pOwner)
	: Component{ pOwner }
	, m_CurrentFrame{}
{}

Integrian2D::Component* FrameCounter::Clone(Integrian2D::GameObject* pOwner) noexcept
{
	return new FrameCounter{ pOwner };
}

void FrameCounter::Update()
{
	if (m_CurrentFrame > 0u)
		m_OnFrameUpdate.Invoke(m_CurrentFrame);

	++m_CurrentFrame;
}

Delegate<uint64_t>& FrameCounter::GetDelegate()
{
	return m_OnFrameUpdate;
}