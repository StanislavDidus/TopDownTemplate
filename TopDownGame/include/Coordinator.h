#pragma once

#include "Systems.h"

class Coordinator
{
public:
	//Singleton
	static Coordinator& Get()
	{
		static Coordinator instance;
		return instance;
	}

	void init()
	{
		entityManager = std::make_shared<EntityManager>();
		chunkManager = std::make_shared<ChunkManager>();
		systemManager = std::make_shared<SystemManager>();
	}

	Entity_t createEntity()	
	{
		return entityManager->createEntity();
	}

	void destroyEntity(Entity_t entity)
	{
		entityManager->removeEntity(entity);
		chunkManager->entityDestroyed(entity);
	}

	template<typename T>
	void registerComponent()
	{
		chunkManager->registerComponent<T>();
	}

	template<typename T>
	void addComponent(Entity_t entity, T component)
	{
		auto& signature = entityManager->getSignature(entity);

		//Add component to the component system
		chunkManager->addComponent<T>(entity, component, signature);

		//Update signature for the entity
		//signature.set(componentManager->getComponentType<T>(), true);
		entityManager->setSignature(entity, signature);

		//Notify systems about change of signature
		//systemManager->entitySignatureChanged(entity, signature);
	}

	template<typename T>
	void removeComponent(Entity_t entity)
	{
		auto& signature = entityManager->getSignature(entity);
		
		componentManager->removeComponent<T>(entity, signature);

		//signature.set(componentManager->getComponentType<T>(), false);
		entityManager->setSignature(signature);

		//systemManager->entitySignatureChanged(entity, signature);
	}

	template<typename T>
	T& getComponent(Entity_t entity)
	{
		return chunkManager->getComponent<T>(entity);
	}

	void printEntities()
	{
		chunkManager->printEntities();
	}

	template<typename... Ts>
	std::vector<Entity_t> getEntities()
	{
		Signature signature;
		(signature.set(chunkManager->getComponentType<Ts>(), true), ...);
		return chunkManager->getEntities(signature);
	}

	template<typename T>
	ComponentType getComponentType()
	{
		return chunkManager->getComponentType<T>();
	}

	template<typename T>
	std::shared_ptr<T> registerSystem()
	{
		return systemManager->registerSystem<T>();
	}

	template<typename T>
	std::shared_ptr<T> getSystem()
	{
		return systemManager->getSystem<T>();
	}

private:
	std::shared_ptr<EntityManager> entityManager;
	std::shared_ptr<ChunkManager> chunkManager;
	std::shared_ptr<SystemManager> systemManager;
};