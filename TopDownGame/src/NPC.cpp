#include "NPC.h"
#include "DialogueSystem.h"

NPC::NPC(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy, int neededLevel) : InteractableObject(sprite, px, py, sx, sy), neededLevel(neededLevel)
{
}

NPC::~NPC()
{
}

void NPC::update(float deltaTime)
{
	if (neededLevel != Map::currentLevel)
		return;
}

void NPC::render(Tmpl8::Surface* screen) 
{
	if (neededLevel != Map::currentLevel)
		return;

	sprite->DrawScaled(px, py, sx, sy, screen);
}

void NPC::giveDialogue(Dialogue* d)
{
	dialogueQueue.push(d);
}
