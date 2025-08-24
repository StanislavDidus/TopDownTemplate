#pragma once

#include <cstdio>
#include "Vector.h"
#include "surface.h"
#include <iostream>
#include <vector>

class DrawableObject
{
public:
	DrawableObject(Tmpl8::Sprite* sprite, float px, float py, int sx, int sy) : sprite(sprite), px(px), py(py), sx(sx), sy(sy), flipped(false) {}
	virtual ~DrawableObject() {}

	virtual void update(float deltaTime) {}
	virtual void render(Tmpl8::Surface* screen) { if(sprite != nullptr) sprite->DrawScaled(static_cast<int>(px), static_cast<int>(py), sx, sy, screen, flipped); }

	Vec2f GetPosition() { return { px, py }; }
	Vec2i GetSize() { return { sx, sy }; }
protected:
	float px, py;
	int sx, sy;
	Tmpl8::Sprite* sprite;
	bool flipped;
};
