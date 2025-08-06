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
	std::cout << "Attack\n";
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

	if (CheckCollision(nx, py + 36) && CheckCollision(nx + sx, py + 36) && CheckCollision(nx, py + sy) && CheckCollision(nx + sx, py + sy))
		px = nx;
	if (CheckCollision(px, ny + 36) && CheckCollision(px + sx, ny + 36) && CheckCollision(px, ny + sy) && CheckCollision(px + sx, ny + sy))
		py = ny;

	px = std::min(std::max(px, 0), 800-48);
	py = std::min(std::max(py, 0 - 24), 500 - 48);
}

bool Player::CheckCollision(int px, int py)
{
	const auto& level = map->GetLevelRef(map->GetLevel());
	const auto& layer = level.getLayer("Col");
	
	for (const auto intPoint : layer.getIntGridValPositions(1))
	{
		int tx = px / 32;
		int ty = py / 32;

		if (tx == intPoint.x && ty == intPoint.y)
			return false;
	}
	return true;
}