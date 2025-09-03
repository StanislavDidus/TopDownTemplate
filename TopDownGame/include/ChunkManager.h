#pragma once

#include <iostream>
#include <array>
#include <unordered_map>
#include <vector>
#include <functional>
#include <bitset>

using Entity_t = uint32_t;
const Entity_t MAX_ENTITIES = 1000;

using ComponentType = uint8_t;
const ComponentType MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;

class Chunk;	

class IChunkArray
{
public:
	virtual void moveData(Entity_t id, std::shared_ptr<Chunk> chunk) = 0;
	virtual void entityDestroyed(Entity_t id) = 0;
};

template<typename T>
class ChunkArray : IChunkArray
{
public:
	void addData(Entity_t id, T t)
	{
		assert(entityToIndex.find(id) == entityToIndex.end() && "Component was added to the entity more than once");

		size_t newSize = size;

		entityToIndex[id] = newSize;
		data[newSize] = t;
		entities[newSize] = id;
		size++;
	}

	void removeData(Entity_t id)
	{
		assert(entityToIndex.find(id) == entityToIndex.end() && "Entity does not exist");

		size_t removedIndex = entityToIndex[id];
		size_t lastIndex = size - 1;
		
		std::swap(data[lastIndex], data[removedIndex]);
		std::swap(entities[lastIndex], entities[removedIndex]);

		entityToIndex[entities[removedIndex]] = removedIndex;

		entityToIndex.erase(id);
		size--;

	}

	void moveData(Entity_t entity, std::shared_ptr<Chunk> chunk) override
	{
		chunk->addData(entity, data[entityToIndex[entity]]);
		removeData(entity);
	}

	T& getData(Entity_t id)
	{
		assert(entityToIndex.find(id) != entityToIndex.end() && "Entity does not exist");

		return data[entityToIndex[id]];
	}

	void entityDestroyed(Entity_t id) override
	{
		if (entityToIndex.find(id) != entityToIndex.end())
			removeData(id);
	}
private:
	std::array<T, MAX_ENTITIES> data;
	std::unordered_map<Entity_t, size_t> entityToIndex;
	std::array<Entity_t, MAX_ENTITIES> entities;
};

class Chunk
{
public:
	Chunk() {}

	template<typename T>
	void addData(Entity_t id, T component)
	{
		getChunkArray<T>()->addData(id, component);

		addEntity(id);
	}

	template<typename T>
	void removeData(Entity_t id)
	{
		getChunkArray<T>()->removeData(id);

		removeEntity(id);
	}

	template<typename T>
	T& getData(Entity_t id)
	{
		return getChunkArray<T>()->getData(id);
	}

	void moveData(Entity_t id, std::shared_ptr<Chunk> chunk)
	{
		for (auto& [name, array] : chunkArrays)
		{
			array->moveData(id, chunk);
		}
	}

	void addChunkArray(const char* name, std::shared_ptr<IChunkArray> chunkArray)
	{
		chunkArrays[name] = chunkArray;
	}

	const std::vector<Entity_t>& getEntities()
	{
		std::vector<Entity_t> entityVector;
		entityVector.reserve(MAX_ENTITIES);
		entityVector.insert(entityVector.begin(), entities.begin(), entities.end());
		return entityVector;
	}

	void setSignature(const Signature& signature)
	{
		this->signature = signature;
	}

	const Signature& getSignature() const
	{
		return signature;
	}

	void entityDestroyed(Entity_t id)
	{
		for (auto& [name, array] : chunkArrays)
		{
			array->entityDestroyed(id);
			if (entityToIndex.find(id) != entityToIndex.end())
				removeEntity(id);
		}
	}

	template<typename T>
	std::shared_ptr<ChunkArray<T>> getChunkArray() const
	{
		const char* name = typeid(T).name();

		return std::static_pointer_cast<ChunkArray<T>>(chunkArrays[name]);
	}
private:
	void addEntity(Entity_t id)
	{
		if (entityToIndex.find(id) == entityToIndex.end())
		{
			entities[size] = id;
			entityToIndex[id] = size;
			size++;
		}
	}
	void removeEntity(Entity_t id)
	{
		if (entityToIndex.find(id) != entityToIndex.end())
		{
			size_t removedIndex = entityToIndex[id];
			size_t lastIndex = size - 1;

			std::swap(entities[lastIndex], entities[removedIndex]);

			entityToIndex[entities[removedIndex]] = removedIndex;
			entityToIndex.erase(id);

			size--;
		}
	}

