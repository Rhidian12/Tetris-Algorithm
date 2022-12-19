#pragma once

#include "../Integrian2D_API.h"
#include <stdint.h>
#include <utility>
#include <algorithm>
#include <string>

#include "../Utils/Utils.h"
#include "BetterUint8.h"

#ifdef max /* This macro was destroying the entire file */
#undef max
#endif

namespace Integrian2D
{
	/* This class represents a RGB colour, note that the rgb components range from [0, 255] and the a component ranges from [0, 1] */

	/* A list of available operators:

	   RGBColour operator+(const RGBColour& cOne, const RGBColour& cTwo)
	   RGBColour operator+(const RGBColour& cOne, const uint8_t inc)

	   RGBColour operator-(const RGBColour& cOne, const RGBColour& cTwo)
	   RGBColour operator-(const RGBColour& cOne, const uint8_t inc)

	   RGBColour operator*(const RGBColour& cOne, const RGBColour& cTwo)
	   RGBColour operator*(const RGBColour& cOne, const uint8_t inc)

	   RGBColour operator/(const RGBColour& cOne, const RGBColour& cTwo)
	   RGBColour operator/(const RGBColour& cOne, const uint8_t inc)

	   RGBColour& operator+=(RGBColour& cOne, const RGBColour& cTwo)
	   RGBColour& operator+=(RGBColour& cOne, const uint8_t inc)

	   RGBColour& operator-=(RGBColour& cOne, const RGBColour& cTwo)
	   RGBColour& operator-=(RGBColour& cOne, const uint8_t inc)

	   RGBColour& operator*=(RGBColour& cOne, const RGBColour& cTwo)
	   RGBColour& operator*=(RGBColour& cOne, const uint8_t inc)

	   RGBColour& operator/=(RGBColour& cOne, const RGBColour& cTwo)
	   RGBColour& operator/=(RGBColour& cOne, const uint8_t inc)
	   */

	   /* A list of available functions:

		 void MaxToOne(RGBColour& colour)
		 =>		Finds the maximum value of the rgb components, then divides every component by that max value
		 =>		This function essentially normalizes the colour

		 std::string ToString(const RGBColour&)
		 =>		Returns the rgba values of the provided colour in a CSV manner
		*/

	struct INTEGRIAN2D_API RGBColour final
	{
#pragma region Constructors
		explicit RGBColour()
			: RGBColour{ 0,0,0,1 }
		{}
		explicit RGBColour(const uint8_t _r, const uint8_t _g, const uint8_t _b)
			: RGBColour{ _r,_g,_b,1 }
		{}
		explicit RGBColour(const uint8_t _r, const uint8_t _g, const uint8_t _b, const uint8_t _a)
			: r{ _r }
			, g{ _g }
			, b{ _b }
			, a{ _a }
		{
			// r, g, b dont need a clamp since their natural range is [0, 255]
			Utils::Clamp(a.v, static_cast<uint8_t>(0), static_cast<uint8_t>(1));
		}
#pragma endregion

#pragma region Rule of 5
		RGBColour(const RGBColour& other) noexcept
			: r{ other.r }
			, g{ other.g }
			, b{ other.b }
			, a{ other.a }
		{}
		RGBColour(RGBColour&& other) noexcept
			: r{ std::move(other.r) }
			, g{ std::move(other.g) }
			, b{ std::move(other.b) }
			, a{ std::move(other.a) }
		{}
		RGBColour& operator=(const RGBColour& other) noexcept
		{
			r = other.r;
			g = other.g;
			b = other.b;
			a = other.a;

			return *this;
		}
		RGBColour& operator=(RGBColour&& other) noexcept
		{
			r = std::move(other.r);
			g = std::move(other.g);
			b = std::move(other.b);
			a = std::move(other.a);

			return *this;
		}
#pragma endregion

#pragma region Data
		Betteruint8_t r, g, b, a; // uint8_t has a range of [0, 255]
#pragma endregion
	};

#pragma region Arithmetic Operators
	inline RGBColour operator+(const RGBColour& cOne, const RGBColour& cTwo) noexcept
	{
		return RGBColour{ cOne.r + cTwo.r, cOne.b + cTwo.b, cOne.g + cTwo.g, cOne.a + cTwo.a };
	}

