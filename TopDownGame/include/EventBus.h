#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>
#include <any>

enum EventType
{
	ATTACK, HIT, HEALED, STUNNED,
	KILLED
};

class EventBus
{
public:
	//Singleton
	static EventBus& Get() {
		static EventBus instance;
		return instance;
	}

	void AddListener(EventType type, std::function<void(int)>& func)
	{
		listeners[type].push_back(func);
	}

	void RemoveListener(EventType type)
	{
		listeners.erase(type);
	}

	void Push(EventType type, int value)
	{
		for (auto& f : listeners[type])
		{
			//(queue.push_back(f);
			f(value);
		}
	}

	/*void process()
	{
		for (auto& f : queue)
		{
			(*f)(1);
		}
		queue.clear();
	}*/
private:
	std::unordered_map<EventType, std::vector<std::function<void(int)>>> listeners;
	//std::vector< std::shared_ptr<std::function<void(int value)>>> queue;

	EventBus() {}
};