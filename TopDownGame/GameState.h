#pragma once
#include "Enemy.h"
#include <vector>
class Game;

struct GameState {
	virtual void onEnter(Tmpl8::Game& game) = 0;
	virtual void onUpdate(Tmpl8::Game& game, float deltaTime) = 0;
	virtual void onRender(Tmpl8::Game& game, Tmpl8::Surface* screen) = 0;
	virtual void onExit(Tmpl8::Game& game) = 0;

	virtual ~GameState() = default;
};

struct ExploringState : public GameState
{
	void onEnter(Tmpl8::Game& game) override
	{

	}
	void onUpdate(Tmpl8::Game& game, float deltaTime)  override
	{
		game.updateControl();
		game.dialogueSystem->update(deltaTime);
		game.levelTriggerManager->CheckCollision(game.player.get());
		game.CheckInteractions();
	}
	void onRender(Tmpl8::Game& game, Tmpl8::Surface* screen) override
	{
		for (auto& npc : game.npcs)
			npc->render(screen);
		if (game.tileMap->GetLevel() == 2)
			game.sprites["House"]->Draw(screen, 240, 0);
		if (game.isInteraction)
			screen->PrintScaled("(E) ", (game.player->GetPosition().x + game.player->GetSize().x / 2) - 3 * 5 * 3 / 2, game.player->GetPosition().y - 20, 3.f, 3.f, 400, Tmpl8::Pixel(0xFFFFFF));
		game.player->render(screen);
	}
	void onExit(Tmpl8::Game& game)  override
	{

	}
};

struct BattleState : public GameState
{
	BattleState(std::vector<std::shared_ptr<Enemy>> enemies) : enemies(enemies) 
	{
		timer = 0.f;
		playerDpsDamage = 0.f;
	}

	void onEnter(Tmpl8::Game& game)  override
	{
		
		
		//Get all weapons from the player's inventory
		const auto& items = game.player->getInventory()->allItems();
		
		for (const auto& i : items)
		{
			const auto& w = std::dynamic_pointer_cast<Weapon>(i);
			if (w != nullptr)
				weapons.push_back(w);
		}

		for (const auto& w : weapons)
		{
			playerDpsDamage += w->getDps();
		}
	}
	void onUpdate(Tmpl8::Game& game, float deltaTime)  override
	{	
		//Start Timer
		timer += deltaTime;

		if (enemies.size() > 0)
		{
			auto& e = enemies[0];

			//Player attacks
			for (const auto& w : weapons)
			{
				w->Attack(e, timer);

				if (e->getHp() <= 0)
				{
					//Entity is dead
					game.player->giveMoney(e->getMoney());
					game.player->giveExp(e->getExp());

					enemies.erase(enemies.begin());
				}
			}

			for (const auto& e : enemies)
			{;
				e->Attack(game.player, timer);

				if (game.player->getHp() <= 0)
				{
					//Game is over
					std::cout << "Game Over\n";
					std::exit(0);
				}
			}
		}

		if (enemies.size() == 0)
		{
			game.setState(std::make_shared<ExploringState>());

			std::cout << game.player->getMoney() << ", " << game.player->getExp() << "\n";
		}
	}
	void onRender(Tmpl8::Game& game, Tmpl8::Surface* screen) override
	{
		game.sprites["BattleMenu"]->Draw(screen, 32, 32);
		game.sprites["Player"]->DrawScaled(200 - 72 / 2, 256 - 108 / 2, 72, 108, screen);

		//Draw Statistics for player
		float scale = 3.f;
		float charWidth = 5 * scale;
		int textHeight = 5.f * scale;

		std::string playerHp, playerDps;
		playerHp += std::to_string(game.player->getHp());
		playerDps += std::to_string((int)playerDpsDamage);
		int playerMidPoint = (200 - 72 / 2) + (72 / 2);
		
		//HP
		int textWidth1 = playerHp.length() * charWidth;
		screen->PrintScaled(&playerHp[0], playerMidPoint - (textWidth1 / 2), 175 - textHeight / 2, scale, scale, 800, Tmpl8::RedMask);
		//DPS
		int textWidth2 = playerDps.length() * charWidth;
		screen->PrintScaled(&playerDps[0], playerMidPoint - (textWidth2 / 2), 200 - textHeight / 2, scale, scale, 800, Tmpl8::Pixel(0xFFFFFF));

		int index = 0;
		for (auto& e : enemies)
		{
			int sizeX = e->GetSize().x * 1.5f;
			int sizeY = e->GetSize().y * 1.5f;
			Tmpl8::vec2 pos = enemiesPositions[enemies.size()][index];

			game.sprites["Wolf"]->DrawScaled(pos.x - sizeX / 2, pos.y - sizeY / 2, sizeX, sizeY, screen);

			index++;

			//Draw enemy statistics
			std::string enemyHp, enemyDps;
			
			enemyHp += std::to_string(e->getHp());
			enemyDps += std::to_string(e->getDps());
			int enemyMidPoint = (pos.x - sizeX / 2) + (sizeX / 2);

			//HP
			int enemyTextWidth1 = enemyHp.length() * charWidth;
			screen->PrintScaled(&enemyHp[0], enemyMidPoint - (enemyTextWidth1 / 2), 175 - textHeight / 2, scale, scale, 800, Tmpl8::RedMask);
			//DPS
			int enemyTextWidth2 = enemyDps.length() * charWidth;
			screen->PrintScaled(&enemyDps[0], enemyMidPoint - (enemyTextWidth2 / 2), 200 - textHeight / 2, scale, scale, 800, Tmpl8::Pixel(0xFFFFFF));
		}
	}
	void onExit(Tmpl8::Game& game)  override
	{

	}

	float timer;
	float playerDpsDamage;

	std::vector<std::shared_ptr<Weapon>> weapons;

	std::vector<std::shared_ptr<Enemy>> enemies;

	std::unordered_map<int, std::vector<Tmpl8::vec2>> enemiesPositions
	{
		{1, std::vector<Tmpl8::vec2>{{600,256}} }
	};
};