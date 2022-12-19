#pragma once

#include "../Integrian2D_API.h"
#include "Point.h" // Point
#include "../Iterator/Iterator.h"

#include <utility>

namespace Integrian2D
{
	/* This class is the specialisation of Point, representing a Point with 3 dimensions */

		/* A list of available operators:
	       Assume Type is the templated type provided to the Point3
		   
		   Vector<3, Type> operator-(const Point<3, Type>& lhs, const Point<3, Type>& rhs)
		   Point<3, Type> operator-(const Point<3, Type>& lhs, const Vector<3, Type>& rhs)

		   Vector<3, Type> operator+(const Point<3, Type>& lhs, const Point<3, Type>& rhs)
		   Point<3, Type> operator+(const Point<3, Type>& lhs, const Vector<3, Type>& rhs)

		   Point<3, Type> operator*(const Point<3, Type>& lhs, const Type& rhs)

		   Point<3, Type> operator/(const Point<3, Type>& lhs, const Type& rhs)

		   Point<3, Type>& operator+=(Point<3, Type>& lhs, const Point<3, Type>& rhs)
		   Point<3, Type>& operator+=(Point<3, Type>& lhs, const Vector<3, Type>& rhs)

		   Point<3, Type>& operator-=(Point<3, Type>& lhs, const Point<3, Type>& rhs)
		   Point<3, Type>& operator-=(Point<3, Type>& lhs, const Vector<3, Type>& rhs)

		   Point<3, Type>& operator*=(Point<3, Type>& lhs, const Type& rhs)
		   Point<3, Type>& operator*=(Point<3, Type>& lhs, const Vector<3, Type>& rhs)
		   
		   Point<3, Type>& operator/=(Point<3, Type>& lhs, const Type& rhs)
		   Point<3, Type>& operator/=(Point<3, Type>& lhs, const Vector<3, Type>& rhs)

		   template<typename Integral>
		   Type& operator[](const Integral index)
		   =>	It is important to note that the ordedr of the variables is x, y, z
		   =>	Therefore, calling Point3f{}[1] will result in the y value being returned

		   template<typename Integral>
		   const Type& operator[](const Integral index) const

		   bool operator==(const Point<3, Type>& lhs, const Point<3, Type>& rhs)
		   bool operator!=(const Point<3, Type>& lhs, const Point<3, Type>& rhs)

			std::ostream& operator<<(std::ostream& os, const Point<3, Type>& point)
		   */

	    /* A list of available functions:
	 	   Type DistanceSquared(const Point<3, Type>& p1, const Point<3, Type>& p2)
	 	   =>	Returns the squared distance between the two provided points
	 	   
	 	   Type Distance(const Point<3, Type>& p1, const Point<3, Type>& p2)
	 	   =>	Returns the distance between the two provided points
	 	   */

		/* The Point3 iterator covers the member variables x, y and z (in that order) */

	template<typename Type>
	struct INTEGRIAN2D_API Point<3, Type>
	{
#pragma region Constructors
		explicit Point<3, Type>()
			: x{}
			, y{}
			, z{}
		{}
		explicit Point<3, Type>(const Type& _x, const Type& _y)
			: x{ _x }
			, y{ _y }
			, z{}
		{}
		explicit Point<3, Type>(const Type& _x, const Type& _y, const Type& _z)
			: x{ _x }
			, y{ _y }
			, z{ _z }
		{}
		explicit Point<3, Type>(const Point<2, Type>& _xy)
			: x{ _xy.x }
			, y{ _xy.y }
			, z{}
		{}
		explicit Point<3, Type>(const Vector<2, Type>& _xy)
			: x{ _xy.x }
			, y{ _xy.y }
			, z{}
		{}
		explicit Point<3, Type>(const Vector<3, Type>& _xyz)
			: x{ _xyz.x }
			, y{ _xyz.y }
			, z{ _xyz.z }
		{}
#pragma endregion

#pragma region Rule Of 5
		Point<3, Type>(const Point<3, Type>& other) noexcept
			: x{ other.x }
			, y{ other.y }
			, z{ other.z }
		{}
		Point<3, Type>(Point<3, Type>&& other) noexcept
			: x{ std::move(other.x) }
			, y{ std::move(other.y) }
			, z{ std::move(other.z) }
		{}
		Point<3, Type>& operator=(const Point<3, Type>& other) noexcept
		{
			x = other.x;
			y = other.y;
			z = other.y;

			return *this;
		}
		Point<3, Type>& operator=(Point<3, Type>&& other) noexcept
		{
			x = std::move(other.x);
			y = std::move(other.y);
			z = std::move(other.y);

			return *this;
		}
#pragma endregion

#pragma region Data
		union
		{
			Type data[3];

#pragma warning ( push )
#pragma warning ( disable : 4201 ) // Disable nameless struct warning
			struct
			{
				Point<2, Type> xy;
				Type x, y, z;
			};
#pragma warning ( pop )
		};
#pragma endregion

#pragma region Access Operators
		template<typename IntegralNumber, typename = std::enable_if_t<std::is_integral_v<IntegralNumber>>>
		Type& operator[](const IntegralNumber index) noexcept
		{
			ASSERT(index < 3 && index > -1, "Point<3, Type>::operator[] > The index was out of bounds!");
			return data[index];
		}

