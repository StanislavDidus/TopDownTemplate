#pragma once

#include <cstdio>
#include "surface.h"
#include "Item.h"
#include <iostream>
#include <vector>

class Inventory
{
public:
	Inventory(int maxCapacity);
	virtual ~Inventory();

	void addItem(std::shared_ptr<Item> item);
	std::vector<std::shared_ptr<Item>> allItems() const;

private:
	std::vector<std::shared_ptr<Item>> items;

	int maxCapacity;
};