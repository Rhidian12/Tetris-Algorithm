#pragma once

#include "../Integrian2D_API.h"

#include <type_traits>
#include <string>
#include <limits>
#include <cmath>

namespace Integrian2D
{
	namespace Utils
	{
		/* I suggest using the ASSERT macro, since it gets optimised away in Release */
		INTEGRIAN2D_API void Assert(const bool expression, const std::string& message);

#define ASSERT(expression, message) Utils::Assert(expression, message);

		inline volatile constexpr double PI{ 3.14159265358979323846264338327950288 };

		// == Templated Functions ==

		/* This function deletes a pointer - if it is not a nullptr - and sets it to a nullptr */
		template<typename Pointer>
		void SafeDelete(Pointer*& pData)
		{
			if (pData)
			{
				delete pData;
				pData = nullptr;
			}
		}

		/* Square any value */
		template<typename Type>
		constexpr Type Square(const Type value) noexcept
		{
			return value * value;
		}

		/* Calculate a random number, but only with Fundamental type, such as integers, floats, doubles, ... */
		template<typename FundamentalType, typename = std::enable_if_t<std::is_fundamental_v<FundamentalType>>>
		constexpr FundamentalType RandomNumber(const FundamentalType min, const FundamentalType max)
		{
			if constexpr (std::is_floating_point_v<FundamentalType>)
			{
				// https://stackoverflow.com/questions/686353/random-float-number-generation
				return min + static_cast<FundamentalType>(rand()) / (static_cast<FundamentalType>(RAND_MAX / (max - min)));
			}
			else
			{
				return static_cast<FundamentalType>(rand()) % max + min;
			}
		}

		/* This function clamps a value between the min and max 
		   If the value is smaller than min, it gets set to min 
		   If the value is bigger than max, it gets set to max*/
		template<typename Type>
		constexpr void Clamp(Type& value, const Type& min, const Type& max) noexcept
		{
			if (value < min)
				value = min;

			if (value > max)
				value = max;
		}

		/* This function checks if 2 fundamental types are equal */
		template<typename FundamentalType, typename = std::enable_if_t<std::is_fundamental_v<FundamentalType>>>
		constexpr bool AreEqual(const FundamentalType a, const FundamentalType b)
		{
			return abs(a - b) <= std::numeric_limits<FundamentalType>::epsilon();
		}

		/* This function converts a floating point value from degrees to radians */
		template<typename FloatingPoint, typename = std::enable_if_t<std::is_floating_point_v<FloatingPoint>>>
		constexpr FloatingPoint ToRadians(const FloatingPoint degrees) noexcept
		{
			return static_cast<FloatingPoint>(degrees * PI / 180.f);
		}

		/* This function converts a floating point value from radians to degrees */
		template<typename FloatingPoint, typename = std::enable_if_t<std::is_floating_point_v<FloatingPoint>>>
		constexpr FloatingPoint ToDegrees(const FloatingPoint radians) noexcept
		{
			return static_cast<FloatingPoint>(radians * 180.0 / PI);
		}

		/* This function rounds a floating point its fraction to zero, if the fraction is smaller than the epsilon 
		   For example:
		   RoundToZero(5.3f, 0.1f) => 0.3f is not smaller than 0.1f, thus the function returns 5.3f
		   RoundToZero(5.3f) => 0.3f is not smaller than 0.00001f (default value), thus the function returns 5.3f 
		   RoundToZero(5.3f, 0.5f) => 0.3f is smaller than 0.5f, thus the function returns 5.f */
		template<typename FloatingPoint, typename = std::enable_if_t<std::is_floating_point_v<FloatingPoint>>>
		constexpr FloatingPoint RoundToZero(FloatingPoint value, const FloatingPoint epsilon = static_cast<FloatingPoint>(1e-5f) /* == 0.00001 */) noexcept
		{
			FloatingPoint fraction{};
			FloatingPoint roundedDownValue{ std::modf(value, &fraction) };

			if (fraction <= epsilon)
				return roundedDownValue;

			return value;
		}

		/* This function rounds a float point up, if the fraction is not zero 
		   For example:
		   RoundDecimalUp(0.5f) => 0.5f is not 0.f, thus the function returns 1.f
		   RoundDecimalUp(1.f) => 0.f is 0.f, thus the function returns 1.f 
		   RoundDecimalUp(1.6f) => 0.6f is not 0.f, thus the function returns 2.f */
		template<typename FloatingPoint, typename = std::enable_if_t<std::is_floating_point_v<FloatingPoint>>>
		constexpr FloatingPoint RoundDecimalUp(FloatingPoint value) noexcept
		{
			if (static_cast<int>(value) != value)
				value = static_cast<FloatingPoint>(static_cast<int>(++value));

			return value;
		}

		/* This function rounds a float point down, if the fraction is not zero
		   For example:
		   RoundDecimalUp(0.5f) => 0.5f is not 0.f, thus the function returns 0.f
		   RoundDecimalUp(1.f) => 0.f is 0.f, thus the function returns 1.f
		   RoundDecimalUp(1.6f) => 0.6f is not 0.f, thus the function returns 0.f */
		template<typename FloatingPoint, typename = std::enable_if_t<std::is_floating_point_v<FloatingPoint>>>
		constexpr FloatingPoint RoundDecimalDown(FloatingPoint value) noexcept
		{
			if (static_cast<int>(value) != value)
				value = static_cast<FloatingPoint>(static_cast<int>(--value));

			return value;
		}

		/* This function checks whether the value is within the range ]min, max[ or [min, max] 
		   The range depends on the boolean rangeLimitsInclusive, which is set to true by default */
		template<typename Type>
		constexpr bool IsInRange(const Type value, const Type min, const Type max, const bool rangeLimitsInclusive = true) noexcept
		{
			if (rangeLimitsInclusive)
			{
				if (value >= min && value <= max)
					return true;
				else
					return false;
			}
			else
			{
				if (value > min && value < max)
					return true;
				else
					return false;
			}
		}
		
		/* This function takes an associative STL container - such as std::map, std::set and their respective unordered versions - 
		   a unary predicate with a parameter of const std::pair<Key, Value>& 
		   Any pair that meets the requirements of the predicate get erased from the associative container */
		template<typename Container, typename Predicate, typename key = typename Container::key_type, typename value = typename Container::mapped_type,
			typename = std::enable_if_t<std::is_invocable_r_v<bool, Predicate, const std::pair<key, value>&>>>
		constexpr void Erase_If(Container& items, const Predicate& predicate)
		{
			for (auto it{ items.begin() }; it != items.end();)
			{
				if (predicate(*it))
					it = items.erase(it);
				else
					++it;
			}
		}
	}
}

