#include "game.h"


namespace Tmpl8
{ 
	constexpr int MAPWIDTH = 25;
	constexpr int MAPHEIGHT = 16;

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
	"b b b b b b b b b b b a a a b b b b b b b b b b b "
	};

	Player player = Player(&playerSprite, 400, 200, 48, 72, map);

	void Game::Init()
	{
	}

	void Game::Shutdown()
	{
	}

	static Sprite tileMap(new Surface("assets/tiles.png"), 4);

	void Game::Tick(float deltaTime)
	{
		screen->Clear(0);

		updateControl();
		player.update(deltaTime);

		for (int i = 0; i < MAPWIDTH; i++)
		{
			for (int j = 0; j < MAPHEIGHT; j++)
			{
				int index = map[j][i * 2] - 'a';
				tileMap.SetFrame(index);
				tileMap.DrawScaled(i * 32, j * 32, 32, 32, screen);
			}
		}

		player.render(screen);
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