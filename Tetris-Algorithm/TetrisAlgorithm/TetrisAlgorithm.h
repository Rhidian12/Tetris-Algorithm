#pragma once

#include "../Point/Point.h"

class TetrisAlgorithm final
{
public:
	
private:
	void SendMousePress(const Point& coords);

	inline constexpr static Point g_LeftCoords{ 84L, 252L };
	inline constexpr static Point g_DownCoords{ 111L, 306L };
	inline constexpr static Point g_UpCoords{ 111L, 239L };
	inline constexpr static Point g_RightCoords{ 138L, 252L };
};