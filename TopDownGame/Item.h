#pragma once

#include <cstdio>
#include "surface.h"
#include <iostream>
#include <vector>
#include <functional>

class Item
{
public:
	Item(const std::string& iconPath, int value);
	virtual ~Item();

	virtual void onUse();
	bool isUsable;

	bool isStackable;
	int stack;
	int maxStackSize;

	int value;
	int ID;
private:
	std::string iconPath;
};


