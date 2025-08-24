#pragma once
#include <cstdio>
#include <iostream>
#include "surface.h"
#include <LDtkLoader/Project.hpp>

#include <set>
#include <vector>
#include <map>

#include "LevelTriggerManager.h"
#include "Entity.h"
#include "Player.h"
#include "NPC.h"
#include "Map.h"
#include "DialogueSystem.h"
#include "EventBus.h"
#include "QuestSystem.h"

class GameState;
class ExploringState;
class BattleState;

namespace Tmpl8 {

class Surface;

static Sprite npcSprite(new Surface("assets/npc.png"), 1);
static Sprite dialogueMenu(new Surface("assets/dm.png"), 1);

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

	friend ExploringState;
	friend BattleState;

private:
	Surface* screen;

	std::unordered_map<std::string, Surface*> surfaces;
	std::unordered_map<std::string, std::shared_ptr<Sprite>> sprites;

	std::unique_ptr<DialogueSystem> dialogueSystem;

	std::set<int> previousButtons;
	std::set<int> buttons;

	std::shared_ptr<GameState> currentState;
	void setState(std::shared_ptr<GameState> state);

	std::shared_ptr<QuestSystem> questSystem;

	std::shared_ptr<Player> player;
	std::shared_ptr<Map> tileMap;

	std::unique_ptr<LevelTriggerManager> levelTriggerManager;

	void initSurfaces();
	void initLevelTriggers();
	void initDialogues();
	void initWeapons();
	void initNPCs();
	void initUI();
	void initMap();
	void initPlayer();
	void initEventBus();
	void initTriggers();
	void initQuests();

	void update(float deltaTime);
	void render(Tmpl8::Surface* screen);

	void Attack();

	void updateControl();
	void CheckInteractions();

	bool wasButtonPresseed(int key) 
	{ 
		return std::find(previousButtons.begin(), previousButtons.end(), key) != previousButtons.end(); 
	}

	std::unordered_map<std::string, std::shared_ptr<Dialogue>> dialogues;
	std::unordered_map<std::string, std::shared_ptr<NPC>> npcs;
	std::unordered_map<std::string, std::shared_ptr<Weapon>> weapons;

	std::vector<std::shared_ptr<Trigger>> triggers;

	InteractableObject* interactableObjectsInRange;
	int interactDistance = 100;
	bool isInteraction = false;

	float reactBattleTime;
};

}; // namespace Tmpl8