#pragma once

#include <vector>
#include <unordered_map>
#include "surface.h"
#include <queue>
#include "bitset"
#include <cassert>
#include <array>

class EntityManager
{
public:
	EntityManager() : livingEntities(0)
	{
		for (int i = 0; i < MAX_ENTITIES; i++)
		{
			availableEntities.push(i);
		}
	}

	Entity_t createEntity()
	{
		assert(livingEntities < MAX_ENTITIES && "Too many entities in existence");
		
		Entity_t e = availableEntities.front();
		availableEntities.pop();

		livingEntities++;

		return e;
	}

	void removeEntity(Entity_t id)
	{
		assert(id < MAX_ENTITIES && "Entity out of range");
		
		signatures[id].reset();
		availableEntities.push(id);

		livingEntities--;
	}

	void setSignature(Entity_t id, Signature signature)
	{
		assert(id < MAX_ENTITIES && "Entity out of range");
		
		signatures[id] = signature;
	}

	Signature getSignature(Entity_t id)
	{
		assert(id < MAX_ENTITIES && "Entity out of range");
		
		return signatures[id];
	}

private:
	std::queue<Entity_t> availableEntities{};
	std::array<Signature, MAX_ENTITIES> signatures{};

	uint32_t livingEntities;
};

