#pragma once

#include "../Integrian2D_API.h"
#include "Polygon.h"
#include "Point2.h"
#include "../Iterator/Iterator.h"

#include <utility>

namespace Integrian2D
{
	/* This class is the specialisation of Polygon, representing a 2D Rectangle
	   This class is supposed to be a more complex Rect class, holding information regarding angle, ... */

	   /* A list of available operators:
		  Assume Type is the templated Type provided to the PRect

		  bool operator==(const Polygon<4, Type>& lhs, const Polygon<4, Type>& rhs)
		  bool operator!=(const Polygon<4, Type>& lhs, const Polygon<4, Type>& rhs)
		  */

		  /* A list of available functions:
			 void Translate(Polygon<4, Type>& p, const Vector<2, Type>& v)
			 =>	Translates the PRect with the provided Vector2

			 void Rotate(Polygon<4, Type>& p, const Type& _angleChange)
			 =>	Rotates the PRect by adding the provided angle to the PRect's current angle
			 =>	This function calls SetRotation() with the new angle internally
			 =>	_angleChange is presumed to be in radians

			 void Scale(Polygon<4, Type>& p, const Point<2, Type>& scale)
			 =>	Scales the PRect by adding the provided scale to the PRect's current scale
			 =>	This function calls SetScale() with the new scale internally

			 void SetRotation(Polygon<4, Type>& p, const Type& _angle)
			 =>	Rotates the PRect by rotating around its pivot point
			 =>	_angleChange is presumed to be in radians

			 void SetScale(Polygon<4, Type>& p, const Point<2, Type>& scale)
			 =>	Scales the PRect by scaling its 4 vertices from the center
			 =>	If the PRect's angle is not 0, SetRotation() will get called to set the angle to 0, after which the scaling happens
			 =>	Once the PRect has been scaled, SetRotation() will get called with the original angle

			 void SetLeftBottom(Polygon<4, Type>& p, const Point<2, Type>& _center)
			 =>	Sets the PRect's leftBottom to the provided Point2
			 =>	If the PRect's angle is no 0, SetRotation() will get called to set the angle to 0,
				  after which all vertices get recalculated using the PRect's width and height
			 =>	If the PRect's scale is not (1, 1), SetScale() will get called to scale the PRect to what it's scale is
			 =>	Once the PRect's leftBottom has been adjusted, all of its vertices recalculated and scaled,
				  SetRotation() will get called with the original angle

			 const Point<2, Type>& GetCenter(const Polygon<4, Type>& p)
			 =>	Returns the PRect's center

			 const Point<2, Type>& GetLeftBottom(const Polygon<4, Type>& p)
			 =>	Returns the PRect's left bottom vertex
			 =>	Note that this merely returns a member variable, this function does not calculate which vertex is the left bottom one

			 const Point<2, Type>& GetLeftTop(const Polygon<4, Type>& p)
			 =>	Returns the PRect's left top vertex
			 =>	Note that this merely returns a member variable, this function does not calculate which vertex is the left top one

			 const Point<2, Type>& GetRightTop(const Polygon<4, Type>& p)
			 =>	Returns the PRect's right top vertex
			 =>	Note that this merely returns a member variable, this function does not calculate which vertex is the right top one

			 const Point<2, Type>& GetRightBottom(const Polygon<4, Type>& p)
			 =>	Returns the PRect's right bottom vertex
			 =>	Note that this merely returns a member variable, this function does not calculate which vertex is the right bottom one

			 SetPivotPoint(Polygon<4, Type>& p, const Point<2, Type>& _pivotPoint)
			 =>	Sets the PRect's pivot point to the provided Point2
			 =>	If the PRect's angle is not 0, SetRotation() will get called

			 const Point<2, Type>& GetPivotPoint(const Polygon<4, Type>& p)
			 =>	Returns the PRect's pivot point

			 void SetWidth(Polygon<4, Type>& p, const Type& _width)
			 =>	Set's the PRect's width to the provided value
			 =>	If the PRect's angle is no 0, SetRotation() will get called to set the angle to 0,
				  after which all vertices's x-values get recalculated using the provided width
			 =>	If the PRect's scale is not (1, 1), SetScale() will get called to scale the PRect to what it's scale is
			 =>	Once the PRect's width has been adjusted, all of its vertices recalculated and scaled,
				  SetRotation() will get called with the original angle

			 void SetHeight(Polygon<4, Type>& p, const Type& _height)
			 =>	Set's the PRect's height to the provided value
			 =>	If the PRect's angle is no 0, SetRotation() will get called to set the angle to 0,
				  after which all vertices's y-values get recalculated using the provided height
			 =>	If the PRect's scale is not (1, 1), SetScale() will get called to scale the PRect to what it's scale is
			 =>	Once the PRect's height has been adjusted, all of its vertices recalculated and scaled,
				  SetRotation() will get called with the original angle
		  */

