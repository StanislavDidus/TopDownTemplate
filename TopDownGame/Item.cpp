#include "Item.h"

Item::Item(const std::string& iconPath, int value) : iconPath(iconPath), value(value), isUsable(false), isStackable(false), stack(1), maxStackSize(1), ID(0)
{
}

Item::~Item()
{
}

void Item::onUse()
{
}
