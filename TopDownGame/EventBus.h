#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>

enum EventType
{
	ATTACK, HIT, KILLED, HEALED, STUNNED
};

class EventBus
{
public:
	//Singleton
	static EventBus& Get() {
		static EventBus instance;
		return instance;
	}

	void AddListener(EventType type, std::shared_ptr<std::function<void(int value)>> func)
	{
		listeners[type].push_back(func);
	}

	void RemoveListener(EventType type, std::shared_ptr < std::function<void(int value)>> func)
	{
		auto v = listeners[type];
		v.erase(std::remove_if(v.begin(), v.end(), [&](std::shared_ptr<std::function<void(int value)>> f) {if (f == func) return f; }));
	}

	void Push(EventType type, int value)
	{
		for (auto& f : listeners[type])
		{
			//(queue.push_back(f);
			(*f)(value);
		}
	}

	void process()
	{
		for (auto& f : queue)
		{
			(*f)(1);
		}
		queue.clear();
	}
private:
	std::unordered_map<EventType, std::vector<std::shared_ptr<std::function<void(int value)>>>> listeners;
	std::vector< std::shared_ptr<std::function<void(int value)>>> queue;

	EventBus() {}
};