#pragma once
#include "Enemy.h"
#include "EventBus.h"

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
	ExploringState() : isEnemyNearby(false) {}

	void onEnter(Tmpl8::Game& game) override
	{
		timeToEncounterBattle = game.reactBattleTime;
	}
	void onUpdate(Tmpl8::Game& game, float deltaTime)  override
	{
		game.updateControl();
		game.dialogueSystem->update(deltaTime);
		game.levelTriggerManager->CheckCollision(game.player.get());
		CheckCloseEncounters(game, deltaTime);
		LookForEnemiesNearby(game);
		game.CheckInteractions();
	}
	void onRender(Tmpl8::Game& game, Tmpl8::Surface* screen) override
	{
		for (auto& npc : game.npcs)
			npc->render(screen);
		if (Map::currentLevel == 2)
			game.sprites["House"]->Draw(screen, 240, 0);
		if (game.isInteraction)
			screen->PrintScaled("(E)", (game.player->GetPosition().x + game.player->GetSize().x / 2) - 3 * 5 * 3 / 2, game.player->GetPosition().y - 20, 3.f, 3.f, 400, Tmpl8::Pixel(0xFFFFFF));
		if (isEnemyNearby)
			screen->PrintScaled("ATTACK!", (game.player->GetPosition().x + game.player->GetSize().x / 2) - 6 * 5 * 5 / 2, game.player->GetPosition().y - 50, 5.f, 5.f, 400, Tmpl8::Pixel(0xFFFFFF));
		game.player->render(screen);
	}
	void onExit(Tmpl8::Game& game)  override
	{

	}

	void CheckCloseEncounters(Tmpl8::Game& game, float deltaTime)
	{
		bool isClose = false;
		for (const auto& enemy : game.tileMap->getEnemies()[Map::currentLevel])
		{
			int dx = enemy->GetPosition().x - game.player->GetPosition().x;
			int dy = enemy->GetPosition().y - game.player->GetPosition().y;

			int squareDistance = dx * dx + dy * dy;
			int squareInteractDistance = game.interactDistance * game.interactDistance;

			if (squareDistance < squareInteractDistance)
			{
				timeToEncounterBattle -= deltaTime;
				isClose = true;
			}
		}

		if (!isClose)
		{
			timeToEncounterBattle = game.reactBattleTime;
		}

		if (timeToEncounterBattle <= 0.f)
		{
			timeToEncounterBattle = game.reactBattleTime;
			//Start Battle
			std::vector<std::weak_ptr<Enemy>> enemiesAttack;
			for (auto& enemy : game.tileMap->getEnemies()[Map::currentLevel])
			{
				enemiesAttack.push_back(enemy);
			}
			game.setState(std::static_pointer_cast<GameState>(std::make_shared<BattleState>(enemiesAttack, game.tileMap->getEnemies()[Map::currentLevel], false)));
			//game.setState(std::make_shared<BattleState>(enemiesAttack, game.tileMap->getEnemies()[Map::currentLevel], false));
		}
	}

	void LookForEnemiesNearby(Tmpl8::Game& game)
	{
		for (const auto& enemy : game.tileMap->getEnemies()[Map::currentLevel])
		{
			int dx = enemy->GetPosition().x - game.player->GetPosition().x;
			int dy = enemy->GetPosition().y - game.player->GetPosition().y;

			int squareDistance = dx * dx + dy * dy;
			int squareInteractDistance = game.interactDistance * game.interactDistance;

			if (squareDistance < squareInteractDistance)
			{
				isEnemyNearby = true;
				return;
			}
		}
		isEnemyNearby = false;
	}

	float timeToEncounterBattle;

	bool isEnemyNearby;
};

struct BattleState : public GameState
{
	BattleState(std::vector<std::weak_ptr<Enemy>>& enemiesInBattle, std::vector<std::shared_ptr<Enemy>>& allEnemies, bool advantage) : enemiesInBattle(enemiesInBattle), allEnemies(allEnemies),
		advantage(advantage)
	{
		playerAttackTimer = 0.f;
		enemyAttackTimer = 0.f;
		advantageTimer = 0.f;
		playerDpsDamage = 0.f;
		messageTime = 2.f;
		messageTimer = 0.f;

		EventBus::Get().AddListener(EventType::ATTACK, std::make_shared<std::function<void(int)>>([this](int value) { ShowMessage("Dealt damage: ", value); }));
		EventBus::Get().AddListener(EventType::HIT, std::make_shared<std::function<void(int)>>([this](int value) { ShowMessage("Taken damage: ", value); }));
		EventBus::Get().AddListener(EventType::KILLED, std::make_shared<std::function<void(int)>>([this](int value) { ShowMessage("Enemies killed: ", value); }));
		EventBus::Get().AddListener(EventType::HEALED, std::make_shared<std::function<void(int)>>([this](int value) { ShowMessage("Hp healed: ", value); }));
		EventBus::Get().AddListener(EventType::STUNNED, std::make_shared<std::function<void(int)>>([this](int value) { ShowMessage("Stunned for: ", value); }));
	}

