#pragma once

#include <cstdio>
#include "surface.h"
#include "DrawableObject.h"
#include <iostream>
#include <vector>

class Entity : public DrawableObject
{
public:
	Entity(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy) :DrawableObject(sprite, px, py, sx, sy) {}
	virtual ~Entity() {}

	void SetPosition(Vec2i v) { px = v.x, py = v.y; }
	void SetPosition(int x, int y) { px = x, py = y; }
protected:

};

