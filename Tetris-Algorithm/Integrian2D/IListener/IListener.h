#pragma once

#include "../Integrian2D_API.h"
#include "../EventQueue/Event/Event.h"

namespace Integrian2D
{
	/* This class is supposed to be inherited from 
	   Any instance of this class gets added to the EventQueue */
	class INTEGRIAN2D_API IListener
	{
	public:
		/* Upon creation, any instance of this class gets added to the EventQueue */
		IListener();
		virtual ~IListener() = default;

		/* The function that receives the events broadcasted by the EventQueue */
		virtual bool OnEvent(const Event&) = 0;

		/* Check whether or not this IListener is receiving events
		   This is true by default */
		bool m_IsActive;
	};
}