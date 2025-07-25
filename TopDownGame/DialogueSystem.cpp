#include "DialogueSystem.h"

DialogueSystem::DialogueSystem(Player* p, Tmpl8::Sprite* sprite, int px, int py, int sx, int sy, int outLine) : p(p), UIElement(sprite, px, py, sx, sy),
isActive(false), timer(0.f), time(0.f), outLine(outLine)
{

}

DialogueSystem::~DialogueSystem()
{

}

void DialogueSystem::update(float deltaTime)
{
	if (replicQueue.size() > 0 && currentText.empty())
	{
		Replic r = replicQueue.front();

		//Divide words from one string
		std::vector<std::string> str_list;
		std::string temp_str;
		for (int i = 0; i < r.text.size(); i++)
		{
			if (r.text[i] != ' ')
				temp_str.push_back(r.text[i]);
			else
			{
				str_list.push_back(temp_str);
				temp_str.clear();
			}
		}

		currentText = str_list;
		time = r.time;
	}
	else if (!currentText.empty())
	{
		timer += deltaTime;
		//std::cout << timer << "\n";
		isActive = true;
		if (timer >= time)
			GetNextMessage();
	}


}

void DialogueSystem::render(Tmpl8::Surface* screen)
{
	if (isActive)
	{
		if (p->GetPosition().y + p->GetSize().y > 384)
		{
			sprite->DrawScaled(px - outLine, 0, sx + outLine, sy + outLine, screen);
			screen->PrintDialogue(currentText, px, outLine, 2.f, 2.f, sx, sy, Tmpl8::Pixel(0xFFFFFF));
		}
		else
		{
			sprite->DrawScaled(px - outLine, py - outLine, sx + outLine, sy + outLine, screen);
			screen->PrintDialogue(currentText, px, py, 2.f, 2.f, sx, sy, Tmpl8::Pixel(0xFFFFFF));
		}
	}
}

void DialogueSystem::showReplic(const Replic& r)
{
	replicQueue.push(r);
}

void DialogueSystem::showDialogue(const Dialogue& d)
{
	for (const auto& r : d.replics)
	{
		replicQueue.push(r);
	}

}

void DialogueSystem::GetNextMessage()
{
	if (replicQueue.size() > 0)
	{
		replicQueue.pop();
		currentText.clear();
		time = 0.f;
		timer = 0.f;
		isActive = false;
	}

	if (replicQueue.size() > 0)
	{
		isActive = true;
		
		Replic r = replicQueue.front();

		//Divide words from one string
		std::vector<std::string> str_list;
		std::string temp_str;
		for (int i = 0; i < r.text.size(); i++)
		{
			if (r.text[i] != ' ')
				temp_str.push_back(r.text[i]);
			else
			{
				str_list.push_back(temp_str);
				temp_str.clear();
			}
		}

		currentText = str_list;
		time = r.time;
	}
}