		template<typename IntegralNumber, typename = std::enable_if_t<std::is_integral_v<IntegralNumber>>>
		const Type& operator[](const IntegralNumber index) const noexcept
		{
			ASSERT(index < 3 && index > -1, "Point<3, Type>::operator[] > The index was out of bounds!");
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
			return Iterator<Type>{ &z + 1 };
		}

		ConstIterator<Type> end() const noexcept
		{
			return ConstIterator<Type>{ &z + 1 };
		}
#pragma endregion

#pragma region Required Member Operators
		Point<3, Type> operator-() noexcept
		{
			return Point<3, Type>{ -x, -y };
		}
#pragma endregion
	};

#pragma region Arithmetic Operators
	template<typename Type>
	Vector<3, Type> operator-(const Point<3, Type>& lhs, const Point<3, Type>& rhs) noexcept
	{
		return Vector<3, Type>{lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
	}

	template<typename Type>
	Point<3, Type> operator-(const Point<3, Type>& lhs, const Vector<3, Type>& rhs) noexcept
	{
		return Point<3, Type>{lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
	}

	template<typename Type>
	Vector<3, Type> operator+(const Point<3, Type>& lhs, const Point<3, Type>& rhs) noexcept
	{
		return Vector<3, Type>{lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
	}

	template<typename Type>
	Point<3, Type> operator+(const Point<3, Type>& lhs, const Vector<3, Type>& rhs) noexcept
	{
		return Point<3, Type>{lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
	}

	template<typename Type>
	Point<3, Type> operator*(const Point<3, Type>& lhs, const Type& rhs) noexcept
	{
		return Point<3, Type>{lhs.x* rhs.x, lhs.y* rhs.y, lhs.z* rhs.z};
	}

	template<typename Type>
	Point<3, Type> operator/(const Point<3, Type>& lhs, const Type& rhs) noexcept
	{
		return Point<3, Type>{lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z};
	}
#pragma endregion

#pragma region Compound Assignment Operators
	template<typename Type>
	Point<3, Type>& operator+=(Point<3, Type>& lhs, const Point<3, Type>& rhs) noexcept
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		lhs.z += rhs.z;
		return lhs;
	}

	template<typename Type>
	Point<3, Type>& operator+=(Point<3, Type>& lhs, const Vector<3, Type>& rhs) noexcept
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		lhs.z += rhs.z;
		return lhs;
	}

	template<typename Type>
	Point<3, Type>& operator-=(Point<3, Type>& lhs, const Point<3, Type>& rhs) noexcept
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		lhs.z -= rhs.z;
		return lhs;
	}

	template<typename Type>
	Point<3, Type>& operator-=(Point<3, Type>& lhs, const Vector<3, Type>& rhs) noexcept
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		lhs.z -= rhs.z;
		return lhs;
	}

	template<typename Type>
	Point<3, Type>& operator*=(Point<3, Type>& lhs, const Type& rhs) noexcept
	{
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
		lhs.z *= rhs.z;
		return lhs;
	}

	template<typename Type>
	Point<3, Type>& operator*=(Point<3, Type>& lhs, const Vector<3, Type>& rhs) noexcept
	{
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
		lhs.z *= rhs.z;
		return lhs;
	}

	template<typename Type>
	Point<3, Type>& operator/=(Point<3, Type>& lhs, const Type& rhs) noexcept
	{
		lhs.x /= rhs.x;
		lhs.y /= rhs.y;
		lhs.z /= rhs.z;
		return lhs;
	}

	template<typename Type>
	Point<3, Type>& operator/=(Point<3, Type>& lhs, const Vector<3, Type>& rhs) noexcept
	{
		lhs.x /= rhs.x;
		lhs.y /= rhs.y;
		lhs.z /= rhs.z;
		return lhs;
	}
#pragma endregion

#pragma region Relational Operators
	template<typename Type>
	bool operator==(const Point<3, Type>& lhs, const Point<3, Type>& rhs) noexcept
	{
		return Utils::AreEqual(lhs.x, rhs.x) && Utils::AreEqual(lhs.y, rhs.y) && Utils::AreEqual(lhs.z, rhs.z);
	}

	template<typename Type>
	bool operator!=(const Point<3, Type>& lhs, const Point<3, Type>& rhs) noexcept
	{
		return !(lhs == rhs);
	}
#pragma endregion

#pragma region Miscellaneous Operators
	template<typename Type>
	std::ostream& operator<<(std::ostream& os, const Point<3, Type>& point) noexcept
	{
		os << point.x << ", " << point.y << ", " << point.z;
		return os;
	}
#pragma endregion
}