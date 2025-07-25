#pragma once

#include <cstdio>
#include "surface.h"
#include <set>
#include "DrawableObject.h"
#include <iostream>
#include <vector>

class InteractableObject : public DrawableObject
{
public:
	InteractableObject(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy);
	virtual ~InteractableObject();

	static const std::set<InteractableObject*>& GetAllInteractables();
private:
	static std::set<InteractableObject*> allInteractables;
};