#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>
#include "surface.h"
#include <memory>
#include <queue>

class Quest
{
public:
	Quest(const std::string& name, const std::string& description, const std::function<void()> onComplete, const std::function<bool()>& condition = nullptr) :
		name(name), description(description), condition(condition), onComplete(onComplete), completed(false) {
	}

	virtual ~Quest() {}

	void complete()
	{
		if (!completed)
		{
			onComplete();
			completed = true;
		}
	}

	const std::string& getName() { return name; }
	const std::string& getDescription() { return description; }

	bool isCompleted() { return completed; }

	std::function<bool()> condition;
private:
	std::string name;
	std::string description;
	std::function<void()> onComplete;
	bool completed;
};

class QuestSystem
{
public:
	QuestSystem(Tmpl8::Sprite* questIcon, Tmpl8::Sprite* cQuestIcon);
	virtual ~QuestSystem();

	void addNewQuest(const std::string& name, const std::string& description, const std::function<void()> complete, const std::function<bool()>& condition = nullptr);
	void completeQuest(const std::string& name);

	void update(float deltaTime);
	void render(Tmpl8::Surface* screen);
private:
	void ShowQuestStatus(std::shared_ptr<Quest> quest);

	Tmpl8::Sprite* questIcon;
	Tmpl8::Sprite* cQuestIcon;
	std::unordered_map<std::string, std::shared_ptr<Quest>> quests;

	std::queue<std::shared_ptr<Quest>> questQueue;
	std::shared_ptr<Quest> currentQuest;
	float time = 5.f;
	float timer = 0.f;
};