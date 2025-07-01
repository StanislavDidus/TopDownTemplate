#pragma once

#include <cstdio>
#include "surface.h"
#include "UIElement.h"
#include "Replic.h"
#include "Dialogue.h"
#include <queue>
#include <iostream>
#include <vector>


class DialogueSystem : public UIElement
{
public:
	DialogueSystem(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy);
	virtual ~DialogueSystem();

	void update(float deltaTime) override;
	void render(Tmpl8::Surface* screen) override;

	void showReplic(const Replic& r);
	void showDialogue(const Dialogue& d);
private:
	bool isActive;
	std::string currentText;

	std::queue<Replic> replicQueue;

	float timer;
	float time;

	void GetNextMessage();

};

