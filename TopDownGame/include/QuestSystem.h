#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>
#include "surface.h"
#include <memory>
#include <queue>

enum QuestStatus
{
	ACTIVE, COMPLETED, FAILED
};

class Quest
{
public:
	Quest(const std::string& name, const std::string& description, const std::function<bool()>& condition = nullptr) : name(name), description(description), condition(condition), status(QuestStatus::ACTIVE) {}

	virtual ~Quest() {}

	void complete()
	{
		status = QuestStatus::COMPLETED;
	}

	const std::string& getName() { return name; }
	const std::string& getDescription() { return description; }
	QuestStatus getStatus() { return status; }

	std::function<bool()> condition;
private:
	std::string name;
	std::string description;
	QuestStatus status;
};

class QuestLine
{
public:
	QuestLine(const std::string& name, const std::string& description, const std::function<void()>& onComplete)
		: name(name), description(description), onComplete(onComplete), index(-1), status(QuestStatus::ACTIVE)
	{

	}
	virtual ~QuestLine() {}

	
	void addQuest(const std::string& name, const std::string& description, const std::function<bool()>& condition = nullptr) { quests.push_back(std::make_shared<Quest>(name, description, condition)); }
	void process()
	{
		if (status == QuestStatus::COMPLETED)
			return;

		index++;
		if (quests.size() > index)
		{
			currentQuest = quests[index];
		}
		else
		{
			onComplete();
			status = QuestStatus::COMPLETED;
		}
	}	

	const std::vector<std::shared_ptr<Quest>>& getQuests() { return quests; }

	const std::string& getName() { return name; }
	const std::string& getDescription() { return description; }

	void setStatus(QuestStatus status) { this->status = status; }
	QuestStatus getStatus() { return status; }

	std::shared_ptr<Quest> currentQuest;

	int index;
private:
	std::string name;
	std::string description;
	std::function<void()> onComplete;

	std::vector<std::shared_ptr<Quest>> quests;
	QuestStatus status;
};

class QuestMsg
{
public:
	QuestMsg(const std::string& questLineName, const std::string& questDescription, QuestStatus status) : questLineName(questLineName), questDescription(questDescription), status(status) {}
	virtual ~QuestMsg() {}

	std::string questLineName;
	std::string questDescription;
	QuestStatus status;
private:

};

class QuestSystem
{
public:
	QuestSystem(Tmpl8::Sprite* questIcon, Tmpl8::Sprite* cQuestIcon);
	virtual ~QuestSystem();

	void addNewQuestLine(std::shared_ptr<QuestLine> questLine);
	void completeQuest(const std::string& questLineName, const std::string& questName);

	void update(float deltaTime);
	void render(Tmpl8::Surface* screen);
private:
	void ShowCurrentQuest(std::shared_ptr<QuestLine> questLine);
	void ShowNewQuest(std::shared_ptr<QuestLine> questLine);

	Tmpl8::Sprite* questIcon;
	Tmpl8::Sprite* cQuestIcon;
	std::unordered_map<std::string, std::shared_ptr<QuestLine>> questLines;

	std::queue<std::shared_ptr<QuestMsg>> questMsgQueue;
	std::shared_ptr<QuestMsg> currentQuestMsg;
	float time = 3.f;
	float timer = 0.f;
};