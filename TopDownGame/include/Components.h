#pragma once

#include "surface.h"
#include "template.h"
#include "Inventory.h"
#include <functional>
#include <iostream>

class Map;

struct Transform
{
	float px = 0.f, py = 0.f;
	int sx = 0, sy = 0;
};

struct Renderable
{
	std::string  spriteKey = "";
	bool flipped;
};

struct Physics
{
	Tmpl8::vec2 velocity{};
	Tmpl8::vec2 maxVelocity{};
	Tmpl8::vec2 acceleration{};
	Tmpl8::vec2 friction{};
};

struct Collider
{
	Tmpl8::vec2 offset;
	int sx, sy;
	bool isTrigger;
};

struct Level
{
	int neededLevel = 0;
};

struct AlwaysRender
{

};

struct Health
{
	int maxHealth = 0, currentHealth = 0;
};

struct Exp
{
	int exp = 0;
};

struct Money
{
	int money = 0;
};

struct MovementSpeed
{
	float movementSpeed = 0.f;
};

struct ManagePlayerInputs
{
	
};

struct Trigger
{
	std::shared_ptr<std::function<void()>> func;
	bool isDestroyable;
};

struct LevelTrigger
{
	int nextLevel;
	float nx, ny;
	bool isDestoyable;
};

struct CanInteract
{
	float interactionDistance = 0.f;
};

struct CanAttack
{
	float damage = 0.f, attackSpeed = 0.f, lastAttackTime = 0.f;
};

struct Tag
{
	size_t tag = 0;
};

//struct DialogueQueue
//{
//	std::queue<std::shared_ptr<Dialogue>> dialogueQueue;
//};

struct InventoryComponent
{
	std::shared_ptr<Inventory> inventory;
};

struct MapComponent
{
	std::shared_ptr<Map> map;
};

struct Wandering
{
	/*GridLocation start;
	GridLocation goal;
	std::vector<GridLocation> path;*/
};