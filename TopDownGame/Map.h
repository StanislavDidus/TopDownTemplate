#pragma once

#include <cstdio>
#include "surface.h"
#include <iostream>
#include <vector>

class Map
{
public:
	Map(Tmpl8::Sprite* sprite, const std::vector<std::string>& map, int levelWidth, int levelHeight);
	virtual ~Map();

	void Draw(int level, Tmpl8::Surface* screen);
	std::vector<std::string> GetLevel(int level);
private:
	std::vector<std::string> Divide(int sx, int sy);

	Tmpl8::Sprite* sprite;

	std::vector<std::string> originMap;
	std::vector<std::vector<std::string>> levels;

	int mapWidth , mapHeight;
	int levelWidth, levelHeight;
};

