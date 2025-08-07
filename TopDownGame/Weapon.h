#pragma once

#include <cstdio>
#include "surface.h"
#include <iostream>
#include "Item.h"
#include <vector>
#include "Entity.h"
#include <functional>

class Weapon : public Item
{
public:
	Weapon(const std::string& iconPath, int damage, float attackSpeed, int value);
	virtual ~Weapon();

	void Attack(std::shared_ptr<Entity> entity, float timer);
	int getDps();
private:
	int damage;
	float attackSpeed; // How much time it takes to hit, 2 - 1 hit per 2 seconds
	float lastAttackTime;
};