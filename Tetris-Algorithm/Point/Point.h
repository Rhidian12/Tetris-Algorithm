#pragma once

struct Point final
{
	constexpr Point()
		: x{}
		, y{}
	{}

	constexpr Point(const long _x, const long _y)
		: x{ _x }
		, y{ _y }
	{}

	constexpr bool operator==(const Point& other)
	{
		return x == other.x && y == other.y;
	}
	constexpr bool operator!=(const Point& other)
	{
		return !(*this == other);
	}

	long x;
	long y;
};