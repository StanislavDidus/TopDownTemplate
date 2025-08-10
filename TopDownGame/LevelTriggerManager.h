#pragma once

#include <cstdio>
#include "Player.h"
#include "Entity.h"
#include "Vector.h"
#include "surface.h"
#include <iostream>
#include <functional>
#include <vector>

class Trigger : public InteractableObject
{
public:
	Trigger(int px, int py, int sx, int sy, int neededLevel, std::function<void()> func) : InteractableObject(nullptr, px, py, sx, sy),
		neededLevel(neededLevel), func(func) {
	}

	void interact(Entity* entity) override
	{
		func();
	}

	void update(float deltaTime) override {}
	void render(Tmpl8::Surface* screen) override {}

	int neededLevel;
private:
	std::function<void()> func;
};

class LevelTrigger
{
public:
	LevelTrigger(int px, int py, int sx, int sy, int nx, int ny, int neededLevel, int nextLevel) :
		px(px), py(py), sx(sx), sy(sy), nx(nx), ny(ny), neededLevel(neededLevel), nextLevel(nextLevel), isActive(true) {
	}
	virtual ~LevelTrigger() {}

	bool CheckCollision(Map* map, Entity* entity)
	{
		if (Map::currentLevel != neededLevel)
			return false;

		int entityX2 = entity->GetPosition().x + entity->GetSize().x;
		int entityY2 = entity->GetPosition().y + entity->GetSize().y;

		int triggerX2 = px + sx;
		int triggerY2 = py + sy;

		if (
			entity->GetPosition().x < triggerX2 &&
			entityX2 > px &&
			entity->GetPosition().y < triggerY2 &&
			entityY2 > py
			)
		{
			if (isActive)
			{
				entity->SetPosition(nx, ny);
				Map::currentLevel = nextLevel;
				return true;
			}
		}
		else
			isActive = true;

		return false;
	}
	void Disable() { isActive = false; }
private:
	int px, py, sx, sy, nx, ny, neededLevel, nextLevel;
	bool isActive;
};

class LevelTriggerManager
{
public:
	LevelTriggerManager(Map* map);
	virtual ~LevelTriggerManager();

	void CheckCollision(Entity* entity);
	void AddTrigger(int px, int py, int sx, int sy, int nx, int ny, int neededLevel, int nextLevel);
private:
	std::vector<LevelTrigger> levelTriggers;
	Map* map;
};

