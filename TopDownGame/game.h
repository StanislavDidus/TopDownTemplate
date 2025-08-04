#pragma once
#include <cstdio>
#include "Entity.h"
#include "Player.h"
#include "NPC.h"
#include "Map.h"
#include "LevelTriggerManager.h"
#include "surface.h"
#include <LDtkLoader/Project.hpp>
#include "DialogueSystem.h"
#include <set>
#include <vector>
#include <map>
#include <iostream>

//Button release, down, pressed

namespace Tmpl8 {

class Surface;

static Sprite playerSprite(new Surface("assets/player.png"), 1);
static Sprite npcSprite(new Surface("assets/npc.png"), 1);
static Sprite dialogueMenu(new Surface("assets/dm.png"), 1);

enum GameState
{
	Exploring, Battle
};

class Game
{
public:
	void SetTarget( Surface* surface ) { screen = surface; }
	void Init();
	void Shutdown();
	void Tick( float deltaTime );
	void MouseUp(int button) { Attack(); }
	void MouseDown(int button) {  }
	void MouseMove( int x, int y ) { /* implement if you want to detect mouse movement */ }
	void KeyUp(int key) { buttons.erase(key); }
	void KeyDown(int key) 
	{ 
		buttons.insert(key); 
	}

private:
	Surface* screen;

	std::unordered_map<std::string, Surface*> surfaces;
	std::unordered_map<std::string, std::shared_ptr<Sprite>> sprites;

	std::unique_ptr<DialogueSystem> dialogueSystem;

	std::set<int> previousButtons;
	std::set<int> buttons;

	GameState currentState;
	void setState(GameState& state);

	std::shared_ptr<Player> player;
	std::shared_ptr<Map> tileMap;

	std::unique_ptr<LevelTriggerManager> levelTriggerManager;

	void initSurfaces();
	void initLevelTriggers();
	void initNPCs();
	void initUI();
	void initMap();
	void initPlayer();

	void Attack();

	void updateControl();
	void CheckInteractions();

	bool wasButtonPresseed(int key) 
	{ 
		return std::find(previousButtons.begin(), previousButtons.end(), key) != previousButtons.end(); 
	}

	InteractableObject* interactableObjectsInRange;
	int interactDistance = 100;
	bool isInteraction = false;
};

}; // namespace Tmpl8