		  /* The PRect iterator covers the member variables leftBottom, leftTop, rightTop and rightBottom (in that order) */

	template<typename Type>
	struct INTEGRIAN2D_API Polygon<4, Type>
	{
#pragma region Constructors
		explicit Polygon<4, Type>()
			: Polygon<4, Type>{ static_cast<Type>(0.f),static_cast<Type>(0.f) ,static_cast<Type>(0.f) ,static_cast<Type>(0.f) }
		{}
		explicit Polygon<4, Type>(const Type _x, const Type _y, const Type _width, const Type _height)
			: Polygon<4, Type>{ _x, _y, _width, _height, static_cast<Type>(0.f) }
		{}
		explicit Polygon<4, Type>(const Point<2, Type>& _xy, const Type _width, const Type _height)
			: Polygon<4, Type>{ _xy, _width, _height, static_cast<Type>(0.f), static_cast<Type>(1.f), static_cast<Type>(1.f) }
		{}
		explicit Polygon<4, Type>(const Type _x, const Type _y, const Type _width, const Type _height, const Type _angle)
			: Polygon<4, Type>{ _x, _y, _width, _height, _angle, static_cast<Type>(1.f), static_cast<Type>(1.f) }
		{}
		explicit Polygon<4, Type>(const Type _x, const Type _y, const Type _width, const Type _height, const Type _angle, const Type _scaleX, const Type _scaleY)
			: Polygon<4, Type>{ Point<2, Type>{ _x, _y }, _width, _height, _angle, _scaleX, _scaleY }
		{}
		explicit Polygon<4, Type>(const Point<2, Type>& _xy, const Type _width, const Type _height, const Type _angle)
			: Polygon<4, Type>{ _xy, _width, _height, _angle, static_cast<Type>(1.f), static_cast<Type>(1.f) }
		{}
		explicit Polygon<4, Type>(const Point<2, Type>& _xy, const Type _width, const Type _height, const Type _angle, const Type _scaleX, const Type _scaleY)
			: Polygon<4, Type>{ _xy, _width, _height, _angle, Point<2, Type>{ _scaleX, _scaleY } }
		{}
		explicit Polygon<4, Type>(const Point<2, Type>& _xy, const Type _width, const Type _height, const Type _angle, const Point<2, Type>& _scale) noexcept
			: points{}
		{
			const Type halfWidth{ _width * static_cast<Type>(0.5f) };
			const Type halfHeight{ _height * static_cast<Type>(0.5f) };

			points.leftBottom = _xy;
			points.center = { _xy.x + halfWidth, _xy.y + halfHeight };
			points.leftTop = { _xy.x, _xy.y + _height };
			points.rightTop = { _xy.x + _width, _xy.y + _height };
			points.rightBottom = { _xy.x + _width, _xy.y };
			points.pivotPoint = { _xy.x + halfWidth, _xy.y + halfHeight }; // pivot point is in the center by default
			width = _width;
			height = _height;
			scaleX = _scale.x;
			scaleY = _scale.y;
			angle = _angle;

			// == Rotate, but only if the angle is not 0 ==
			if (!Utils::AreEqual(angle, static_cast<Type>(0.f)))
				SetRotation(*this, angle);

			// == Scale, but only if the scale is not 1 ==
			if (!Utils::AreEqual(scaleX, static_cast<Type>(1.f)) || !Utils::AreEqual(scaleY, static_cast<Type>(1.f)))
				SetScale(*this, _scale);
		}
#pragma endregion

#pragma region Rule Of 5
		Polygon<4, Type>(const Polygon<4, Type>& other) noexcept
		{
			points = other.points;
			width = other.width;
			height = other.height;
			scaleX = other.scaleX;
			scaleY = other.scaleY;
			angle = other.angle;
		}
		Polygon<4, Type>(Polygon<4, Type>&& other) noexcept
		{
			points = std::move(other.points);
			width = std::move(other.width);
			height = std::move(other.height);
			scaleX = std::move(other.scaleX);
			scaleY = std::move(other.scaleY);
			angle = std::move(other.angle);
		}
		Polygon<4, Type>& operator=(const Polygon& other) noexcept
		{
			points = other.points;
			width = other.width;
			height = other.height;
			scaleX = other.scaleX;
			scaleY = other.scaleY;
			angle = other.angle;

			return *this;
		}
		Polygon<4, Type>& operator=(Polygon&& other) noexcept
		{
			points = std::move(other.points);
			width = std::move(other.width);
			height = std::move(other.height);
			scaleX = std::move(other.scaleX);
			scaleY = std::move(other.scaleY);
			angle = std::move(other.angle);

			return *this;
		}
#pragma endregion

#pragma region Data
		class INTEGRIAN2D_API Points final
		{
		private:
			friend struct Polygon<4, Type>;

