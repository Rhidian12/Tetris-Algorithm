#pragma once

#include "../Integrian2D_API.h"
#include "Point2.h"
#include "../Utils/Utils.h"

#include <utility>
#include <algorithm>

#undef min
#undef max

namespace Integrian2D
{
	/* This class is a much simpler version of PTriangle, containing only information about its 3 vertices */

	/* IMPORTANT NOTE: The vertices of a triangle are defined in a CLOCKWISE ORDER */
		//		    pointTwo
		//		   /		\
		//		  /			 \
		//		 /			  \
		//		/			   \
		//	   /				\
		//	  /					 \
		// pointOne ========= pointThree

		/* A list of available operators:
		   Assume Type is the templated Type provided to the Triangle

		   bool operator==(const Triangle<Type>& t1, const Triangle<Type>& t2)
		   bool operator!=(const Triangle<Type>& t1, const Triangle<Type>& t2)
		   */

		   /* A list of available functions:
			  Assume Type is the templated Type provided to the Triangle

			  Type GetArea(const Triangle<Type>& t)
			  =>		Returns the Triangle's area

			  Type GetWidth(const Triangle<Type>& t)
			  =>		Returns the Triangle's width

			  Type GetHeight(cont Triangle<Type>& t)
			  =>		Returns the Triangle's height
			*/

	template<typename Type>
	struct INTEGRIAN2D_API Triangle
	{
#pragma region Constructors
		explicit Triangle()
			: leftPoint{}
			, topPoint{}
			, rightPoint{}
		{}
		explicit Triangle(const Point<2, Type>& _leftPoint, const Point<2, Type>& _topPoint, const Point<2, Type>& _rightPoint)
			: leftPoint{ _leftPoint }
			, topPoint{ _topPoint }
			, rightPoint{ _rightPoint }
		{}
#pragma endregion

#pragma region Rule of 5
		Triangle(const Triangle& other) noexcept
			: leftPoint{ other.leftPoint }
			, topPoint{ other.topPoint }
			, rightPoint{ other.rightPoint }
		{}
		Triangle(Triangle&& other) noexcept
			: leftPoint{ std::move(other.leftPoint) }
			, topPoint{ std::move(other.topPoint) }
			, rightPoint{ std::move(other.rightPoint) }
		{}
		Triangle& operator=(const Triangle& other) noexcept
		{
			leftPoint = other.leftPoint;
			rightPoint = other.rightPoint;
			topPoint = other.topPoint;

			return *this;
		}
		Triangle& operator=(Triangle&& other) noexcept
		{
			leftPoint = std::move(other.leftPoint);
			rightPoint = std::move(other.rightPoint);
			topPoint = std::move(other.topPoint);

			return *this;
		}
#pragma endregion

#pragma region Data
		Point<2, Type> leftPoint, topPoint, rightPoint;
#pragma endregion
	};

#pragma region Relational Operators
	template<typename Type>
	inline bool operator==(const Triangle<Type>& a, const Triangle<Type>& b) noexcept
	{
		return a.leftPoint == b.leftPoint && a.topPoint == b.topPoint && a.rightPoint == b.rightPoint;
	}

	template<typename Type>
	inline bool operator!=(const Triangle<Type>& a, const Triangle<Type>& b) noexcept
	{
		return !(a == b);
	}
#pragma endregion


#pragma region Functions
	template<typename Type>
	Type GetWidth(const Triangle<Type>& t) noexcept
	{
		const Point<2, Type> leftPoint = Utils::AreEqual(std::min(t.leftPoint.x, t.rightPoint.x), t.leftPoint.x) ? t.leftPoint : t.rightPoint;
		const Point<2, Type> rightPoint = Utils::AreEqual(leftPoint.x, t.leftPoint.x) ? t.rightPoint : t.leftPoint;
		
		return rightPoint.x - leftPoint.x;
	}

	template<typename Type>
	Type GetHeight(const Triangle<Type>& t) noexcept
	{
		const Point<2, Type> topPoint = Utils::AreEqual(std::max(t.leftPoint.y, t.topPoint.y), t.topPoint.y) ? t.topPoint : t.leftPoint;
		const Point<2, Type> bottomPoint = Utils::AreEqual(topPoint.y, t.topPoint.y) ? t.leftPoint: t.topPoint;

		return topPoint.y - bottomPoint.y;
	}

	template<typename Type>
	Type GetArea(const Triangle<Type>& t) noexcept
	{
		return (GetWidth(t) * GetHeight(t)) * static_cast<Type>(0.5f);
	}
#pragma endregion
}