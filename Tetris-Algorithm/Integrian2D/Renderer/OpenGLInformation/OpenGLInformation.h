#pragma once

#include "../../Math/TypeDefines.h"

#include <vector>

namespace Integrian2D
{
	enum class OpenGLShape
	{
		Rectangle,
		Triangle,
		Line,
		Circle,
		Polygon
	};

	template<OpenGLShape Shape>
	struct OpenGLInformation final
	{
		OpenGLInformation() = default;
		~OpenGLInformation() = default;

#pragma region AlwaysPresent
		RGBColour colour{ 255,0,0 };
		bool isFilled{};
#pragma endregion

#pragma region SFINAE
		template<OpenGLShape _Shape = Shape>
		struct Type
		{};

		template<>
		struct Type<OpenGLShape::Rectangle>
		{
			PRectf rectangle{};
		};

		template<>
		struct Type<OpenGLShape::Triangle>
		{
			PTrianglef triangle{};
		};

		template<>
		struct Type<OpenGLShape::Line>
		{
			PLinef line{};
			float lineWidth{};
		};

		template<>
		struct Type<OpenGLShape::Circle>
		{
			Circlef circle{};
		};

		template<>
		struct Type<OpenGLShape::Polygon>
		{
			std::vector<Point2f> points{};
			float lineWidth{};
		};
#pragma endregion

		OpenGLShape shape{ Shape };
		Type<Shape> shapeSpecificData{};
	};

	using OpenGLRectangle	= OpenGLInformation<OpenGLShape::Rectangle>;
	using OpenGLTriangle	= OpenGLInformation<OpenGLShape::Triangle>;
	using OpenGLLine		= OpenGLInformation<OpenGLShape::Line>;
	using OpenGLCircle		= OpenGLInformation<OpenGLShape::Circle>;
	using OpenGLPolygon		= OpenGLInformation<OpenGLShape::Polygon>;
}