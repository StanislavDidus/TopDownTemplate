#include "Map.h"

int Map::currentLevel = 0;

Map::Map(Tmpl8::Sprite* sprite, const std::unordered_map<std::string, std::shared_ptr<Tmpl8::Sprite>>& sprites, const std::string& path) : sprite(sprite), sprites(sprites)
{
	ldtkMap.loadFromFile(path);
	initMap();
}

Map::~Map()
{
}

void Map::Draw(Tmpl8::Surface* screen)
{
	const auto& world = ldtkMap.getWorld("");
	const auto& tileSet = world.getTileset("Tiles");
	const auto& cLevel = world.allLevels()[currentLevel];
	const auto& layers = cLevel.allLayers();

	for (const auto& layer : layers)
	{
		for (const auto& tile : layer.allTiles())
		{
			//Init variables
			const auto& rect = tile.getTextureRect();
			const auto& pos = tile.getPosition();

			int srcX = rect.x;
			int srcY = rect.y;

			int dstX = pos.x * 2;
			int dstY = pos.y * 2;

			Tmpl8::Pixel* base = sprite->GetBuffer();
			int tileSetWidth = tileSet.texture_size.x;

			for (int j = 0; j < tileSet.tile_size * 2; j++)
			{
				//Move throught each pixel of the tile
				int sampleY = srcY + j / 2;
				Tmpl8::Pixel* row = base + sampleY * tileSetWidth;

				for (int i = 0; i < tileSet.tile_size * 2; i++)
				{
					int sampleX = srcX + i / 2;
					Tmpl8::Pixel* c = row + sampleX;

					//Remove transparent pixels
					if (*c != Tmpl8::Pixel(0x000000))
						screen->Plot(dstX + i, dstY + j, *c);
				}
			}
		}
	}
}

const ldtk::Level& Map::GetLevelRef(int level)
{
	const auto& world = ldtkMap.getWorld("");
	return world.allLevels()[level];
}

std::unordered_map<int, std::vector<std::shared_ptr<Enemy>>>& Map::getEnemies()
{
	return enemies;
}

std::shared_ptr<SquareGrid> Map::getGrid(int level)
{
	return grids[level];
}

bool Map::CheckCollision(int px, int py)
{
	const auto& level = GetLevelRef(Map::currentLevel);
	const auto& layer = level.getLayer("Col");

	for (const auto intPoint : layer.getIntGridValPositions(1))
	{
		int tx = static_cast<int>(px) / 32;
		int ty = static_cast<int>(py) / 32;

		if (tx == intPoint.x && ty == intPoint.y)
			return false;
	}
	return true;
}

bool Map::intersects(int px, int py, int width, int height)
{
	if (width <= 0 || height <= 0) return false;

	const auto& level = GetLevelRef(Map::currentLevel);
	const auto& layer = level.getLayer("Col");

	for (const auto intPoint : layer.getIntGridValPositions(1))
	{
		if (!(px + width <= intPoint.x * 32 ||
			intPoint.x * 32 + 32 <= px ||
			py + height <= intPoint.y * 32 ||
			intPoint.y * 32 + 32 <= py))
			return true;
	}
	return false;
}

void Map::update(float deltaTime)
{
	for (const auto& enemy : enemies[Map::currentLevel])
	{
		enemy->update(deltaTime);
	}
}

void Map::render(Tmpl8::Surface* screen)
{
	Draw(screen);

	for (const auto& enemy : enemies[Map::currentLevel])
	{
		enemy->render(screen);
	}

}

void Map::initMap()
{
	const auto& world = ldtkMap.getWorld("");
	const auto& tileSet = world.getTileset("Tiles");

	for (int i = 0; i < world.allLevels().size(); i++)
	{
		const auto& cLevel = world.allLevels()[i];
		const auto& layer = cLevel.getLayer("Entities");
		const auto& layerCol = cLevel.getLayer("Col");

		std::vector<std::shared_ptr<Enemy>> enemiesOnLevel;

		initGrid(layerCol, i);
		initLevelTriggers(layer, i);
		initCollisions(layerCol, i);
		initEntities(layer, i, "Wolf", enemiesOnLevel);

		enemies[i] = enemiesOnLevel;

	}
}

