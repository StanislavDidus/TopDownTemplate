#pragma once
#include <cstdio>
#include "Entity.h"
#include "Map.h"
#include "LDtkLoader/Project.hpp"
#include "surface.h"
#include <iostream>
#include "NPC.h"
#include <vector>
#include <algorithm>
#include "Inventory.h"
#include "Weapon.h"

class Player : public Entity
{
public:
	Player(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy, Map* map);
	virtual ~Player();

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	std::shared_ptr<Inventory> getInventory() const;

	void Attack();
	void giveMoney(int money);
	void giveExp(int exp);

	void update(float deltaTime) override;
private:
	bool CheckCollision(int px, int py);

	std::shared_ptr<Inventory> inventory;

	int fx, fy;
	Map* map;

	int interactDistance = 100; 
 };
