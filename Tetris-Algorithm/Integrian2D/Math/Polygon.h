#pragma once

#include "../Integrian2D_API.h"
#include "Point.h"

namespace Integrian2D
{
	/* This class is the base class representing a 2D polygon
	   I suggest using the provided Polygon types - such as PTriangle - but others can be created using this class */

	template<int Points, typename Type>
	struct INTEGRIAN2D_API Polygon
	{
		Point<2, Type> data[Points];

		template<typename IntegralNumber, typename = std::enable_if_t<std::is_integral_v<IntegralNumber>>>
		Type& operator[](const IntegralNumber index) noexcept
		{
			ASSERT(index < Points, "Polygon<Points, Type>::operator[] > Index is out of bounds!");
			return data[index];
		}
		
		template<typename IntegralNumber, typename = std::enable_if_t<std::is_integral_v<IntegralNumber>>>
		const Type& operator[](const IntegralNumber index) const noexcept
		{
			ASSERT(index < Points, "Polygon<Points, Type>::operator[] > Index is out of bounds!");
			return data[index];
		}
	};
}