#pragma once

#include <cstdio>
#include "surface.h"
#include "DrawableObject.h"
#include <iostream>
#include <vector>

class Entity : public DrawableObject
{
public:
	Entity(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy, int health, int money, int exp) :DrawableObject(sprite, px, py, sx, sy), maxHealth(health), currentHealth(maxHealth), money(money), exp(exp) {}
	virtual ~Entity() {}

	void Hit(int damage) { currentHealth -= damage; }
	//void Attack(std::shared_ptr<Entity> entity) {entity->Hit() }
	int getHp() { return currentHealth; }
	int getMoney() { return money; };
	int getExp() { return exp; }

	void SetPosition(Vec2i v) { px = v.x, py = v.y; }
	void SetPosition(int x, int y) { px = x, py = y; }
protected:
	int maxHealth, currentHealth, money, exp;
};

