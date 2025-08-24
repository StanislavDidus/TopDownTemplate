#pragma once

#include <cstdio>
#include "surface.h"
#include "UIElement.h"
#include "Replic.h"
#include "Player.h"
#include "Dialogue.h"
#include <queue>
#include <iostream>
#include <vector>


class DialogueSystem : public UIElement
{
public:
	DialogueSystem(Player* p, Tmpl8::Sprite* sprite, int px, int py, int sx, int sy, int outLine);
	virtual ~DialogueSystem();

	void update(float deltaTime) override;
	void render(Tmpl8::Surface* screen) override;

	void showReplic(Replic* r);
	void showDialogue(Dialogue* d);

	void GetNextMessage();
	void Skip();

	bool isActive;
private:
	 std::vector<std::string> currentText;

	std::queue<Replic*> replicQueue;
	Player* p;

	int outLine;

	float timer;
	float time;
};

