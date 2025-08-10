#pragma once

#include <cstdio>
#include "InteractableObject.h"
#include "Vector.h"
#include "Dialogue.h"

#include <queue>
#include "surface.h"
#include <iostream>
#include <vector>

class DialogueSystem;

class NPC : public InteractableObject
{
public:
	NPC(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy, int neededLevel);
	virtual ~NPC();

	void update(float deltaTime) override;
	void render(Tmpl8::Surface* screen) override;

	void giveDialogue(const Dialogue& d);

	std::queue<Dialogue> dialogueQueue;
	int neededLevel;
};

