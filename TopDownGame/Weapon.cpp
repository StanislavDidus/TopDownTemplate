#include "Weapon.h"

Weapon::Weapon(const std::string & iconPath, int damage, float attackSpeed, int value) : Item(iconPath, value), damage(damage), attackSpeed(attackSpeed), lastAttackTime(0.f)
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

void Weapon::Attack(std::shared_ptr<Entity> entity)
{
	entity->Hit(damage);
}