	std::unordered_map<const char*, std::shared_ptr<IChunkArray>> chunkArrays;
	std::array<Entity_t, MAX_ENTITIES> entities;
	std::unordered_map<Entity_t, size_t> entityToIndex;
	Signature signature;
	size_t size;
};

class ChunkManager
{
public:
	template<typename T>
	void registerComponent()
	{
		const char* name = typeid(T).name();

		chunkArraysRegister[registeredComponents] = []() { return std::make_shared<ChunkArray<T>>(); };

		componentTypes[name] = registeredComponents;
		componentNames[registeredComponents] = name;

		registeredComponents++;
	}

	template<typename T>
	void addComponent(Entity_t entity, T component, Signature& signature)
	{
		const char* name = typeid(T).name();

		assert(componentTypes.find(name) != componentTypes.end());

		//Find old chunk of an entity
		std::shared_ptr<Chunk> oldChunk;
		if (auto it = entityToChunk.find(entity); it != entityToChunk.end())
			oldChunk = it->second; 

		//Generate new signature
		signature.set(componentType[name], true);

		//Find or create new chunk for entity
		auto newChunk = findOrCreateChunk(signature);
		newChunk->setSignature(signature);

		//Move all data to new chunk from the old one
		if(oldChunk != nullptr)
			oldChunk->moveData(entity, newChunk);

		//Add new data
		newChunk.addData(entity, component);

		//Set chunk to this entity
		entityToChunk[entity] = newChunk;
	}

	template<typename T>
	void removeComponent(Entity_t entity, Signature& signature)
	{
		const char* name = typeid(T).name();

		assert(componentTypes.find(name) != componentTypes.end());
		assert(entitiesToChunk.find(entity) != entityToChunk.end());

		//Find old chunk of an entity
		auto oldChunk = entityToChunk[entity]; 

		//Change signature
		signature.set(componentType[name], false);

		//Find or create new chunk for entity
		auto newChunk = findOrCreateChunk(signature);

		//Move all data to new chunk from the old one
		oldChunk->removeData<T>(entity);
		oldChunk->moveData(entity, newChunk);

		//Set chunk to this entity
		entityToChunk[entity] = newChunk;
	}

	template<typename T>
	T& getComponent(Entity_t entity)
	{
		assert(entitiesToChunk.find(entity) != entityToChunk.end());
		
		entityToChunk[entity]->getData(entity);
	}

	template<typename T>
	ComponentType getComponentType()
	{
		const char* name = typeid(T).name();

		return componentTypes[name];
	}

	const std::vector<Entity_t>& getEntities(const Signature& signature)
	{
		std::vector<Entity_t> entities;
		for (const auto& chunk : chunks)
		{
			if ((chunk->getSignature() & signature) == signature)
			{
				auto& chunkVector = chunk->getEntities();
				entities.reserve(chunkVector.size());
				entities.insert(entities.end(), chunkVector.begin(), chunkVector.end());
			}
		}
	}

	void entityDestroyed(Entity_t entity)
	{
		for (auto& chunk : chunks)
		{
			chunk->entityDestroyed(entity);
		}
	}

private:
	std::shared_ptr<Chunk> findOrCreateChunk(const Signature& signature)
	{
		//Tries to find an existing chunk with given signature
		for (int i = 0; i < chunks.size(); i++)
		{
			if (chunks[i]->getSignature() == signature)
			{
				return chunks[i];
			}
		}

		//If not creates new chunk

		auto& chunk = std::make_shared<Chunk>();
		chunk->setSignature(signature);

		//Add needed components
		for (ComponentType i = 0; i < signature.size(); i++)
		{
			if (signature.test(i))
			{
				auto it = chunkArraysRegister.find(i);
				if (it != chunkArraysRegister.end())
				{
					auto& chunkArray = it->second();
					chunk->addChunkArray(componentNames[i], chunkArray);
				}
			}
		}

		chunk->setSignature(signature);
		return chunk;
	}

	std::unordered_map<ComponentType, std::function<std::shared_ptr<IChunkArray>()>> chunkArraysRegister;
	std::unordered_map<const char*, ComponentType> componentTypes;
	std::unordered_map<ComponentType, const char*> componentNames;

	std::unordered_map<Entity_t, std::shared_ptr<Chunk>> entityToChunk;
	std::vector<std::shared_ptr<Chunk>> chunks;
	ComponentType registeredComponents;
};