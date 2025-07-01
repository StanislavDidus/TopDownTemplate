#pragma once

#include <cstdio>
#include "Entity.h"
#include "Vector.h"
#include "surface.h"
#include <iostream>
#include <vector>

class NPC : public Entity
{
public:
	NPC(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy, int neededLevel, int& currentLevel);
	virtual ~NPC();

	void update(float deltaTime) override;
	void render(Tmpl8::Surface* screen) override;
private:
	int neededLevel;
	int& currentLevel;
};

