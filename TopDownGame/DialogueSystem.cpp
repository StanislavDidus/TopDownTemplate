#include "DialogueSystem.h"

DialogueSystem::DialogueSystem(Player* p,Tmpl8::Sprite* sprite, int px, int py, int sx, int sy, int outLine) : p(p), UIElement(sprite, px, py, sx, sy),
isActive(false), currentText(""), timer(0.f), time(0.f), outLine(outLine)
{

}

DialogueSystem::~DialogueSystem()
{

}

void DialogueSystem::update(float deltaTime)
{
	if (replicQueue.size() > 0 && currentText == "")
	{
		Replic r = replicQueue.front();
		currentText = r.text;
		time = r.time;
	}
	else if(currentText != "")
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
		if (p->GetPosition().y > 384)
		{
			sprite->DrawScaled(px - outLine, 0, sx + outLine, sy + outLine, screen);
			screen->PrintScaled(&currentText[0], px, outLine, 2.f, 2.f, px + sx - 20, Tmpl8::Pixel(0xFFFFFF));
		}
		else
		{
			sprite->DrawScaled(px - outLine, py - outLine, sx + outLine, sy + outLine, screen);
			screen->PrintScaled(&currentText[0], px, py, 2.f, 2.f, px + sx - 20, Tmpl8::Pixel(0xFFFFFF));
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
	replicQueue.pop();
	currentText = "";
	time = 0.f;
	timer = 0.f;
	isActive = false;
}
