#pragma once

#include "../Integrian2D_API.h"
#include "Point2.h"

#include <utility>

namespace Integrian2D
{
	template<typename Type>
	struct INTEGRIAN2D_API Circle
	{
#pragma region Constructors
		explicit Circle<Type>()
			: radius{}
			, position{}
		{}
		explicit Circle<Type>(const Point<2, Type>& _position, const Type _radius)
			: radius{ _radius }
			, position{ _position }
		{}
#pragma endregion

#pragma region Rule Of 5
		Circle<Type>(const Circle<Type>& other) noexcept
			: radius{ other.radius }
			, position{ other.position }
		{}
		Circle<Type>(Circle<Type>&& other) noexcept
			: radius{ std::move(other.radius) }
			, position{ std::move(other.position) }
		{}
		Circle<Type>& operator=(const Circle<Type>& other) noexcept
		{
			radius = other.radius;
			position = other.position;
			return *this;
		}
		Circle<Type>& operator=(Circle<Type>&& other) noexcept
		{
			radius = std::move(other.radius);
			position = std::move(other.position);
			return *this;
		}
#pragma endregion

#pragma region Data
		Type radius;
		Point<2, Type> position;
#pragma endregion
	};

#pragma region Relational Operators
	template<typename Type>
	bool operator==(const Circle<Type>& c1, const Circle<Type>& c2) noexcept
	{
		return Utils::AreEqual(c1.radius, c2.radius) && (c1.position == c2.position);
	}

	template<typename Type>
	bool operator!=(const Circle<Type>& c1, const Circle<Type>& c2) noexcept
	{
		return !(c1 == c2);
	}
#pragma endregion

#pragma region Misscellaneous Operators
	template<typename Type>
	std::ostream& operator<<(std::ostream& os, const Circle<Type>& c1) noexcept
	{
		os << c1.position << ", " << c1.radius << std::endl;
		return os;
	}
#pragma endregion
}