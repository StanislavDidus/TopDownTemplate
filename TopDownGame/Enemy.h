#pragma once


#include <cstdio>
#include "surface.h"
#include "Entity.h"
#include <iostream>
#include <vector>

class Map;

class Enemy : public Entity
{
public:
	Enemy(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy, Map* map, int health, int damage, float attackSpeed, int money, int exp);
	virtual ~Enemy();

	virtual void Attack(std::shared_ptr<Entity> e, float timer);
	int getDps();

	void update(float deltaTime) override;
private:
	Map* map;
	float attackSpeed, lastAttackTime;
	int damage;
	
};

