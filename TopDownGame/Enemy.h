#pragma once


#include <cstdio>
#include "surface.h"
#include "Entity.h"
#include <iostream>
#include <vector>
#include "time.h"
#include "stdlib.h"
#include <algorithm>

class Map;

class Enemy : public Entity
{
public:
	Enemy(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy, Map* map, int neededLevel, int health, int damage, float attackSpeed, int money, int exp);
	virtual ~Enemy();

	virtual void Attack(std::shared_ptr<Entity> e, float timer);
	int getDps();

	void update(float deltaTime) override;

	int neededLevel;
private:
	Map* map;
	float attackSpeed, lastAttackTime;
	int damage;

	//Movement
	float x, y;
	void ChangeDirection();

	float timer;
};

