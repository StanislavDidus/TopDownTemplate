#pragma once


#include <cstdio>
#include "surface.h"
#include <cmath>
#include "Entity.h"
#include <iostream>
#include <vector>
#include "time.h"
#include "stdlib.h"
#include <algorithm>
#include "A-star.h"

struct GridLocation;

class Map;

class Enemy : public Entity
{
public:
	Enemy(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy, Map* map, int neededLevel, int tag, int health, int damage, float attackSpeed, int money, int exp, float movementSpeed);
	virtual ~Enemy();

	virtual void Attack(std::shared_ptr<Entity> e, float timer);
	int getDps();
	int getTag();

	void update(float deltaTime) override;
	void render(Tmpl8::Surface* screen) override;

	int neededLevel;
private:
	Map* map;
	float attackSpeed, lastAttackTime;
	int damage;

	int tag;

	//Movement
	float x, y;
	void ChangeDirection();

	float timer;

	GridLocation start;
	GridLocation goal;

	std::vector<GridLocation> path;
	GridLocation* currentGoal;

	void StartFollowing(std::vector<GridLocation>& path);
	void NextMark();
	GridLocation GenerateGoal();
};

