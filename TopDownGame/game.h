#pragma once
#include <cstdio>
#include "Entity.h"
#include "Player.h"
#include "NPC.h"
#include "Map.h"
#include "LevelTriggerManager.h"
#include "surface.h"
#include "DialogueSystem.h"
#include <set>
#include <vector>
#include <iostream>

namespace Tmpl8 {

class Surface;

static Sprite playerSprite(new Surface("assets/player.png"), 1);
static Sprite npcSprite(new Surface("assets/npc.png"), 1);
static Sprite dialogueMenu(new Surface("assets/dm.png"), 1);

class Game
{
public:
	void SetTarget( Surface* surface ) { screen = surface; }
	void Init();
	void Shutdown();
	void Tick( float deltaTime );
	void MouseUp( int button ) { /* implement if you want to detect mouse button presses */ }
	void MouseDown( int button ) { /* implement if you want to detect mouse button presses */ }
	void MouseMove( int x, int y ) { /* implement if you want to detect mouse movement */ }
	void KeyUp(int key) { buttons.erase(key); }
	void KeyDown(int key) { buttons.insert(key); }

private:
	Surface* screen;

	std::unique_ptr<DialogueSystem> dialogueSystem;

	std::set<int> buttons;

	void initLevelTriggers();
	void initNPCs();
	void initUI();

	void updateControl();
};

}; // namespace Tmpl8