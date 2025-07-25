#include "InteractableObject.h"

std::set<InteractableObject*> InteractableObject::allInteractables;

InteractableObject::InteractableObject(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy) : DrawableObject(sprite, px, py, sx, sy)
{
	allInteractables.insert(this);
}

InteractableObject::~InteractableObject()
{
	allInteractables.erase(this);
}

const std::set<InteractableObject*>& InteractableObject::GetAllInteractables()
{
	return allInteractables;
}