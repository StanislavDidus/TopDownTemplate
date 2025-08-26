#include "QuestSystem.h"

QuestSystem::QuestSystem(Tmpl8::Sprite* questIcon, Tmpl8::Sprite* cQuestIcon) : questIcon(questIcon), cQuestIcon(cQuestIcon), currentQuestMsg(nullptr)
{
}

QuestSystem::~QuestSystem()
{
}

void QuestSystem::addNewQuestLine(std::shared_ptr<QuestLine> questLine)
{
	questLines[questLine->getName()] = questLine;
	ShowNewQuest(questLine);
}

void QuestSystem::completeQuest(const std::string& questLineName, const std::string& questName)
{
	auto& questLine = questLines[questLineName];

	auto it = std::find_if(questLine->getQuests().begin(), questLine->getQuests().end(), [&](const std::shared_ptr<Quest>& q) {return q->getName() == questName; });
	if (it != questLine->getQuests().end())
	{
		std::shared_ptr<Quest> quest = *it;
		auto& currentQuest = questLine->currentQuest;
		quest->complete();
		if (quest == currentQuest)
			ShowCurrentQuest(questLine);
	}
}

void QuestSystem::update(float deltaTime)
{
	/*for (const auto& [name, questLine] : questLines)
	{	
		for (const auto& quest : questLine->getQuests())
		{
			if (quest->condition != nullptr)
				if (quest->condition())
				{
					quest->complete();
					ShowCurrentQuest(questLine);
				}
		}
	}*/

	if (currentQuestMsg == nullptr && questMsgQueue.size() > 0)
	{
		currentQuestMsg = questMsgQueue.front();
	}

	if (currentQuestMsg != nullptr)
		timer += deltaTime;
	if (timer >= time)
	{
		questMsgQueue.pop();
		currentQuestMsg = nullptr;
		timer = 0.f;
	}
}

void QuestSystem::render(Tmpl8::Surface* screen)
{
	//Display when a player gets new quests or ends ones
	if (currentQuestMsg != nullptr)
	{
		if(currentQuestMsg->status == QuestStatus::ACTIVE)
			questIcon->DrawScaled(200, 384, 64, 64, screen);
		else if (currentQuestMsg->status == QuestStatus::COMPLETED)
			cQuestIcon->DrawScaled(200, 384, 64, 64, screen);

		screen->Bar(264, 384, 600, 448, Tmpl8::Pixel(0x0f0f0f));

		std::string str = currentQuestMsg->questLineName;
		str += ": " + currentQuestMsg->questDescription;

		screen->PrintScaled(&str[0], 264,384,2.5f,2.5f,336, Tmpl8::Pixel(0xFFFFFF));
	}

}

void QuestSystem::ShowCurrentQuest(std::shared_ptr<QuestLine> questLine)
{
	questMsgQueue.push(std::make_shared<QuestMsg>(questLine->getName(), questLine->currentQuest->getDescription(), QuestStatus::COMPLETED));
	ShowNewQuest(questLine);
}

void QuestSystem::ShowNewQuest(std::shared_ptr<QuestLine> questLine)
{
	//Get next quest
	questLine->process();
	if (questLine->getStatus() != QuestStatus::COMPLETED)
	{
		questMsgQueue.push(std::make_shared<QuestMsg>(questLine->getName(), questLine->currentQuest->getDescription(), QuestStatus::ACTIVE));
		//If quest if already completed then we display it
		if (questLine->currentQuest->getStatus() == QuestStatus::COMPLETED)
			ShowCurrentQuest(questLine);
	}
}
