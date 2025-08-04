#include "Weapon.h"

Weapon::Weapon(const std::string & iconPath, int damage, float attackSpeed, int value) : Item(iconPath, value), damage(damage), attackSpeed(attackSpeed)
{
	isUsable = false;
	isStackable = false;
	stack = 1;
	maxStackSize = 1;
	ID = 0;
}

Weapon::~Weapon()
{
}
