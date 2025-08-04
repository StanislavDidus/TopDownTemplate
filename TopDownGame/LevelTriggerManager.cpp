#include "LevelTriggerManager.h"

LevelTriggerManager::LevelTriggerManager(Map* map) : map(map)
{

}

LevelTriggerManager::~LevelTriggerManager()
{

}

void LevelTriggerManager::CheckCollision(Entity* entity)
{
	for (auto& l : levelTriggers)
	{
		if (l.CheckCollision(map, entity))
		{
			for (auto& l : levelTriggers)
				l.Disable();
			return;
		}
	}
}

void LevelTriggerManager::AddTrigger(int px, int py, int sx, int sy, int nx, int ny, int neededLevel, int nextLevel)
{
	levelTriggers.emplace_back(px, py, sx, sy, nx, ny, neededLevel, nextLevel);
}

