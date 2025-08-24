#include "Inventory.h"

Inventory::Inventory(int maxCapacity) : maxCapacity(maxCapacity)
{
}

Inventory::~Inventory()
{
}

void Inventory::addItem(std::shared_ptr<Item> item)
{
	if (items.size() < maxCapacity)
	{
		if(!item->isStackable)
			items.push_back(item);
		else if(item->isStackable)
		{
			auto it = std::find_if(items.begin(), items.end(),
				[&](const std::shared_ptr<Item>& existing) {
					return existing->ID == item->ID;
				});
			if (it != items.end())
			{
				auto& foundItem = *it;
				if (foundItem->stack < foundItem->maxStackSize)
					foundItem->stack++;
				else
					items.push_back(item);
			}
			else
				items.push_back(item);
		}
	}
}

std::vector<std::shared_ptr<Item>> Inventory::allItems() const
{
	return items;
}
