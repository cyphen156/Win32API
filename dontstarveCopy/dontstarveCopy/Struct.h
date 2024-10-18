#pragma once
#include "assert.h"

struct Vec2D
{
	float x;
	float y;

public:
	float Length()
	{
		return sqrt(x * x + y * y);
	}

	Vec2D& Normalize()
	{
		float fLen = Length();

		assert(fLen != 0.f);

		x /= fLen;
		y /= fLen;

		return *this;
	}

	Vec2D& operator = (const POINT& pt)
	{
		x = (float)pt.x;
		y = (float)pt.y;
	}

	Vec2D operator + (Vec2D vOther)
	{
		return Vec2D(x + vOther.x, y + vOther.y);
	}

	Vec2D operator - (Vec2D vOther)
	{
		return Vec2D(x - vOther.x, y - vOther.y);
	}
	Vec2D operator * (Vec2D vOther)
	{
		return Vec2D(x * vOther.x, y * vOther.y);
	}
	Vec2D operator / (Vec2D vOther)
	{
		assert(!(0.f == vOther.x || 0.f == vOther.y));
		return Vec2D(x / vOther.x, y / vOther.y);
	}

	Vec2D() : x(0.f), y(0.f){}

	Vec2D(int x, int y) : x(float(x)), y(float(y)) {}

	Vec2D(float x, float y) : x(x), y(y) {}

	Vec2D(POINT pt) : x((float)pt.x), y((float)pt.y) {}
};