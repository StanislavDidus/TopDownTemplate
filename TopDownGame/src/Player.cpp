#include "Player.h"

Player::Player(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy, Map* map) : Entity(sprite, px, py, sx, sy, 100, 0, 0), map(map), fx(0), fy(0)
{
	inventory = std::make_shared<Inventory>(5);
}

Player::~Player()
{

}

void Player::moveLeft()
{
	fx--;
}

void Player::moveRight()
{
	fx++;
}

void Player::moveUp()
{
	fy--;
}

void Player::moveDown()
{
	fy++;
}

int Clamp(int value, int min, int max)
{
	return std::min(std::max(value, min), max);
}

std::shared_ptr<Inventory> Player::getInventory() const
{
	return inventory;
}

void Player::Attack()
{
	//std::cout << "Attack\n";
}

void Player::giveMoney(int money)
{
	this->money += money;
}

void Player::giveExp(int exp)
{
	this->exp = exp;
}

void Player::update(float deltaTime)
{
	int nx = px + fx;
	int ny = py + fy;
	fx = 0, fy = 0;

	if (map->CheckCollision(nx, py + 36) && map->CheckCollision(nx + sx, py + 36) && map->CheckCollision(nx, py + sy) && map->CheckCollision(nx + sx, py + sy))
		px = nx;
	if (map->CheckCollision(px, ny + 36) && map->CheckCollision(px + sx, ny + 36) && map->CheckCollision(px, ny + sy) && map->CheckCollision(px + sx, ny + sy))
		py = ny;

	px = std::min(std::max(static_cast<int>(px), 0), 800-48);
	py = std::min(std::max(static_cast<int>(py), 0 - 24), 500 - 48);
}