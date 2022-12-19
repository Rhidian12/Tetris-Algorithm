#pragma once

#include "../Integrian2D_API.h"
#include "../Utils/Utils.h"
#include "Point2.h"
#include "Vector2.h"

#include <utility>

namespace Integrian2D
{
	/* This class is a much simpler version of PRect, containing only information about its position, width and height */

		/* A list of available operators:
		   Assume Type is the templated Type provided to the Rect

		   bool operator==(const Rect<Type>& r1, const Rect<Type>& r2)
		   bool operator!=(const Rect<Type>& r1, const Rect<Type>& r2)
		   */

		   /* A list of available functions:
			  Assume Type is the templated Type provided to the Rect

			  Type GetArea(const Rect<Type>& r)
			  =>		Returns the Rect's area
			*/

	template<typename Type>
	struct INTEGRIAN2D_API Rect
	{
#pragma region Constructors
		explicit Rect()
			: x{}
			, y{}
			, width{}
			, height{}
		{}
		explicit Rect(const Type _x, const Type _y, const Type _width, const Type _height)
			: x{ _x }
			, y{ _y }
			, width{ _width }
			, height{ _height }
		{}
		explicit Rect(const Point<2, Type>& _xy, const Type _width, const Type _height)
			: x{ _xy.x }
			, y{ _xy.y }
			, width{ _width }
			, height{ _height }
		{}
#pragma endregion

#pragma region Rule of 5
		Rect(const Rect& other) noexcept
			: x{ other.x }
			, y{ other.y }
			, width{ other.width }
			, height{ other.height }
		{}
		Rect(Rect&& other) noexcept
			: x{ std::move(other.x) }
			, y{ std::move(other.y) }
			, width{ std::move(other.width) }
			, height{ std::move(other.height) }
		{}
		Rect& operator=(const Rect& other) noexcept
		{
			x = other.x;
			y = other.y;
			width = other.width;
			height = other.height;

			return *this;
		}
		Rect& operator=(Rect&& other) noexcept
		{
			x = std::move(other.x);
			y = std::move(other.y);
			width = std::move(other.width);
			height = std::move(other.height);

			return *this;
		}
#pragma endregion

#pragma region Data
		union
		{
#pragma warning ( push )
#pragma warning ( disable : 4201 ) // Disable nameless struct warning
			struct { Type x, y; };
#pragma warning ( pop )
			Point<2, Type> xy;
		};

		Type width, height;
#pragma endregion
	};

#pragma region Relational Operators
	template<typename Type>
	bool operator==(const Rect<Type>& r1, const Rect<Type>& r2) noexcept
	{
		return (r1.xy == r2.xy) && Utils::AreEqual(r1.width, r2.width) && Utils::AreEqual(r1.height, r2.height);
	}

	template<typename Type>
	bool operator!=(const Rect<Type>& r1, const Rect<Type>& r2) noexcept
	{
		return !(r1 == r2);
	}
#pragma endregion

#pragma region Functions
	template<typename Type>
	Type GetArea(const Rect<Type>& r) noexcept
	{
		return r.width * r.height;
	}
#pragma endregion
}