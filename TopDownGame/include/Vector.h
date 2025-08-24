#pragma once

struct Vec2i
{
	Vec2i() : x(0), y(0) {}
	Vec2i(int x, int y) : x(x), y(y) {}
	int x, y;
};

struct Vec2f
{
	Vec2f() : x(0), y(0) {}
	Vec2f(float x, float y) : x(x), y(y) {}
	float x, y;
};

