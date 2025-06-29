#pragma once

#include <cstdio>
#include "Player.h"
#include "Entity.h"
#include "Vector.h"
#include "surface.h"
#include <iostream>
#include <vector>

class LevelTrigger
{
public:
	LevelTrigger(int px, int py, int sx, int sy, int nx, int ny, int neededLevel, int nextLevel) :
		px(px), py(py), sx(sx), sy(sy), nx(nx), ny(ny), neededLevel(neededLevel), nextLevel(nextLevel), isActive(true) {}
	virtual ~LevelTrigger() {}

	bool CheckCollision(Entity* entity, int& currentLevel) 
	{
		if (currentLevel != neededLevel)
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
				currentLevel = nextLevel;
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
	LevelTriggerManager(int& currentLevel);
	virtual ~LevelTriggerManager();

	void CheckCollision(Entity* entity);
	void AddTrigger(int px, int py, int sx, int sy, int nx, int ny, int neededLevel, int nextLevel);
private:
	std::vector<LevelTrigger> levelTriggers;

	int& currentLevel;
};