	inline RGBColour operator+(const RGBColour& cOne, const uint8_t inc) noexcept
	{
		return RGBColour{ cOne.r + inc, cOne.b + inc, cOne.g + inc, cOne.a + inc };
	}

	inline RGBColour operator-(const RGBColour& cOne, const RGBColour& cTwo) noexcept
	{
		return RGBColour{ cOne.r - cTwo.r, cOne.b - cTwo.b, cOne.g - cTwo.g, cOne.a - cTwo.a };
	}

	inline RGBColour operator-(const RGBColour& cOne, const uint8_t inc) noexcept
	{
		return RGBColour{ cOne.r - inc, cOne.b - inc, cOne.g - inc, cOne.a - inc };
	}

	inline RGBColour operator*(const RGBColour& cOne, const RGBColour& cTwo) noexcept
	{
		return RGBColour{ cOne.r * cTwo.r, cOne.b * cTwo.b, cOne.g * cTwo.g, cOne.a * cTwo.a };
	}

	inline RGBColour operator*(const RGBColour& cOne, const uint8_t inc) noexcept
	{
		return RGBColour{ cOne.r * inc, cOne.b * inc, cOne.g * inc, cOne.a * inc };
	}

	inline RGBColour operator/(const RGBColour& cOne, const RGBColour& cTwo) noexcept
	{
		return RGBColour{ cOne.r / cTwo.r, cOne.b / cTwo.b, cOne.g / cTwo.g, cOne.a / cTwo.a };
	}

	inline RGBColour operator/(const RGBColour& cOne, const uint8_t inc) noexcept
	{
		return RGBColour{ cOne.r / inc, cOne.b / inc, cOne.g / inc, cOne.a / inc };
	}
#pragma endregion

#pragma region Compound Arithmetic Operators
	inline RGBColour& operator+=(RGBColour& cOne, const RGBColour& cTwo) noexcept
	{
		cOne.r += cTwo.r;
		cOne.g += cTwo.g;
		cOne.b += cTwo.b;
		cOne.a += cTwo.a;

		return cOne;
	}

	inline RGBColour& operator+=(RGBColour& cOne, const uint8_t inc) noexcept
	{
		cOne.r += inc;
		cOne.g += inc;
		cOne.b += inc;
		cOne.a += inc;

		return cOne;
	}

	inline RGBColour& operator-=(RGBColour& cOne, const RGBColour& cTwo) noexcept
	{
		cOne.r -= cTwo.r;
		cOne.g -= cTwo.g;
		cOne.b -= cTwo.b;
		cOne.a -= cTwo.a;

		return cOne;
	}

	inline RGBColour& operator-=(RGBColour& cOne, const uint8_t inc) noexcept
	{
		cOne.r -= inc;
		cOne.g -= inc;
		cOne.b -= inc;
		cOne.a -= inc;

		return cOne;
	}

	inline RGBColour& operator*(RGBColour& cOne, const RGBColour& cTwo) noexcept
	{
		cOne.r *= cTwo.r;
		cOne.g *= cTwo.g;
		cOne.b *= cTwo.b;
		cOne.a *= cTwo.a;

		return cOne;
	}

	inline RGBColour& operator*=(RGBColour& cOne, const uint8_t inc) noexcept
	{
		cOne.r *= inc;
		cOne.g *= inc;
		cOne.b *= inc;
		cOne.a *= inc;

		return cOne;
	}

	inline RGBColour& operator/=(RGBColour& cOne, const RGBColour& cTwo) noexcept
	{
		cOne.r /= cTwo.r;
		cOne.g /= cTwo.g;
		cOne.b /= cTwo.b;
		cOne.a /= cTwo.a;

		return cOne;
	}

