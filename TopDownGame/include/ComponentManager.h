#pragma once

#include "EntityManager.h"
#include <memory>
#include <unordered_set>

class IComponentArray
{
public:
	virtual void entityDestroyed(Entity_t id) = 0;
	virtual bool hasData(Entity_t id) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray
{
public:
	void addData(Entity_t id, T t)
	{
		assert(entityToIndex.find(id) == entityToIndex.end() && "Component was added to the entity more than once");
		
		size_t newSize = size;
		
		entityToIndex[id] = newSize;
		indexToEntity[newSize] = id;
		data[newSize] = t;
		size++;
	}

	void removeData(Entity_t id)
	{
		assert(entityToIndex.find(id) == entityToIndex.end() && "Entity does not exist");
		
		//Copy element at the end to the position of removed entity
		size_t removedIndex = entityToIndex[id];
		size_t lastIndex = size - 1;
		data[removedIndex] = data[lastIndex];

		//Update maps to point to the new spot
		Entity_t lastEnemy = indexToEntity[lastIndex];
		entityToIndex[lastEnemy] = removedIndex;
		indexToEntity[removedIndex] = lastEnemy;

		entityToIndex.erase(id);
		indexToEntity.erase(lastIndex);

		size--;
	}

	T& getData(Entity_t id)
	{
		assert(entityToIndex.find(id) != entityToIndex.end() && "Entity does not exist");
		
		return data[entityToIndex[id]];
	}

	/*void getEntities(std::unordered_set<Entity_t>& entities)
	{
		for (auto& [entity, index] : entityToIndex)
			entities.insert(entity);
	}*/

	bool hasData(Entity_t id) override
	{
		return entityToIndex.find(id) != entityToIndex.end();
	}

	void entityDestroyed(Entity_t id) override
	{
		if (entityToIndex.find(id) != entityToIndex.end())
			removeData(id);
	}
private:
	std::array<T, MAX_ENTITIES> data;
	std::unordered_map<Entity_t, size_t> entityToIndex;
	std::unordered_map<size_t, Entity_t> indexToEntity;

	size_t size;
};

class ComponentManager
{
public:
	template<typename T>
	void registerComponent()
	{
		const char* name = typeid(T).name();

		assert(componentTypes.find(name) == componentTypes.end() && "Component is registered more than once");

		std::shared_ptr<ComponentArray<T>> componentArray = std::make_shared<ComponentArray<T>>();

		componentTypes[name] = registeredComponents;
		componentArrays[name] = componentArray;

		registeredComponents++;
	}

	template<typename T>
	ComponentType getComponentType()
	{
		const char* name = typeid(T).name();

		assert(componentTypes.find(name) != componentTypes.end() && "Component is not registered");

		return componentTypes[name];
	}

	template<typename T>
	void addComponent(Entity_t entity, T component)
	{
		getComponentArray<T>()->addData(entity, component);
	}

	template<typename T>
	void removeComponent(Entity_t entity)
	{
		getComponentArray<T>()->removeData(entity);
	}

	template<typename T>
	T& getComponent(Entity_t entity)
	{
		return getComponentArray<T>()->getData(entity);
	}

	template<typename T>
	bool hasComponent(Entity_t entity)
	{
		return getComponentArray<T>()->hasData(entity);
	}

	template<typename T>
	void getEntities(std::unordered_set<Entity_t>& entities)
	{
		getComponentArray<T>().getEntities(entities)
	}

	template<typename T>
	void getEntitiesWithComponent(std::unordered_set<Entity_t>& entities)
	{
		for (int i = entities.size() - 1; i > 0; i--)
		{
			if (!hasComponent<T>(entities[i]))
				entities.erase(entities[i]);
		}
	}

	void entityDestroyed(Entity_t entity)
	{
		for (auto& pair : componentArrays)
		{
			pair.second->entityDestroyed(entity);
		}
	}
private:
	std::unordered_map<const char*, ComponentType> componentTypes;
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays;

	ComponentType registeredComponents;

	template<typename T>
	std::shared_ptr<ComponentArray<T>> getComponentArray()
	{
		const char* name = typeid(T).name();

		assert(componentTypes.find(name) != componentTypes.end() && "Component is not registered");

		return std::static_pointer_cast<ComponentArray<T>>(componentArrays[name]);
	}
};