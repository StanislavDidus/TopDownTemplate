#pragma once
#include <cstdio>
#include "Entity.h"
#include "Map.h"
#include "surface.h"
#include <iostream>
#include <vector>
#include <algorithm>

class Player : public Entity
{
public:
	Player(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy, Map* map, int& currentLevel);
	virtual ~Player();

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	void update(float deltaTime) override;
private:
	bool CheckCollision(int px, int py);

	int fx, fy;
	Map* map;
	int& currentLevel;
};
