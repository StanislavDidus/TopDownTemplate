#include "Map.h"

Map::Map(Tmpl8::Sprite* sprite, const std::vector<std::string>& map, int levelWidth, int levelHeight) : sprite(sprite), originMap(map),
levelWidth(levelWidth), levelHeight(levelHeight)
{
	mapWidth = originMap[0].size();
	mapHeight = originMap.size();

	//Number of levels
	int xSec = mapWidth / 2 / levelWidth;
	int ySec = mapHeight / levelHeight;

	for (int i = 0; i < ySec; i++)
	{
		for (int j = 0; j < xSec; j++)
		{
			levels.push_back(Divide(j, i));
		}
	}
}

Map::~Map()
{
}

std::vector<std::string> Map::Divide(int sx, int sy)
{
	std::vector<std::string> m;

	for (int y = sy * levelHeight; y < sy * levelHeight + levelHeight; y++)
	{
		std::string str;
		for (int x = sx * levelWidth * 2; x < sx * levelWidth * 2 + levelWidth * 2; x++)
		{
			str += originMap[y][x];
		}
		m.push_back(str);
	}

	return m;
}

void Map::Draw(int level, Tmpl8::Surface* screen)
{
	for (int i = 0; i < levelWidth; i++)
	{
		for (int j = 0; j < levelHeight; j++)
		{
			int index = levels[level][j][i * 2] - 'a';
			sprite->SetFrame(index);
			sprite->DrawScaled(i * 32, j * 32, 32, 32, screen);
		}
	}
}

std::vector<std::string> Map::GetLevel(int level)
{
	return levels[level];
}
