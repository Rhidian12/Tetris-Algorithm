#include "Timer.h"

Timer& Timer::GetInstance()
{
	if (!m_pInstance)
		m_pInstance.reset(new Timer{});

	return *m_pInstance.get();
}