	inline RGBColour& operator/=(RGBColour& cOne, const uint8_t inc) noexcept
	{
		cOne.r /= inc;
		cOne.g /= inc;
		cOne.b /= inc;
		cOne.a /= inc;

		return cOne;
	}
#pragma endregion

#pragma region Relational Operators
	inline bool operator==(const RGBColour& c1, const RGBColour& c2) noexcept
	{
		return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a;
	}

	inline bool operator!=(const RGBColour& c1, const RGBColour& c2) noexcept
	{
		return !(c1 == c2);
	}
#pragma endregion

#pragma region Miscellaneous Operators
	inline std::ostream& operator<<(std::ostream& os, const RGBColour& c) noexcept
	{
		return os << c.r << ", " << c.g << ", " << c.b << ", " << c.a << std::endl;
	}
#pragma endregion

#pragma region RGBColour Functions
	inline void MaxToOne(RGBColour& colour) noexcept
	{
		const uint8_t max{ std::max(colour.r.v, std::max(colour.g.v, colour.b.v)) };

		colour /= max;
	}

	inline std::string ToString(const RGBColour& colour) noexcept
	{
		return std::to_string(colour.r.v) + ", " + std::to_string(colour.g.v) + ", " + std::to_string(colour.b.v) + ", " + std::to_string(colour.a.v);
	}
#pragma endregion

#pragma region Static Variables
	namespace Colours
	{
		const static RGBColour White{ 255u, 255u, 255u };

		const static RGBColour Black{ 0u, 0u, 0u };
		const static RGBColour Grey{ 128u, 128u, 128u };

		const static RGBColour Red{ 255u, 0u, 0u };
		const static RGBColour DarkRed{ 139u, 0u, 0u };
		const static RGBColour LightRed{ 255u, 204u, 203u };
		const static RGBColour Crimson{ 220u, 20u, 60u };
		const static RGBColour Salmon{ 250u, 128u, 114u };

		const static RGBColour Green{ 0u, 255u, 0u };
		const static RGBColour DarkGreen{ 0u, 128u, 0u };
		const static RGBColour LightGreen{ 127u, 255u, 0u };
		const static RGBColour Olive{ 128u, 128u, 0u };
		const static RGBColour LightSeaGreen{ 32u, 128u, 0u };

		const static RGBColour Blue{ 0u, 0u, 255u };
		const static RGBColour LightBlue{ 0u, 191u, 255u };
		const static RGBColour DarkBlue{ 0u, 0u, 139u };
		const static RGBColour Cyan{ 0u, 255u, 255u };
		const static RGBColour Navy{ 0u, 0u, 128u };

		const static RGBColour Orange{ 255u, 165u, 0u };
		const static RGBColour DarkOrange{ 255u, 140u, 0u };

		const static RGBColour Purple{ 128u, 0u, 128u };
		const static RGBColour Indigo{ 75u, 0u, 130u };
		const static RGBColour Violet{ 238u, 130u, 238u };
		const static RGBColour DarkViolet{ 148u, 0u, 211u };
		const static RGBColour Fuchsia{ 255u, 0u, 255u };

		const static RGBColour Yellow{ 255u, 255u, 0u };
		const static RGBColour LightYellow{ 255u, 255u, 224u };
		const static RGBColour Khaki{ 240u, 230u, 140u };
		const static RGBColour DarkKhaki{ 189u, 183u, 107u };
		const static RGBColour Gold{ 212u, 175u, 55u };

		const static RGBColour Pink{ 255u, 192u, 203u };
		const static RGBColour LightPink{ 255u, 182u, 193u };
		const static RGBColour HotPink{ 255u, 105u, 180u };
		const static RGBColour DeepPink{ 255u, 20u, 147u };
	}
#pragma endregion
}