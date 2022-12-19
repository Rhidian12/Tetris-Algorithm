#pragma once

#include "../Integrian2D_API.h"
#include "Event/Event.h"

#include <vector>
#include <deque>

namespace Integrian2D
{
	class IListener;

	class EventQueue final
	{
	public:
		INTEGRIAN2D_API static EventQueue* const GetInstance() noexcept;
		static void Cleanup() noexcept;

		/* Queue an event to the EventQueue, this EventQueue operates on a principle of FIFO (First In, First Out) */
		template<typename Type>
		void QueueEvent(const char* eventName, const Type& data, const int delay = 0) noexcept
		{
			m_Events.emplace_back(Event{ EventImplementation{ eventName, data}, delay});
		}

		/* This function gets called interally and should NOT get called manually
		   All events that were added before Update() gets called get executed, or their timer gets increased by 1
		   See Event for documentation regarding the event timer */
		void Update() noexcept;

		/* Add a Listener class that should receive events from the EventQueue */
		INTEGRIAN2D_API void AddListener(IListener* const pListener) noexcept;

		/* Remove a Listener class from the EventQueue */
		INTEGRIAN2D_API void RemoveListener(IListener* const pListener) noexcept;

		/* Check if all events have been processed */
		INTEGRIAN2D_API bool AreAllEventsProcessed() const noexcept;

	private:
		friend class Scene;

		EventQueue();

		inline static EventQueue* m_pInstance{};
		std::vector<IListener*> m_pListeners;
		std::deque<Event> m_Events;
	};
}
