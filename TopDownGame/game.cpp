#include "game.h"

namespace Tmpl8
{ 
	constexpr int MAPWIDTH = 25;
	constexpr int MAPHEIGHT = 32;

	std::vector<std::string> map = {
	"b b b b b b b b b b b a a a b b b b b b b b b b b ",
	"b b b b b b b b b b b a a a b b b b b b b b b b b ",
	"b b b b b b b b b b b a a a b b b b b b b b b b b ",
	"b b b b b b b b b b b a a a b b b b b b b b b b b ",
	"b b cXcXcXcXb b b b b a a a b b b b b b b b b b b ",
	"b b cXcXcXcXb b b b b a a a b b b b b b b b b b b ",
	"b b cXcXcXcXb b b b b a a a b b b b b b b b b b b ",
	"b b b cXcXcXb b b b b a a a b b b b b b b b b b b ",
	"b b b cXcXcXb b b b b a a a b b b b b b b b b b b ",
	"b b b cXcXcXb b b b b a a a b b b b b b b b b b b ",
	"b b b b b b b b b b b a a a b b b b b b b b b b b ",
	"b b b b b b b b b b b a a a b b b b b b b b b b b ",
	"b b b b b b b b b b b a a a b b b b b b b b b b b ",
	"b b b b b b b b b b b a a a b b b b b b b b b b b ",
	"b b b b b b b b b b b a a a b b b b b b b b b b b ",
	"b b b b b b b b b b b a a a b b b b b b b b b b b ",
	//"-------------------------------------------------",
	"b b b b b b b b b b b a a a b b b b b b b b b b b ",
	"b b b b b b b b b b b a a a b b b b b b b b b b b ",
	"b b b b b b b b b b b a a a b b b b b b b b b b b ",
	"b b b b b b b b b b b a a a b b b b b b b b b b b ",
	"b b cXcXcXb b b b b b a a a b b b b b b b b b b b ",
	"b b cXcXcXb b b b b b a a a b b b b b b b b b b b ",
	"b b cXcXcXcXb b b b b a a a a a a a a b b b b b b ",
	"b b cXcXcXcXb b b b b a a a a a a a a b b b b b b ",
	"b b cXcXcXcXb b b b b a a a a a a a a b b b b b b ",
	"b b cXcXcXcXb b b b b a a a a a a a a b b b b b b ",
	"b b cXcXcXcXb b b b b a a a a a a a a b b b b b b ",
	"b b cXcXcXcXb b b b b a a a a a a a a b b b b b b ",
	"b b b b cXcXb b b b b a a a b b b b b b b b b b b ",
	"b b b b b b b b b b b a a a b b b b b b b b b b b ",
	"b b b b b b b b b b b a a a b b b b b b b b b b b ",
	"b b b b b b b b b b b a a a b b b b b b b b b b b "
	};

	int currentLevel = 1;

	static Sprite tileMapSprite(new Surface("assets/tiles.png"), 4);

	Map tileMap(&tileMapSprite, map, 25, 16);

	std::vector<std::shared_ptr<NPC>> npcs;

	Player player = Player(&playerSprite, 400, 200, 48, 72, &tileMap, currentLevel);

	LevelTriggerManager levelTriggerManager(currentLevel);

	void Game::initLevelTriggers()
	{
		levelTriggerManager.AddTrigger(11 * 32, 0, 3 * 32, 5, 11 * 32 + 3 * 32 / 2 - 48 / 2, 512 - 72, 0, 1);
		levelTriggerManager.AddTrigger(11 * 32, 512 - 5, 3 * 32, 5, 11 * 32 + 3 * 32 / 2 - 48 / 2, 0, 1, 0);
	}

	void Game::initNPCs()
	{
		npcs.push_back(std::make_shared<NPC>(&npcSprite, 500, 246, 48, 72, 1, currentLevel));
	}

	void Game::initUI()
	{
		dialogueSystem = std::make_unique<DialogueSystem>(&player, &dialogueMenu, 100, 384, 600, 128, 8);

		Dialogue dialogue = { std::vector<Replic>
		{
			{"Hi stranger! ", 2.f}, 
			{"What brought you to this place? ", 3.f}, 
			{"If you are looking for some work, you should pay a visit to a tavern nearby. ", 5.f},
			{"Though, be aware of the local flock of wolfs, they have already killed one innocent lady. ", 7.f}
		} };

		npcs.front()->giveDialogue(dialogue);
		npcs.front()->giveDialogue({ std::vector<Replic> { {"cought cought ", 2.f} }});
		//npcs.front()->showDialogue(dialogueSystem.get());
	}

	//UI DIALOGUE SIZE 100,384,600,128

	void Game::Init()
	{
		//ldtk::Project ldtk_project;
		//ldtk_project.loadFromFile("my_project.ldtk");
		
		initLevelTriggers();
		initNPCs();
		initUI();
	}

	void Game::Shutdown()
	{
	}

	void Game::Tick(float deltaTime)
	{
		screen->Clear(0);
		deltaTime /= 1000.f;

		//UPDATE
		updateControl();

		player.update(deltaTime);

		dialogueSystem->update(deltaTime);

		levelTriggerManager.CheckCollision(&player);

		//RENDER
		tileMap.Draw(currentLevel, screen);

		for (auto& npc : npcs)
			npc->render(screen);

		player.render(screen);

		dialogueSystem->render(screen);

		previousButtons = buttons;
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
						player.moveLeft();
					break;
				case 'd':
						player.moveRight();
					break;
				case 'w':
						player.moveUp();
					break;
				case 's':
						player.moveDown();
					break;
				}
			}
			
			if (key == 'e' && !wasButtonPresseed('e'))
			{
				//Interact with NPCs
				CheckInteractions();
			}
			else if (key == ' ' && !wasButtonPresseed(' '))
			{
				//Skip dialogue
				dialogueSystem->GetNextMessage();
			}
		}
	}

	

	void Game::CheckInteractions()
	{
		for (auto* obj : InteractableObject::GetAllInteractables())
		{
			NPC* n = dynamic_cast<NPC*>(obj);
			if (n != nullptr)
			{
				if (currentLevel == n->neededLevel)
				{
					int dx = n->GetPosition().x - player.GetPosition().x;
					int dy = n->GetPosition().y - player.GetPosition().y;

					int squareDistance = dx * dx + dy * dy;
					int squareInteractDistance = interactDistance * interactDistance;

					if (squareDistance < squareInteractDistance)
					{
						if (!n->dialogueQueue.empty())
						{
							dialogueSystem->showDialogue(n->dialogueQueue.front());
							n->dialogueQueue.pop();
							return;
						}
					}
				}
			}
		}

	}
};