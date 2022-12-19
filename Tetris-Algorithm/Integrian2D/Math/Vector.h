#pragma once

#include "../Integrian2D_API.h"
#include "../Utils/Utils.h"

namespace Integrian2D
{
	/* This is the base class representing a vector
		I suggest using the provided Vector typedefs, but others can be created using this class */

	/* Available functions:
		Type MagnitudeSquared(const Vector<V, Type>& v1)
		=>		Returns the squared magnitude of the provided vector
		
		Type Magnitude(const Vector<V, Type>& v1)
		=>		Returns the magnitude of the provided vector
		
		Type Normalize(Vector<V, Type>& v1)
		=>		Normalizes the provided vector and returns the vector's magnitude
		
		Vector<V, Type> GetNormalized(const Vector<V, Type>& v1)
		=>		Returns a normalized copy of the provided vector 
		*/

	template<int V, typename Type>
	struct INTEGRIAN2D_API Vector
	{
		// == Data == 
		Type data[V];

#pragma region Access Operators
		template<typename IntegralNumber, typename = std::enable_if_t<std::is_integral_v<IntegralNumber>>>
		Type& operator[](const IntegralNumber index) noexcept
		{
			ASSERT(index < V, "Vector::operator[]() > index is out of bounds!");

			return data[index];
		}

		template<typename IntegralNumber, typename = std::enable_if_t<std::is_integral_v<IntegralNumber>>>
		const Type& operator[](const IntegralNumber index) const noexcept
		{
			ASSERT(index < V, "Vector::operator[]() > index is out of bounds!");

			return data[index];
		}
#pragma endregion
	};

	// == Non-Member Methods That Are Useful For All Vectors ==
	template<int V, typename Type>
	Type MagnitudeSquared(const Vector<V, Type>& v1) noexcept
	{
		return Dot(v1, v1);
	}

	template<int V, typename Type>
	Type Magnitude(const Vector<V, Type>& v1) noexcept
	{
		return static_cast<Type>(sqrt(MagnitudeSquared(v1)));
	}

	template<int V, typename Type>
	Type Normalize(Vector<V, Type>& v1)
	{
		const Type magnitude{ Magnitude(v1) };

		ASSERT(magnitude != static_cast<Type>(0.f), "Vector::Normalize() > Magnitude cannot be 0!");

		v1 /= magnitude;
		return magnitude;
	}

	template<int V, typename Type>
	Vector<V, Type> GetNormalized(const Vector<V, Type>& v1)
	{
		Vector<V, Type> temp{ v1 };
		Normalize(temp);
		return temp;
	}
}