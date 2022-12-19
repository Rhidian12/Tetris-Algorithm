#pragma once

#include "../Integrian2D_API.h"

#include <stdint.h>

namespace Integrian2D
{
	struct INTEGRIAN2D_API Betteruint8_t final /* for some strange reason, doing uint8_t + uint8_t returned an integer, so I made this struct to circumvent that issue */
	{
		explicit Betteruint8_t(const uint8_t _value)
			: v{ _value }
		{}

		uint8_t v;

		operator uint8_t()
		{
			return v;
		}
	};

	inline std::ostream& operator<<(std::ostream& os, const Betteruint8_t& b) noexcept
	{
		return os << b.v;
	}

	inline bool operator==(const Betteruint8_t& a, const Betteruint8_t& b) noexcept
	{
		return a.v == b.v;
	}

	inline bool operator!=(const Betteruint8_t& a, const Betteruint8_t& b) noexcept
	{
		return a.v != b.v;
	}

	inline Betteruint8_t operator+(const Betteruint8_t& a, const Betteruint8_t& b) noexcept
	{
		return Betteruint8_t(static_cast<uint8_t>(a.v + b.v));
	}

	inline Betteruint8_t& operator+=(Betteruint8_t& a, const Betteruint8_t& b) noexcept
	{
		static_cast<uint8_t>(a.v += b.v);
		return a;
	}

	inline Betteruint8_t operator+(const Betteruint8_t& a, const uint8_t b) noexcept
	{
		return Betteruint8_t(static_cast<uint8_t>(a.v + b));
	}

	inline Betteruint8_t& operator+=(Betteruint8_t& a, const uint8_t b) noexcept
	{
		static_cast<uint8_t>(a.v += b);
		return a;
	}

	inline Betteruint8_t operator-(const Betteruint8_t& a, const Betteruint8_t& b) noexcept
	{
		return Betteruint8_t(static_cast<uint8_t>(a.v - b.v));
	}

	inline Betteruint8_t& operator-=(Betteruint8_t& a, const Betteruint8_t& b) noexcept
	{
		static_cast<uint8_t>(a.v -= b.v);
		return a;
	}

	inline Betteruint8_t operator-(const Betteruint8_t& a, const uint8_t b) noexcept
	{
		return Betteruint8_t(static_cast<uint8_t>(a.v - b));
	}

	inline Betteruint8_t& operator-=(Betteruint8_t& a, const uint8_t b) noexcept
	{
		static_cast<uint8_t>(a.v -= b);
		return a;
	}

	inline Betteruint8_t operator*(const Betteruint8_t& a, const Betteruint8_t& b) noexcept
	{
		return Betteruint8_t(static_cast<uint8_t>(a.v * b.v));
	}

	inline Betteruint8_t& operator*=(Betteruint8_t& a, const Betteruint8_t& b) noexcept
	{
		static_cast<uint8_t>(a.v *= b.v);
		return a;
	}

	inline Betteruint8_t operator*(const Betteruint8_t& a, const uint8_t b) noexcept
	{
		return Betteruint8_t(static_cast<uint8_t>(a.v * b));
	}

	inline Betteruint8_t& operator*=(Betteruint8_t& a, const uint8_t b) noexcept
	{
		static_cast<uint8_t>(a.v *= b);
		return a;
	}

	inline Betteruint8_t operator/(const Betteruint8_t& a, const Betteruint8_t& b) noexcept
	{
		return Betteruint8_t(static_cast<uint8_t>(a.v / b.v));
	}

	inline Betteruint8_t& operator/=(Betteruint8_t& a, const Betteruint8_t& b) noexcept
	{
		static_cast<uint8_t>(a.v /= b.v);
		return a;
	}

	inline Betteruint8_t operator/(const Betteruint8_t& a, const uint8_t b) noexcept
	{
		return Betteruint8_t(static_cast<uint8_t>(a.v / b));
	}

	inline Betteruint8_t& operator/=(Betteruint8_t& a, const uint8_t b) noexcept
	{
		static_cast<uint8_t>(a.v /= b);
		return a;
	}
}