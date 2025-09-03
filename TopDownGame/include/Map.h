#pragma once

#include <cstdio>
#include <iostream>
#include <vector>

#include "surface.h"
#include "template.h"
#include "LDtkLoader/Project.hpp"
#include "Enemy.h"
#include <map>

#include "Coordinator.h"

class Map
{
public:
	Map(Tmpl8::Sprite* sprite, const std::unordered_map<std::string, std::shared_ptr<Tmpl8::Sprite>>& sprites, const std::string& path);
	virtual ~Map();

	void Draw(Tmpl8::Surface* screen);
	const ldtk::Level& GetLevelRef(int level);

	std::unordered_map<int, std::vector<std::shared_ptr<Enemy>>>& getEnemies();
    std::shared_ptr<SquareGrid> getGrid(int level);

	void update(float deltaTime);
	void render(Tmpl8::Surface* screen);

	bool CheckCollision(int px, int py);
	bool intersects(int px, int py, int width, int height);

	static int currentLevel;
private:
	Tmpl8::Sprite* sprite;
	std::unordered_map<std::string, std::shared_ptr<Tmpl8::Sprite>> sprites;

	ldtk::Project ldtkMap;

	std::unordered_map<int, std::vector<std::shared_ptr<Enemy>>> enemies;
	std::unordered_map<int, std::shared_ptr<SquareGrid>> grids;

	/*int mapWidth , mapHeight;
	int levelWidth, levelHeight;*/

	std::vector<std::shared_ptr<LevelTrigger>> levelTriggers;

	void initMap();
	void initEntities(const ldtk::Layer& layer, int level, const std::string& name, std::vector<std::shared_ptr<Enemy>>& enemiesOnLevel);
	void initGrid(const ldtk::Layer& layer, int level);
	void initLevelTriggers(const ldtk::Layer& layer, int level);
	void initCollisions(const ldtk::Layer& layer, int level);
};

