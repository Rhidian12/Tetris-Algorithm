#pragma once

#include "../../Integrian2D_API.h"
#include "EventImplementation.h"

namespace Integrian2D
{
	/* An Event gets only used by the EventQueue, which sends the Event to all of the EventQueue's Listeners 
	   An Event has an identifier - which is required - optional data and an optional delay 
	   The delay is set to 0 by default 
	   The delay is used to indicate by how many frames an event should get delayed before it gets broadcasted by the EventQueue */
	struct INTEGRIAN2D_API Event final
	{
		/* Create an Event 
		   The EventImplementation constructor looks like this: 
			
			EventImplementation(const std::string& eventName);

			or

			template<typename Type>
			EventImplementation(const std::string& eventName, const Type& data); */
		Event(const EventImplementation& _event, const int _delay = 0);

		/* The Event, from which you can call GetEvent() - which is the identifier -
		   and GetData() which is the optionally provided data */
		EventImplementation event;

		/* The provided delay */
		int delay;
		
		/* Internal usage, do NOT adjust this */
		int delayCounter;

		/* Internal usage, do NOT adjust this */
		bool isExecuted;
	};
}