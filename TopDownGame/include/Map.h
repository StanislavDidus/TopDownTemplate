#pragma once

#include <cstdio>
#include <iostream>
#include <vector>

#include "surface.h"
#include "template.h"
#include "LDtkLoader/Project.hpp"
#include "Enemy.h"
#include <map>

class Map
{
public:
	Map(Tmpl8::Sprite* sprite, const std::unordered_map<std::string, std::shared_ptr<Tmpl8::Sprite>>& sprites, const std::string& path);
	virtual ~Map();

	void Draw(Tmpl8::Surface* screen);
	const ldtk::Level& GetLevelRef(int level);

	std::unordered_map<int, std::vector<std::shared_ptr<Enemy>>>& getEnemies();

	void update(float deltaTime);
	void render(Tmpl8::Surface* screen);

	bool CheckCollision(int px, int py);

	static int currentLevel;
private:
	Tmpl8::Sprite* sprite;
	std::unordered_map<std::string, std::shared_ptr<Tmpl8::Sprite>> sprites;

	ldtk::Project ldtkMap;

	std::vector<std::string> originMap;
	std::vector<std::vector<std::string>> levels;

	std::unordered_map<int, std::vector<std::shared_ptr<Enemy>>> enemies;

	int mapWidth , mapHeight;
	int levelWidth, levelHeight;

	void initEnemies();
};

