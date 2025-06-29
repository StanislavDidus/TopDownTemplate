#pragma once

#include <cstdio>
#include "Vector.h"
#include "surface.h"
#include <iostream>
#include <vector>

class DrawableObject
{
public:
	DrawableObject(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy) : sprite(sprite), px(px), py(py), sx(sx), sy(sy) {}
	virtual ~DrawableObject() {}

	virtual void update(float deltaTime) {}
	virtual void render(Tmpl8::Surface* screen) const { sprite->DrawScaled(px, py, sx, sy, screen); }

	Vec2i GetPosition() { return { px, py }; }
	Vec2i GetSize() { return { sx, sy }; }
protected:
	int px, py, sx, sy;
	Tmpl8::Sprite* sprite;
};
