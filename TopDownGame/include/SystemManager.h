#pragma once

#include "ChunkManager.h"
#include "EntityManager.h"
#include <set>

class Coordinator;

class System {};

class SystemManager
{
public:
	template<typename T>
	std::shared_ptr<T> registerSystem()
	{
		const char* name = typeid(T).name();

		assert(systems.find(name) == systems.end() && "System is registered more than once");

		std::shared_ptr<T> newSystem = std::make_shared<T>();
		systems[name] = newSystem;
		size++;

		return newSystem;
	}

	template<typename T>
	std::shared_ptr<T> getSystem()
	{
		const char* name = typeid(T).name();

		assert(systems.find(name) != systems.end() && "System is not registered");

		return std::static_pointer_cast<T>(systems[name]);
	}
private:
	std::unordered_map<const char*, std::shared_ptr<System>> systems;

	size_t size;
};