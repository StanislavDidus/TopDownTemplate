#pragma once

#include "SystemManager.h"
#include "Components.h"
#include "surface.h"
#include "Map.h"

class RenderByLevelSystem : public System
{
public:
	void update(Tmpl8::Surface* screen);

	std::unordered_map<std::string, std::shared_ptr<Tmpl8::Sprite>> sprites;
};

class AlwaysRenderSystem : public System
{
public:
	void update(Tmpl8::Surface* screen);

	std::unordered_map<std::string, std::shared_ptr<Tmpl8::Sprite>> sprites;
};

class HandlePlayerInputSystem : public System
{
public:
	void update(float deltaTime);
	bool isKeyDown(int key);

	std::set<int> buttons;
};

class EntityCollisionSystem : public System
{
public:
	void update(float deltaTime);
};

class PlayerCollisionSystem : public System
{
public:
	void update(float deltaTime);
};

class LevelTriggerSystem : public System
{
public:
	void update(float deltaTime);

	Entity_t player;
};