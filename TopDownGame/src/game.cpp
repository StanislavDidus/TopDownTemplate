#include "game.h"
#include "GameState.h"

namespace Tmpl8
{
	void Game::setState(std::shared_ptr<GameState> state)
	{
		if (currentState != nullptr)
			currentState->onExit(*this);

		if (state != nullptr && currentState != state)
			currentState = std::move(state);

		currentState->onEnter(*this);
	}

	void Game::initSurfaces()
	{
		surfaces["Tiles"] = new Surface("assets/tiles.png");
		sprites["Tiles"] = std::make_shared<Sprite>(surfaces["Tiles"], 4);

		surfaces["House"] = new Surface("assets/house.png");
		sprites["House"] = std::make_shared<Sprite>(surfaces["House"], 1);

		surfaces["Wolf"] = new Surface("assets/wolf.png");
		sprites["Wolf"] = std::make_shared<Sprite>(surfaces["Wolf"], 1);

		surfaces["BattleMenu"] = new Surface("assets/bm.png");
		sprites["BattleMenu"] = std::make_shared<Sprite>(surfaces["BattleMenu"], 1);

		surfaces["Player"] = new Surface("assets/player.png");
		sprites["Player"] = std::make_shared<Sprite>(surfaces["Player"], 4);

		surfaces["StunMarker"] = new Surface("assets/stunMarker.png");
		sprites["StunMarker"] = std::make_shared<Sprite>(surfaces["StunMarker"], 1);

		surfaces["QuestIcon"] = new Surface("assets/questIcon.png");
		sprites["QuestIcon"] = std::make_shared<Sprite>(surfaces["QuestIcon"], 1);

		surfaces["CQuestIcon"] = new Surface("assets/completedQuestIcon.png");
		sprites["CQuestIcon"] = std::make_shared<Sprite>(surfaces["CQuestIcon"], 1);
	}

	void Game::initLevelTriggers()
	{
		levelTriggerManager = std::make_unique<LevelTriggerManager>(tileMap.get());

		//Level 0-1 1-0
		levelTriggerManager->AddTrigger(11 * 32, 0, 3 * 32, 5, 11 * 32 + 3 * 32 / 2 - 48 / 2, 512 - 72, 0, 1);
		levelTriggerManager->AddTrigger(11 * 32, 512 - 5, 3 * 32, 5, 11 * 32 + 3 * 32 / 2 - 48 / 2, 0, 1, 0);

		levelTriggerManager->AddTrigger(11 * 32, 0, 3 * 32, 5, 11 * 32 + 3 * 32 / 2 - 48 / 2, 512 - 72, 1, 2);
		levelTriggerManager->AddTrigger(11 * 32, 512 - 5, 3 * 32, 5, 11 * 32 + 3 * 32 / 2 - 48 / 2, 0, 2, 1);

		//TeleportTriggers
		triggers.push_back(std::make_shared<Trigger>(13 * 32, 6 * 32, 32, 32 * 3, 2, [this]() {player->SetPosition(14 * 32, 512 - 5 * 32); Map::currentLevel = 3; }));
		levelTriggerManager->AddTrigger(14 * 32, 512 - 3 * 32, 2 * 32, 32, 13 * 32, 8 * 32, 3, 2);
		//13,6 14,8

		levelTriggerManager->AddTrigger(24 * 32, 0, 32, 320, 32, 5 * 32, 2, 4);
		levelTriggerManager->AddTrigger(0, 0, 32, 320, 23 * 32, 5 * 32, 4, 2);

		player->getInventory()->addItem(weapons["Sword"]);
	}

	void Game::initDialogues()
	{
		dialogueSystem = std::make_unique<DialogueSystem>(player.get(), &dialogueMenu, 100, 384, 600, 128, 8);

		dialogues["Valley"] = std::make_shared<Dialogue>(std::vector<Replic>
		{
			{"Hi stranger!", 2.f},
			{ "What brought you to this place?", 3.f },
			{ "If you are looking for some work, you should pay a visit to a tavern nearby.", 5.f },
			{ "Though, be aware of the local flock of wolfs, they have already killed one innocent lady on the pond.", 7.f },
			{ "Cough cought.", 2.f }
		});

		dialogues["Tavern1"] = std::make_shared<Dialogue>(std::vector<Replic>
		{
			{"Hi!, I guess you are looking for some work, aren't you?", 5.f},
			{ "There is a wolf east of here, you will be rewarded if you kill him.", 5.f, [this]() {questSystem->completeQuest("Initiation", "Work"); } }
		});

		dialogues["Tavern2"] = std::make_shared<Dialogue>(std::vector<Replic>
		{
			{"It seems like you have killed that wolf already.", 2.f},
			{ "That wolf has killed many people, please, accept this gift from us.", 3.f, [this]() {questSystem->completeQuest("Initiation", "Reward"); } }
		});
	}

	void Game::initWeapons()
	{
		weapons["Sword"] = std::make_shared<Weapon>("assets/sword.png", 50, 1.f, 50);
	}

	void Game::initNPCs()
	{
		npcs["Valley"] = std::make_shared<NPC>(&npcSprite, 500, 215, 48, 72, 1);
		npcs["Valley"]->giveDialogue(dialogues["Valley"].get());

		npcs["Tavern"] = std::make_shared<NPC>(&npcSprite, 15 * 32, 4 * 32, 48, 72, 3);
		npcs["Tavern"]->giveDialogue(dialogues["Tavern1"].get());
	}

	void Game::initUI()
	{

	}

	void Game::initMap()
	{
		tileMap = std::make_shared<Map>(sprites["Tiles"].get(), sprites, "assets/TiledMap/map.ldtk");
	}

