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

	constexpr Point& operator+=(const Point& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}
	constexpr Point& operator-=(const Point& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}
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

inline constexpr Point operator+(const Point& lhs, const Point& rhs)
{
	return Point{ lhs.x + rhs.x,lhs.y + rhs.y };
}
inline constexpr Point operator-(const Point& lhs, const Point& rhs)
{
	return Point{ lhs.x - rhs.x,lhs.y - rhs.y };
}