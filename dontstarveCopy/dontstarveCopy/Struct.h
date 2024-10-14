#pragma once

struct Vec2D
{
	float x;
	float y;

	Vec2D() : x(0.f), y(0.f){}

	Vec2D(int x, int y) : x(float(x)), y(float(y)) {}

	Vec2D(float x, float y) : x(x), y(y) {}
};