#pragma once

#include "../Integrian2D_API.h"
#include "Point.h" // Point 
#include "../Iterator/Iterator.h"

#include <utility>

namespace Integrian2D
{
	/* This class is the specialisation of Point, representing a Point with 2 dimensions */

	/* A list of available operators:
	   Assume Type is the templated type provided to the Point2

	   Vector<2, Type> operator-(const Point<2, Type>& lhs, const Point<2, Type>& rhs)
	   Point<2, Type> operator-(const Point<2, Type>& lhs, const Vector<2, Type>& rhs)

	   Vector<2, Type> operator+(const Point<2, Type>& lhs, const Point<2, Type>& rhs)
	   Point<2, Type> operator+(const Point<2, Type>& lhs, const Vector<2, Type>& rhs)

	   Point<2, Type> operator*(const Point<2, Type>& lhs, const Type& rhs)

	   Point<2, Type> operator/(const Point<2, Type>& lhs, const Type& rhs)

	   Point<2, Type>& operator-=(Point<2, Type>& lhs, const Point<2, Type>& rhs)
	   Point<2, Type>& operator-=(Point<2, Type>& lhs, const Vector<2, Type>& rhs)

	   Point<2, Type>& operator+=(Point<2, Type>& lhs, const Point<2, Type>& rhs)
	   Point<2, Type>& operator+=(Point<2, Type>& lhs, const Vector<2, Type>& rhs)

	   Point<2, Type>& operator*=(Point<2, Type>& lhs, Type& rhs)
	   Point<2, Type>& operator*=(Point<2, Type>& lhs, Vector<2, Type>& rhs)

	   Point<2, Type>& operator/=(Point<2, Type>& lhs, Type& rhs)
	   Point<2, Type>& operator/=(Point<2, Type>& lhs, Vector<2, Type>& rhs)

	   template<typename Integral>
	   Type& operator[](const Integral index)
	   =>	It is important to note that the order of the variables is x, y
	   =>	Therefore, calling Point2f{}[0] will result in the x value being returned

	   template<typename Integral>
	   const Type& operator[](const Integral index) const

	   bool operator==(const Point<2, Type>& lhs, const Point<2, Type>& other)
	   bool operator!=(const Point<2, Type>& lhs, const Point<2, Type>& other)

	   std::ostream& operator<<(std::ostream& os, const Point<2, Type>& point)
	   */

	   /* A list of available functions:
		  Type DistanceSquared(const Point<2, Type>& p1, const Point<2, Type>& p2)
		  =>	Returns the squared distance between the two provided points

		  Type Distance(const Point<2, Type>& p1, const Point<2, Type>& p2)
		  =>	Returns the distance between the two provided points
		  */

		  /* The Point2 iterator covers the member variables x and y (in that order) */

	template<typename Type>
	struct INTEGRIAN2D_API Point<2, Type>
	{
#pragma region Constructors
		explicit Point<2, Type>()
			: x{}
			, y{}
		{}
		explicit Point<2, Type>(const Type& _x, const Type& _y)
			: x{ _x }
			, y{ _y }
		{}
		explicit Point<2, Type>(const Point<3, Type>& p)
			: x{ p.x }
			, y{ p.y }
		{}
		explicit Point<2, Type>(const Vector<2, Type>& _xy)
			: x{ _xy.x }
			, y{ _xy.y }
		{}
		explicit Point<2, Type>(const Vector<3, Type>& _xy)
			: x{ _xy.x }
			, y{ _xy.y }
		{}
#pragma endregion

#pragma region Rule Of 5
		Point<2, Type>(const Point<2, Type>& p) noexcept
			: x{ p.x }
			, y{ p.y }
		{}
		Point<2, Type>(Point<2, Type>&& other) noexcept
			: x{ std::move(other.x) }
			, y{ std::move(other.y) }
		{}
		Point<2, Type>& operator=(const Point<2, Type>& p) noexcept
		{
			x = p.x;
			y = p.y;
			return *this;
		}
		Point<2, Type>& operator=(Point<2, Type>&& p) noexcept
		{
			x = std::move(p.x);
			y = std::move(p.y);
			return *this;
		}
#pragma endregion

#pragma region Data
		union
		{
			Type data[2];

#pragma warning ( push )
#pragma warning ( disable : 4201 ) // Disable nameless struct warning
			struct
			{
				Type x, y;
			};
#pragma warning ( pop )
		};
#pragma endregion

#pragma region Access Operators
		template<typename IntegralNumber, typename = std::enable_if_t<std::is_integral_v<IntegralNumber>>>
		Type& operator[](const IntegralNumber index) noexcept
		{
			ASSERT(index < 2 && index > -1, "Point<2, Type>::operator[] > The index was out of bounds!");
			return data[index];
		}

