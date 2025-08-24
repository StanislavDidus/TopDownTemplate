#include "QuestSystem.h"

QuestSystem::QuestSystem(Tmpl8::Sprite* questIcon, Tmpl8::Sprite* cQuestIcon) : questIcon(questIcon), cQuestIcon(cQuestIcon)
{
}

QuestSystem::~QuestSystem()
{
}

void QuestSystem::addNewQuest(const std::string& name, const std::string& description, const std::function<void()> complete, const std::function<bool()>& condition)
{
	quests[name] = std::make_shared<Quest>(name, description, complete, condition);
	questQueue.push(quests[name]);
}

void QuestSystem::completeQuest(const std::string& name)
{
	quests[name]->complete();
	questQueue.push(quests[name]);
}

void QuestSystem::update(float deltaTime)
{
	for (const auto& [name, quest] : quests)
	{
		if (quest->condition != nullptr)
			if (quest->condition())
			{
				quest->complete();
				questQueue.push(quest);
			}
	}

	if (currentQuest == nullptr && questQueue.size() > 0)
	{
		currentQuest = questQueue.front();
	}

	if (currentQuest != nullptr)
		timer += deltaTime;
	if (timer >= time)
	{
		questQueue.pop();
		currentQuest = nullptr;
		timer = 0.f;
	}
}

void QuestSystem::render(Tmpl8::Surface* screen)
{
	//Display when a player gets new quests or ends ones
	if (currentQuest != nullptr)
	{
		if(!currentQuest->isCompleted())
			questIcon->DrawScaled(200, 384, 64, 64, screen);
		else if (currentQuest->isCompleted())
			cQuestIcon->DrawScaled(200, 384, 64, 64, screen);

		screen->Bar(264, 384, 600, 448, Tmpl8::Pixel(0x0f0f0f));

		std::string str = currentQuest->getName();
		str += ": " + currentQuest->getDescription();

		screen->PrintScaled(&str[0], 264,384,2.5f,2.5f,336, Tmpl8::Pixel(0xFFFFFF));
	}

}

void QuestSystem::ShowQuestStatus(std::shared_ptr<Quest> quest)
{
	
}
