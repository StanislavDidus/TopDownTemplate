#pragma once

#include <cstdio>
#include "surface.h"
#include "DrawableObject.h"
#include <iostream>
#include <vector>


class UIElement : public DrawableObject
{
public:
	UIElement(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy) :DrawableObject(sprite, px, py, sx, sy) {}
	virtual ~UIElement() {}
protected:

};
