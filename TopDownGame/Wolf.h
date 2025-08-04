#pragma once


#include <cstdio>
#include "surface.h"
#include "Entity.h"
#include <iostream>
#include <vector>

class Map;

class Wolf : public Entity
{
public:
	Wolf(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy, Map* map);
	virtual ~Wolf();

	void update(float deltaTime) override;
private:
	Map* map;
};