			template<typename Type>
			friend void Translate(Polygon<4, Type>& p, const Vector<2, Type>& v) noexcept;

			template<typename Type>
			friend void Rotate(Polygon<4, Type>& p, const Type& _angleChange) noexcept;

			template<typename Type>
			friend void Scale(Polygon<4, Type>& p, const Point<2, Type>& scale) noexcept;

			template<typename Type>
			friend void SetRotation(Polygon<4, Type>& p, const Type& _angle) noexcept;

			template<typename Type>
			friend void SetScale(Polygon<4, Type>& p, const Point<2, Type>& scale) noexcept;

			template<typename Type>
			friend void SetLeftBottom(Polygon<4, Type>& p, const Point<2, Type>& _center) noexcept;

			template<typename Type>
			friend const Point<2, Type>& GetCenter(const Polygon<4, Type>& p) noexcept;

			template<typename Type>
			friend const Point<2, Type>& GetLeftBottom(const Polygon<4, Type>& p) noexcept;

			template<typename Type>
			friend const Point<2, Type>& GetLeftTop(const Polygon<4, Type>& p) noexcept;

			template<typename Type>
			friend const Point<2, Type>& GetRightTop(const Polygon<4, Type>& p) noexcept;

			template<typename Type>
			friend const Point<2, Type>& GetRightBottom(const Polygon<4, Type>& p) noexcept;

			template<typename Type>
			friend void SetPivotPoint(Polygon<4, Type>& p, const Point<2, Type>& _pivotPoint) noexcept;

			template<typename Type>
			friend const Point<2, Type>& GetPivotPoint(const Polygon<4, Type>& p) noexcept;

			template<typename Type>
			friend void SetWidth(Polygon<4, Type>& p, const Type& _width) noexcept;

			template<typename Type>
			friend void SetHeight(Polygon<4, Type>& p, const Type& _height) noexcept;

			Point<2, Type> center, leftBottom, leftTop, rightTop, rightBottom, pivotPoint;

		public:
			bool operator==(const Points& rhs) const noexcept
			{
				return (leftBottom == rhs.leftBottom) && (leftTop == rhs.leftTop)
					&& (rightTop == rhs.rightTop) && (rightBottom == rhs.rightBottom)
					&& (pivotPoint == rhs.pivotPoint) && (center == rhs.center);
			}

			bool operator!=(const Points& rhs) const noexcept
			{
				return !(*this == rhs);
			}
		};

		Points points;
		Type width, height, scaleX, scaleY, angle;
#pragma endregion

#pragma region Iterator
		Iterator<Point<2, Type>> begin() noexcept
		{
			return Iterator<Point<2, Type>>{ &points.leftBottom };
		}

		ConstIterator<Point<2, Type>> begin() const noexcept
		{
			return ConstIterator<Point<2, Type>>{ &points.leftBottom };
		}

		Iterator<Point<2, Type>> end() noexcept
		{
			return Iterator<Point<2, Type>>{ &points.rightBottom + 1 };
		}

		ConstIterator<Type> end() const noexcept
		{
			return ConstIterator<Point<2, Type>>{ &points.rightBottom + 1 };
		}
#pragma endregion
	};

#pragma region Relational Operators
	template<typename Type>
	bool operator==(const Polygon<4, Type>& lhs, const Polygon<4, Type>& rhs) noexcept
	{
		return (lhs.points == rhs.points) && Utils::AreEqual(lhs.width, rhs.width)
			&& Utils::AreEqual(lhs.height, rhs.height) && Utils::AreEqual(lhs.scaleX, rhs.scaleX)
			&& Utils::AreEqual(lhs.scaleY, rhs.scaleY) && Utils::AreEqual(lhs.angle, rhs.angle);
	}

	template<typename Type>
	bool operator!=(const Polygon<4, Type>& lhs, const Polygon<4, Type>& rhs) noexcept
	{
		return !(lhs == rhs);
	}
#pragma endregion

#pragma region Functions
	template<typename Type>
	void Translate(Polygon<4, Type>& p, const Vector<2, Type>& v) noexcept
	{
		p.points.leftBottom += v;
		p.points.leftTop += v;
		p.points.rightTop += v;
		p.points.rightBottom += v;
		p.points.pivotPoint += v;
		p.points.center += v;
	}

