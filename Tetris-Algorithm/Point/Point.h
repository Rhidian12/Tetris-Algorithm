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

	long x;
	long y;
};