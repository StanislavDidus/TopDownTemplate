#include "NPC.h"

NPC::NPC(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy, int neededLevel, int& currentLevel) : Entity(sprite, px, py, sx, sy), neededLevel(neededLevel),
currentLevel(currentLevel)
{
}

NPC::~NPC()
{
}

void NPC::update(float deltaTime)
{
	if (neededLevel != currentLevel)
		return;
}

void NPC::render(Tmpl8::Surface* screen) 
{
	if (neededLevel != currentLevel)
		return;

	sprite->DrawScaled(px, py, sx, sy, screen);
}