	template<typename Type>
	void Rotate(Polygon<4, Type>& p, const Type& _angleChange) noexcept
	{
		p.angle += _angleChange;

		SetRotation(p, p.angle);
	}

	template<typename Type>
	void Scale(Polygon<4, Type>& p, const Point<2, Type>& scale) noexcept
	{
		p.scaleX += scale.x;
		p.scaleY += scale.y;

		SetScale(p, Point<2, Type>{ p.scaleX, p.scaleY });
	}

	template<typename Type>
	void SetRotation(Polygon<4, Type>& p, const Type& _angle) noexcept
	{
		p.angle = _angle;

		const Type c{ static_cast<Type>(cos(_angle)) };
		const Type s{ static_cast<Type>(sin(_angle)) };
		const Point<2, Type>& pivotPoint{ p.points.pivotPoint };

		// == Translate all points with pivotpoint ==
		p.points.leftBottom -= pivotPoint;
		p.points.leftTop -= pivotPoint;
		p.points.rightTop -= pivotPoint;
		p.points.rightBottom -= pivotPoint;
		p.points.center -= pivotPoint;

		// == Rotate All Points ==
		p.points.leftBottom = { p.points.leftBottom.x * c - p.points.leftBottom.y * s, p.points.leftBottom.x * s + p.points.leftBottom.y * c };
		p.points.leftTop = { p.points.leftTop.x * c - p.points.leftTop.y * s, p.points.leftTop.x * s + p.points.leftTop.y * c };
		p.points.rightTop = { p.points.rightTop.x * c - p.points.rightTop.y * s, p.points.rightTop.x * s + p.points.rightTop.y * c };
		p.points.rightBottom = { p.points.rightBottom.x * c - p.points.rightBottom.y * s, p.points.rightBottom.x * s + p.points.rightBottom.y * c };
		p.points.center = { p.points.center.x * c - p.points.center.y * s, p.points.center.x * s + p.points.center.y * c };

		// == Translate All Points Back ==
		p.points.leftBottom += pivotPoint;
		p.points.leftTop += pivotPoint;
		p.points.rightTop += pivotPoint;
		p.points.rightBottom += pivotPoint;
		p.points.center += pivotPoint;
	}

	template<typename Type>
	void SetScale(Polygon<4, Type>& p, const Point<2, Type>& scale) noexcept
	{
		Type originalAngle{};
		if (!Utils::AreEqual(p.angle, static_cast<Type>(0.f)))
		{
			originalAngle = p.angle;
			SetRotation(p, static_cast<Type>(0.f));
		}

		p.scaleX = scale.x;
		p.scaleY = scale.y;

		const Point<2, Type>& leftBottom{ p.points.leftBottom };

		p.points.leftTop = { leftBottom.x, leftBottom.y + p.height * scale.y };
		p.points.rightTop = { leftBottom.x + p.width * scale.x, leftBottom.y + p.height * scale.y };
		p.points.rightBottom = { leftBottom.x + p.width * scale.x, leftBottom.y };
		p.points.center = { leftBottom.x + (p.width * 0.5f) * scale.x, leftBottom.y + (p.height * 0.5f) * scale.y };

		// == Rotate, but only if the angle is not 0 ==
		if (!Utils::AreEqual(originalAngle, static_cast<Type>(0.f)))
			SetRotation(p, originalAngle);
	}

	template<typename Type>
	void SetLeftBottom(Polygon<4, Type>& p, const Point<2, Type>& _leftBottom) noexcept
	{
		Type originalAngle{};
		if (!Utils::AreEqual(p.angle, static_cast<Type>(0.f)))
		{
			originalAngle = p.angle;
			SetRotation(p, static_cast<Type>(0.f));
		}

		const Type halfWidth{ p.width * static_cast<Type>(0.5f) };
		const Type halfHeight{ p.height * static_cast<Type>(0.5f) };

		p.points.leftBottom = _leftBottom;
		p.points.center = { _leftBottom.x + halfWidth, _leftBottom.y + halfHeight };
		p.points.leftTop = { _leftBottom.x, _leftBottom.y + p.height };
		p.points.rightTop = { _leftBottom.x + p.width, _leftBottom.y + p.height };
		p.points.rightBottom = { _leftBottom.x + p.width, _leftBottom.y };
		p.points.pivotPoint = { _leftBottom.x + halfWidth, _leftBottom.y + halfHeight }; // pivot point is in the center by default

		// == Scale, but only if the scale is not 1 ==
		if (!Utils::AreEqual(p.scaleX, static_cast<Type>(1.f)) || !Utils::AreEqual(p.scaleY, static_cast<Type>(1.f)))
			SetScale(p, Point<2, Type>{ p.scaleX, p.scaleY });

		// == Rotate, but only if the angle is not 0 ==
		if (!Utils::AreEqual(originalAngle, static_cast<Type>(0.f)))
			SetRotation(p, originalAngle);
	}