	void Game::initPlayer()
	{
		player = std::make_shared<Player>(sprites["Player"].get(), 200, 200, 48, 72, tileMap.get());
	}

	void Game::initEventBus()
	{
		EventBus::Get();
	}

	void Game::initTriggers()
	{
		//Chest with a sword
		triggers.push_back(std::make_shared<Trigger>(20 * 32, 10 * 32, 64, 32, 0, [this]() {player->getInventory()->addItem(weapons["Sword"]); }, true));

		//If wolf_boss gets killed , add replic to npc
		EventBus::Get().AddListener(EventType::KILLED, std::function<void(int)>([this](int tag)
			{
				if (tag == 1)
				{
					npcs["Tavern"]->giveDialogue(dialogues["Tavern2"].get());
					questSystem->completeQuest("Initiation", "Prey");
				}
			}));
	}

	void Game::initQuests()
	{
		questSystem = std::make_shared<QuestSystem>(sprites["QuestIcon"].get(), sprites["CQuestIcon"].get());

		auto& questLine = std::make_shared<QuestLine>("Initiation", "You are starving, you should find some work before you die of hunger", [&]() {player->giveExp(100); });
		questLine->addQuest("Work", "Find some work");
		questLine->addQuest("Prey", "Kill the wolf");
		questLine->addQuest("Reward", "Collect your reward");
		questSystem->addNewQuestLine(questLine);
	}

	void Game::Init()
	{
		reactBattleTime = 1.5f;

		setState(std::make_shared<ExploringState>());

		initEventBus();
		initSurfaces();
		initMap();
		initPlayer();
		initDialogues();
		initWeapons();
		initNPCs();
		initUI();
		initLevelTriggers();
		initTriggers();
		initQuests();
	}

	void Game::Shutdown()
	{
		for (auto& [kay, value] : surfaces)
			delete value;

	}

	void Game::update(float deltaTime)
	{
		deltaTime /= 1000.f;

		player->update(deltaTime);
		tileMap->update(deltaTime);

		currentState->onUpdate(*this, deltaTime);

		previousButtons = buttons;
	}

	void Game::render(Tmpl8::Surface* screen)
	{
		screen->Clear(0);
		tileMap->render(screen);
		dialogueSystem->render(screen);

		currentState->onRender(*this, screen);
	}

	void Game::Tick(float deltaTime)
	{
		update(deltaTime);
		render(screen);
	}
	void Game::updateControl()
	{
		for (const auto& key : buttons)
		{
			if (!dialogueSystem->isActive)
			{
				switch (key)
				{
				case 'a':
					player->moveLeft();
					break;
				case 'd':
					player->moveRight();
					break;
				case 'w':
					player->moveUp();
					break;
				case 's':
					player->moveDown();
					break;
				}
			}

			if (!dialogueSystem->isActive)
				if (key == 'e' && !wasButtonPresseed('e') && isInteraction)
				{
					if (interactableObjectsInRange != nullptr)
					{
						NPC* n = dynamic_cast<NPC*>(interactableObjectsInRange);
						if (n != nullptr)
						{
							if (!n->dialogueQueue.empty())
							{
								dialogueSystem->showDialogue(n->dialogueQueue.front());
								n->dialogueQueue.pop();
							}
						}
						Trigger* t = dynamic_cast<Trigger*>(interactableObjectsInRange);
						if (t != nullptr)
						{
							t->interact(player.get());
						}
					}
					//CheckInteractions();
				}

			if (key == ' ' && !wasButtonPresseed(' '))
			{
				//Skip dialogue
				dialogueSystem->Skip();
			}
		}
	}

	void Game::Attack()
	{
		if (std::dynamic_pointer_cast<BattleState>(currentState) != nullptr)
			return;

		std::vector<std::weak_ptr<Enemy>> enemiesAttack;
		bool isHit = false;
		//Check if at least one enemy is hit
		for (auto& enemy : tileMap->getEnemies()[Map::currentLevel])
		{
			int dx = enemy->GetPosition().x - player->GetPosition().x;
			int dy = enemy->GetPosition().y - player->GetPosition().y;

			int squareDistance = dx * dx + dy * dy;
			int squareInteractDistance = interactDistance * interactDistance;

			if (squareDistance < squareInteractDistance)
			{
				isHit = true;
				break;
			}
		}
		//Get all enemies from the level
		if (isHit)
		{
			for (auto& enemy : tileMap->getEnemies()[Map::currentLevel])
			{
				enemiesAttack.push_back(enemy);
			}
		}
		//Start Battle
		if (!enemiesAttack.empty())
		{
			setState(std::make_shared<BattleState>(enemiesAttack, tileMap->getEnemies()[Map::currentLevel], true));
		}
	}

	void Game::CheckInteractions()
	{
		for (auto* obj : InteractableObject::GetAllInteractables())
		{
			int dx = obj->GetPosition().x - player->GetPosition().x;
			int dy = obj->GetPosition().y - player->GetPosition().y;

			int squareDistance = dx * dx + dy * dy;
			int squareInteractDistance = interactDistance * interactDistance;

			if (squareDistance < squareInteractDistance)
			{
				NPC* n = dynamic_cast<NPC*>(obj);
				if (n != nullptr)
				{
					if (n->neededLevel == Map::currentLevel)
					{
						if (!n->dialogueQueue.empty())
						{
							interactableObjectsInRange = obj;
							isInteraction = true;
							return;
						}
					}
				}
				Trigger* t = dynamic_cast<Trigger*>(obj);
				if (t != nullptr)
				{
					if (t->neededLevel == Map::currentLevel)
					{
						interactableObjectsInRange = obj;
						isInteraction = true;
						return;
					}
				}
			}

			interactableObjectsInRange = nullptr;
			isInteraction = false;
		}
	}
};