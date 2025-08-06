#include "Map.h"

Map::Map(Tmpl8::Sprite* sprite, const std::unordered_map<std::string, std::shared_ptr<Tmpl8::Sprite>>& sprites, const std::string& path) : currentLevel(4), sprite(sprite), sprites(sprites)
{
	ldtkMap.loadFromFile(path);
	initEntities(currentLevel);
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
					if(*c != Tmpl8::Pixel(0x000000))
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

void Map::SetLevel(int level)
{
	currentLevel = level;
	initEntities(currentLevel);
}

int& Map::GetLevel()
{
	return currentLevel;
}

std::vector<std::shared_ptr<Entity>> Map::getEntities()
{
	return entities;
}

void Map::update(float deltaTime)
{
}

void Map::render(Tmpl8::Surface* screen)
{
	Draw(screen);
	
	for (const auto& e : entities)
	{
		e->render(screen);
	}
}

void Map::initEntities(int level)
{
	entities.clear();
	
	const auto& world = ldtkMap.getWorld("");
	const auto& tileSet = world.getTileset("Tiles");
	const auto& cLevel = world.allLevels()[level];
	const auto& layer = cLevel.getLayer("Entities");

	for (const ldtk::Entity& e : layer.getEntitiesByName("Wolf"))
	{
		const auto& pos = e.getPosition();
		const auto& size = e.getSize();
		const auto& rect = e.getTextureRect();
		const auto& path = e.getTexturePath();
		const auto& name = e.getName();

		auto& health = e.getField<float>("Health").value();
		auto& damage = e.getField<float>("Damage").value();
		auto& attackSpeed = e.getField<float>("AttackSpeed").value();
		auto& money = e.getField<float>("Money").value();
		auto& exp = e.getField<float>("Exp").value();

		std::cout << "Entity's stats: \n"
				  << "Health: " << health << "\n"
				  << "Damage: " << damage << "\n"
				  << "Money: " << money << "\n"
				  << "Exp: " << exp << "\n";

		entities.push_back(std::make_shared<Enemy>(sprites["Wolf"].get(), pos.x * 2, pos.y * 2, size.x * 3, size.y * 3, this, health, damage, attackSpeed, money, exp));
	}
}
