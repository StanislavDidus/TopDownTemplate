#include "Enemy.h"
#include "Map.h"

Enemy::Enemy(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy, Map* map, int health, int damage, float attackSpeed, int money, int exp) : Entity(sprite, px, py ,sx, sy, health, money, exp), map(map),
damage(damage), attackSpeed(attackSpeed), lastAttackTime(0.f)
{
}

Enemy::~Enemy()
{
}

void Enemy::Attack(std::shared_ptr<Entity> e, float timer)
{
	if (timer - lastAttackTime >= attackSpeed)
	{
		lastAttackTime = timer;
		e->Hit(damage);
	}
}

int Enemy::getDps()
{
	return damage / attackSpeed;
}

void Enemy::update(float deltaTime)
{
}
