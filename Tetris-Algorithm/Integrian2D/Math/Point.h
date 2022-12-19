#pragma once

#include "../Integrian2D_API.h"

#include "Vector.h"

namespace Integrian2D
{
	/* This is the base class representing a point
	   I suggest using the provided Point2f type, but others can be created using this class */

	/* Available functions:
		Type DistanceSquared(const Point<P, Type>& p1, const Point<P, Type>& p2) 
		=>		Returns the squared distance between the two provided points
		
		Type Distance(const Point<P, Type>& p1, const Point<P, Type>& p2)
		=>		Returns the distance between the two provided points
		*/

	template<int P, typename Type>
	struct INTEGRIAN2D_API Point
	{
		// == Data ==
		Type data[P];

#pragma region Access Operators
		template<typename IntegralNumber, typename = std::enable_if_t<std::is_integral_v<IntegralNumber>>>
		Type& operator[](const IntegralNumber index) noexcept
		{
			ASSERT(index < P, "Point::operator[]() > index is out of bounds!");

			return data[index];
		}

		template<typename IntegralNumber, typename = std::enable_if_t<std::is_integral_v<IntegralNumber>>>
		const Type& operator[](const IntegralNumber index) const noexcept
		{
			ASSERT(index < P, "Point::operator[]() > index is out of bounds!");

			return data[index];
		}
#pragma endregion
	};

	// == Non-Member Functions That Are Useful For All Points ==
	template<int P, typename Type>
	Type DistanceSquared(const Point<P, Type>& p1, const Point<P, Type>& p2) noexcept
	{
		const Vector<P, Type> vector{ p1, p2 };
		return static_cast<Type>(MagnitudeSquared(vector));
	}

	template<int P, typename Type>
	Type Distance(const Point<P, Type>& p1, const Point<P, Type>& p2) noexcept
	{
		const Vector<P, Type> vector{ p1, p2 };
		return static_cast<Type>(sqrt(MagnitudeSquared(vector)));
	}
};
