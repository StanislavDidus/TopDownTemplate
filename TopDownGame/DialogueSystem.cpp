#include "DialogueSystem.h"

DialogueSystem::DialogueSystem(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy) : UIElement(sprite, px, py, sx, sy),
isActive(false), currentText(""), timer(0.f), time(0.f)
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
		std::cout << timer << "\n";
		isActive = true;
		if (timer >= time)
			GetNextMessage();
	}
}

void DialogueSystem::render(Tmpl8::Surface* screen)
{
	if (isActive)
	{
		sprite->DrawScaled(px, py, sx, sy, screen);
		screen->PrintScaled(&currentText[0], px, py, 1.5f, 1.5f, Tmpl8::Pixel(0xFFFFFF));
	}
}

void DialogueSystem::showReplic(const Replic& r)
{
	replicQueue.push(r);
}

void DialogueSystem::showDialogue(const Dialogue& d)
{
	
}

void DialogueSystem::GetNextMessage()
{
	replicQueue.pop();
	currentText = "";
	time = 0.f;
	timer = 0.f;
	isActive = false;
}
