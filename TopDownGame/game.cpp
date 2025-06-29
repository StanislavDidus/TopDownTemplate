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

	Player player = Player(&playerSprite, 400, 200, 48, 72, &tileMap, currentLevel);

	std::vector<NPC> npcs;

	LevelTriggerManager levelTriggerManager(currentLevel);

	void Game::initLevelTriggers()
	{
		levelTriggerManager.AddTrigger(11 * 32, 0, 3 * 32, 5, 11 * 32 + 3 * 32 / 2 - 48 / 2, 512 - 72, 0, 1);
		levelTriggerManager.AddTrigger(11 * 32, 512 - 5, 3 * 32, 5, 11 * 32 + 3 * 32 / 2 - 48 / 2, 0, 1, 0);
	}

	void Game::initNPCs()
	{
		npcs.emplace_back(&npcSprite, 500, 246, 48, 72, 1, currentLevel);
	}

	//UI DIALOGUE SIZE 100,384,600,128

	void Game::Init()
	{
		initLevelTriggers();
		initNPCs();
	}

	void Game::Shutdown()
	{
	}

	void Game::Tick(float deltaTime)
	{
		screen->Clear(0);

		updateControl();
		player.update(deltaTime);

		levelTriggerManager.CheckCollision(&player);

		tileMap.Draw(currentLevel, screen);

		for (const auto& npc : npcs)
			npc.render(screen);

		player.render(screen);

		//Up
		//screen->Bar(100, 0, 700, 128, Tmpl8::Pixel(0x000000));
		//Down
		//screen->Bar(100, 384, 700, 512, Tmpl8::Pixel(0x000000));
	}
	void Game::updateControl()
	{
		for (const auto& key : buttons)
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
	}
};