	template<typename Type>
	const Point<2, Type>& GetCenter(const Polygon<4, Type>& p) noexcept
	{
		return p.points.center;
	}

	template<typename Type>
	const Point<2, Type>& GetLeftBottom(const Polygon<4, Type>& p) noexcept
	{
		return p.points.leftBottom;
	}

	template<typename Type>
	const Point<2, Type>& GetLeftTop(const Polygon<4, Type>& p) noexcept
	{
		return p.points.leftTop;
	}

	template<typename Type>
	const Point<2, Type>& GetRightTop(const Polygon<4, Type>& p) noexcept
	{
		return p.points.rightTop;
	}

	template<typename Type>
	const Point<2, Type>& GetRightBottom(const Polygon<4, Type>& p) noexcept
	{
		return p.points.rightBottom;
	}

	template<typename Type>
	void SetPivotPoint(Polygon<4, Type>& p, const Point<2, Type> _pivotPoint) noexcept
	{
		Type originalAngle{};
		if (!Utils::AreEqual(p.angle, static_cast<Type>(0.f)))
		{
			originalAngle = p.angle;
			SetRotation(p, static_cast<Type>(0.f));
		}

		p.points.pivotPoint = _pivotPoint;

		// == Rotate, but only if the angle is not 0 ==
		if (!Utils::AreEqual(originalAngle, static_cast<Type>(0.f)))
			SetRotation(p, originalAngle);
	}

	template<typename Type>
	const Point<2, Type>& GetPivotPoint(const Polygon<4, Type>& p) noexcept
	{
		return p.points.pivotPoint;
	}

	template<typename Type>
	void SetWidth(Polygon<4, Type>& p, const Type& _width) noexcept
	{
		Type originalAngle{};
		if (!Utils::AreEqual(p.angle, static_cast<Type>(0.f)))
		{
			originalAngle = p.angle;
			SetRotation(p, static_cast<Type>(0.f));
		}

		p.width = _width;

		const Point<2, Type>& leftBottom{ p.points.leftBottom };

		p.points.rightTop = { leftBottom.x + _width * p.scaleX, p.points.rightTop.y };
		p.points.rightBottom = { leftBottom.x + _width * p.scaleX, p.points.rightBottom.y };
		p.points.center = { leftBottom.x + (_width * 0.5f) * p.scaleX, p.points.center.y };

		// == Scale, but only if the scale is not 1 ==
		if (!Utils::AreEqual(p.scaleX, static_cast<Type>(1.f)) || !Utils::AreEqual(p.scaleY, static_cast<Type>(1.f)))
			SetScale(p, Point<2, Type>{ p.scaleX, p.scaleY });

		// == Rotate, but only if the angle is not 0 ==
		if (!Utils::AreEqual(originalAngle, static_cast<Type>(0.f)))
			SetRotation(p, originalAngle);
	}

	template<typename Type>
	void SetHeight(Polygon<4, Type>& p, const Type& _height) noexcept
	{
		Type originalAngle{};
		if (!Utils::AreEqual(p.angle, static_cast<Type>(0.f)))
		{
			originalAngle = p.angle;
			SetRotation(p, static_cast<Type>(0.f));
		}

		p.height = _height;

		const Point<2, Type>& leftBottom{ p.points.leftBottom };

		p.points.leftTop.y = leftBottom.y + _height;
		p.points.rightTop.y = leftBottom.y + _height;
		p.points.center.y = leftBottom.y + _height * static_cast<Type>(0.5f);

		// == Scale, but only if the scale is not 1 ==
		if (!Utils::AreEqual(p.scaleX, static_cast<Type>(1.f)) || !Utils::AreEqual(p.scaleY, static_cast<Type>(1.f)))
			SetScale(p, Point<2, Type>{ p.scaleX, p.scaleY });

		// == Rotate, but only if the angle is not 0 ==
		if (!Utils::AreEqual(originalAngle, static_cast<Type>(0.f)))
			SetRotation(p, originalAngle);
	}
#pragma endregion
}