		template<typename IntegralNumber, typename = std::enable_if_t<std::is_integral_v<IntegralNumber>>>
		const Type& operator[](const IntegralNumber index) const noexcept
		{
			ASSERT(index < 2 && index > -1, "Point<2, Type>::operator[] > The index was out of bounds!");
			return data[index];
		}
#pragma endregion

#pragma region Iterator
		Iterator<Type> begin() noexcept
		{
			return Iterator<Type>{ &x };
		}

		ConstIterator<Type> begin() const noexcept
		{
			return ConstIterator<Type>{ &x };
		}

		Iterator<Type> end() noexcept
		{
			return Iterator<Type>{ &y + 1 };
		}

		ConstIterator<Type> end() const noexcept
		{
			return ConstIterator<Type>{ &y + 1 };
		}
#pragma endregion

#pragma region Required Member Operators
		Point<2, Type> operator-() noexcept
		{
			return Point<2, Type>{ -x, -y };
		}
#pragma endregion
	};

#pragma region Arithmetic Operators
	template<typename Type>
	Vector<2, Type> operator-(const Point<2, Type>& lhs, const Point<2, Type>& rhs) noexcept
	{
		return Vector<2, Type>{lhs.x - rhs.x, lhs.y - rhs.y};
	}

	template<typename Type>
	Point<2, Type> operator-(const Point<2, Type>& lhs, const Vector<2, Type>& rhs) noexcept
	{
		return Point<2, Type>{lhs.x - rhs.x, lhs.y - rhs.y};
	}

	template<typename Type>
	Vector<2, Type> operator+(const Point<2, Type>& lhs, const Point<2, Type>& rhs) noexcept
	{
		return Vector<2, Type>{lhs.x + rhs.x, lhs.y + rhs.y};
	}

	template<typename Type>
	Point<2, Type> operator+(const Point<2, Type>& lhs, const Vector<2, Type>& rhs) noexcept
	{
		return Point<2, Type>{lhs.x + rhs.x, lhs.y + rhs.y};
	}

	template<typename Type>
	Point<2, Type> operator*(const Point<2, Type>& lhs, Type& rhs) noexcept
	{
		return Point<2, Type>{lhs.x* rhs, lhs.y* rhs};
	}

	template<typename Type>
	Point<2, Type> operator/(const Point<2, Type>& lhs, Type& rhs) noexcept
	{
		return Point<2, Type>{lhs.x* rhs, lhs.y* rhs};
	}
#pragma endregion

#pragma region Compound Assignment Operators
	template<typename Type>
	Point<2, Type>& operator+=(Point<2, Type>& lhs, const Point<2, Type>& rhs) noexcept
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		return lhs;
	}

	template<typename Type>
	Point<2, Type>& operator+=(Point<2, Type>& lhs, const Vector<2, Type>& rhs) noexcept
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		return lhs;
	}

	template<typename Type>
	Point<2, Type>& operator-=(Point<2, Type>& lhs, const Point<2, Type>& rhs) noexcept
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		return lhs;
	}

	template<typename Type>
	Point<2, Type>& operator-=(Point<2, Type>& lhs, const Vector<2, Type>& rhs) noexcept
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		return lhs;
	}

	template<typename Type>
	Point<2, Type>& operator*=(Point<2, Type>& lhs, Type& rhs) noexcept
	{
		lhs.x *= rhs;
		lhs.y *= rhs;
		return lhs;
	}

	template<typename Type>
	Point<2, Type>& operator*=(Point<2, Type>& lhs, const Vector<2, Type>& rhs) noexcept
	{
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
		return lhs;
	}

	template<typename Type>
	Point<2, Type>& operator/=(Point<2, Type>& lhs, Type& rhs) noexcept
	{
		lhs.x /= rhs;
		lhs.y /= rhs;
		return lhs;
	}

	template<typename Type>
	Point<2, Type>& operator/=(Point<2, Type>& lhs, const Vector<2, Type>& rhs) noexcept
	{
		lhs.x /= rhs.x;
		lhs.y /= rhs.y;
		return lhs;
	}
#pragma endregion

#pragma region Relational Operators
	template<typename Type>
	bool operator==(const Point<2, Type>& lhs, const Point<2, Type>& other) noexcept
	{
		return Utils::AreEqual(lhs.x, other.x) && Utils::AreEqual(lhs.y, other.y);
	}

	template<typename Type>
	bool operator!=(const Point<2, Type>& lhs, const Point<2, Type>& other) noexcept
	{
		return !(lhs == other);
	}
#pragma endregion

#pragma region Miscellaneous Operators
	template<typename Type>
	std::ostream& operator<<(std::ostream& os, const Point<2, Type>& point) noexcept
	{
		os << point.x << ", " << point.y;
		return os;
	}
#pragma endregion
}