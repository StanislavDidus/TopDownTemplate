#pragma once
#include <cstdio>
#include "Player.h"
#include "surface.h"
#include <iostream>
#include <vector>

class Player
{
public:
	Player(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy, const std::vector<std::string>& map);
	virtual ~Player();

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	void update(float deltaTime);
	void render(Tmpl8::Surface* screen);
private:
	Tmpl8::Sprite sprite;

	bool CheckCollision(int px, int py);

	int px, py, sx, sy;
	int fx, fy;
	std::vector<std::string> map;
};