void Map::initEntities(const ldtk::Layer& layer, int level, const std::string& name, std::vector<std::shared_ptr<Enemy>>& enemiesOnLevel)
{
	for (const ldtk::Entity& e : layer.getEntitiesByName(name))
	{
		const auto& pos = e.getPosition();
		const auto& size = e.getSize();
		const auto& rect = e.getTextureRect();
		const auto& path = e.getTexturePath();
		const auto& name = e.getName();

		auto& health = e.getField<int>("Health").value();
		auto& damage = e.getField<int>("Damage").value();
		auto& attackSpeed = e.getField<float>("AttackSpeed").value();
		auto& money = e.getField<int>("Money").value();
		auto& exp = e.getField<int>("Exp").value();
		auto& tag = e.getField<int>("QuestTag").value();
		auto& movementSpeed = e.getField<float>("MovementSpeed").value();


		/*std::cout << "Entity's stats: \n"
			<< "Health: " << health << "\n"
			<< "Damage: " << damage << "\n"
			<< "Money: " << money << "\n"
			<< "Exp: " << exp << "\n";*/;

			enemiesOnLevel.push_back(std::make_shared<Enemy>(sprites[name].get(), pos.x * 2, pos.y * 2, size.x * 3, size.y * 3, this, level, tag, health, damage, attackSpeed, money, exp, movementSpeed));

	}
}

void Map::initGrid(const ldtk::Layer& layer, int level)
{
	auto grid = std::make_shared<SquareGrid>(ScreenWidth, ScreenHeight);

	for (const auto intPoint : layer.getIntGridValPositions(1))
	{
		grid->walls.insert(GridLocation{ intPoint.x * 32,intPoint.y * 32 });
		grid->walls.insert(GridLocation{ intPoint.x * 32 + 16,intPoint.y * 32 });
		grid->walls.insert(GridLocation{ intPoint.x * 32,intPoint.y * 32 + 16 });
		grid->walls.insert(GridLocation{ intPoint.x * 32 + 16,intPoint.y * 32 + 16 });
	}
	grids[level] = grid;
	//std::cout << level << ",  " << grids.size() << "\n";
}

void Map::initLevelTriggers(const ldtk::Layer& layer, int level)
{
	for (const ldtk::Entity& e : layer.getEntitiesByName("LevelTrigger"))
	{
		const auto& pos = e.getPosition();
		const auto& size = e.getSize();
		const auto& rect = e.getTextureRect();
		const auto& path = e.getTexturePath();
		const auto& name = e.getName();

		auto& nextLevel = e.getField<int>("NextLevel").value();
		auto& newX = e.getField<float>("NewX").value();
		auto& newY = e.getField<float>("NewY").value();
		auto& isDestroyable = e.getField<bool>("IsDestroyable").value();

		auto trigger = Coordinator::Get().createEntity();
		Coordinator::Get().addComponent<Transform>(trigger, Transform{ pos.x * 2.f, pos.y * 2.f, size.x * 2, size.y * 2 });
		Coordinator::Get().addComponent<Collider>(trigger, Collider{ {0.f,0.f}, size.x * 2, size.y * 2, true });
		Coordinator::Get().addComponent<Level>(trigger, Level{ level });
		Coordinator::Get().addComponent<LevelTrigger>(trigger, LevelTrigger{ nextLevel, newX, newY, isDestroyable });
	}
}

void Map::initCollisions(const ldtk::Layer& layer, int level)
{
	for (const auto intPoint : layer.getIntGridValPositions(1))
	{
		auto tile = Coordinator::Get().createEntity();
		Coordinator::Get().addComponent<Transform>(tile, Transform{ static_cast<float>(intPoint.x * 32), static_cast<float>(intPoint.y * 32), 32, 32 });
		Coordinator::Get().addComponent<Collider>(tile, Collider{ {0.f,0.f}, 32, 32, false });
		Coordinator::Get().addComponent<Level>(tile, Level{ level });
	}
}
	