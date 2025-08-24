#include "Enemy.h"
#include "Map.h"
#include "EventBus.h"

Enemy::Enemy(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy, Map* map, int neededLevel, int tag, int health, int damage, float attackSpeed, int money, int exp) : Entity(sprite, px, py, sx, sy, health, money, exp), map(map),
damage(damage), attackSpeed(attackSpeed), lastAttackTime(0.f), neededLevel(neededLevel), timer(0.f), x(0.f), y(0.f), tag(tag)
{
	srand(time(NULL));
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

		EventBus::Get().Push(EventType::HIT, damage);
	}
}

int Enemy::getDps()
{
	return damage / attackSpeed;
}

int Enemy::getTag()
{
	return tag;
}

float Clamp(float value, float min, float max)
{
	if (value < min)
		return min;
	if (value > max)
		return max;
	return value;
}

void Enemy::update(float deltaTime)
{
	timer += deltaTime;
	
	float nx, ny;

	nx = px + x / 3;
	ny = py + y / 3;

	if (map->CheckCollision(nx, py + sy) && map->CheckCollision(nx + sx, py + sy) && map->CheckCollision(nx, py) && map->CheckCollision(nx + sx, py))
		px = nx;
	if (map->CheckCollision(px, ny + sy) && map->CheckCollision(px + sx, ny + sy) && map->CheckCollision(px, ny) && map->CheckCollision(px + sx, ny))
		py = ny;

	px = Clamp(px, 0, 800 - sx);
	py = Clamp(py, 0, 512 - sy);

	if (timer >= 0.5f)
	{
		timer = 0.f;
		ChangeDirection();
	}
}

void Enemy::ChangeDirection()
{
	int i = rand() % 21 - 10;
	x = i / 10.f;

	int j = rand() % 21 - 10;
	y = j / 10.f;

	if (x < 0)
		flipped = true;
	else
		flipped = false;
}
