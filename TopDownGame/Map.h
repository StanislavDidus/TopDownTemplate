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

	void SetLevel(int level);
	int& GetLevel();

	std::vector<std::shared_ptr<Entity>> getEntities();	

	void update(float deltaTime);
	void render(Tmpl8::Surface* screen);
private:
	int currentLevel;

	Tmpl8::Sprite* sprite;
	std::unordered_map<std::string, std::shared_ptr<Tmpl8::Sprite>> sprites;

	ldtk::Project ldtkMap;

	std::vector<std::string> originMap;
	std::vector<std::vector<std::string>> levels;

	std::vector<std::shared_ptr<Entity>> entities;

	int mapWidth , mapHeight;
	int levelWidth, levelHeight;

	void initEntities(int level);
};