	void onEnter(Tmpl8::Game& game)  override
	{
		//std::cout << advantage << "\n";
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

		if(!advantage)
			EventBus::Get().Push(EventType::STUNNED, 1);
	}
	void onUpdate(Tmpl8::Game& game, float deltaTime)  override
	{
		//Start Timer
		if (!advantage)
			advantageTimer += deltaTime;

		messageTimer += deltaTime;
		if (playerMessages.size() > 0 && messageTimer >= messageTime)
		{
			playerMessages.erase(playerMessages.begin());
			messageTimer = 0.f;
		}

		//If player does not have advantage he does not attack for the first second
		if ((!advantage && advantageTimer >= 1.f) || advantage)
		{
			playerAttackTimer += deltaTime;
			advantage = true;
		}

		enemyAttackTimer += deltaTime;

		if (enemiesInBattle.size() > 0)
		{
			if (auto& e = enemiesInBattle[0].lock())
			{
				//Player attacks 

				for (const auto& w : weapons)
				{
					w->Attack(e, playerAttackTimer);

					if (e->getHp() <= 0)
					{
						//Entity is dead
						game.player->giveMoney(e->getMoney());
						game.player->giveExp(e->getExp());

						auto it = std::find(allEnemies.begin(), allEnemies.end(), e);
						if (it != allEnemies.end())
							allEnemies.erase(it);

						e.reset();


						if (enemiesInBattle[0].expired())
							enemiesInBattle.erase(enemiesInBattle.begin());

						EventBus::Get().Push(EventType::KILLED, 1);
					}
				}
			}

			//std::cout << enemiesInBattle.size() << "\n";

			for (const auto& enemy : enemiesInBattle)
			{
				if (auto e = enemy.lock())
				{
					e->Attack(game.player, enemyAttackTimer);

					if (game.player->getHp() <= 0)
					{
						//Game is over
						std::cout << "Game Over\n";
						std::exit(0);
					}
				}
			}
		}

		if (enemiesInBattle.empty())
		{
			game.setState(std::make_shared<ExploringState>());
		}
	}
	void onRender(Tmpl8::Game& game, Tmpl8::Surface* screen) override
	{
		

		//Draw Statistics for player
		float scale = 3.f;
		float charWidth = 5 * scale;
		int textHeight = 5.f * scale;

		std::string playerHp, playerDps;
		playerHp += std::to_string(game.player->getHp());
		playerDps += std::to_string((int)playerDpsDamage);
		
		int playerX = 200 - 72 / 2;
		int playerY = 256 - 108 / 2;
		int playerWidth = 72;
		int playerHeight = 108;
		int playerMidPoint = playerX + (playerWidth / 2);

		game.sprites["BattleMenu"]->Draw(screen, 32, 32);
		game.sprites["Player"]->DrawScaled(playerX, playerY, playerWidth, playerHeight, screen);

		//HP
		int textWidth1 = playerHp.length() * charWidth;
		screen->PrintScaled(&playerHp[0], playerMidPoint - (textWidth1 / 2), 175 - textHeight / 2, scale, scale, 800, Tmpl8::RedMask);
		//DPS
		int textWidth2 = playerDps.length() * charWidth;
		screen->PrintScaled(&playerDps[0], playerMidPoint - (textWidth2 / 2), 200 - textHeight / 2, scale, scale, 800, Tmpl8::Pixel(0xFFFFFF));
		//Messages
		for (int i = 0; i < playerMessages.size(); i++)
		{
			screen->PrintScaled(&playerMessages[i][0], playerX + playerWidth + 25, playerY + playerHeight - 10 * 2.f * (i + 1), 2.f, 2.f, 800, Tmpl8::Pixel(0xFFFFFF));
		}

		if (!advantage)
			game.sprites["StunMarker"]->DrawScaled(200 - 72 / 2 - 15, 256 - 108 / 2 - 15, 72 + 30, 108 + 30, game.screen);

		int index = 0;
		for (auto& enemy : enemiesInBattle)
		{
			if (auto e = enemy.lock())
			{
				if (e->neededLevel != Map::currentLevel)
					continue;

				int sizeX = e->GetSize().x * 1.5f;
				int sizeY = e->GetSize().y * 1.5f;
				Tmpl8::vec2 pos = enemiesPositions[enemiesInBattle.size()][index];

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
	}
	void onExit(Tmpl8::Game& game)  override
	{

	}

	void ShowMessage(std::string str, int value)
	{
		str += std::to_string(value) + "\n";
		playerMessages.push_back(str);
	}

	bool advantage;
	float advantageTimer;

	float playerAttackTimer;
	float enemyAttackTimer;
	float playerDpsDamage;

	std::vector<std::string> playerMessages;
	float messageTimer;
	float messageTime;

	std::vector<std::shared_ptr<Weapon>> weapons;

	std::vector<std::weak_ptr<Enemy>> enemiesInBattle;
	std::vector<std::shared_ptr<Enemy>>& allEnemies;

	std::unordered_map<int, std::vector<Tmpl8::vec2>> enemiesPositions
	{
		{1, std::vector<Tmpl8::vec2>{{600,256}} }
	};
};