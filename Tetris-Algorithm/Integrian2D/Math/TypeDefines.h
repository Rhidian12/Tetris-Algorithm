#pragma once

#include "Point.h"
#include "Point2.h"
#include "Point3.h"

#include "Vector.h"
#include "Vector2.h"

#include "Rect.h"
#include "Triangle.h"
#include "Circle.h"

#include "Polygon.h"
#include "PLine.h"
#include "PTriangle.h"
#include "PRect.h"

#include "RGBColour.h"

#include "Matrix.h"

namespace Integrian2D
{
	typedef Point<2, float> Point2f;
	typedef Point<3, float> Point3f;

	typedef Vector<2, float> Vector2f;
	typedef Vector<3, float> Vector3f;

	typedef Rect<float> Rectf;
	typedef Triangle<float> Trianglef;
	typedef Circle<float> Circlef;

	typedef Polygon<2, float> PLinef;
	typedef Polygon<3, float> PTrianglef;
	typedef Polygon<4, float> PRectf;

	typedef Matrix<2, 2, float> Matrix2x2;
	typedef Matrix<3, 3, float> Matrix3x3;
}

