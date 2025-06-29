#include "Player.h"

Player::Player(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy, Map* map, int& currentLevel) : Entity(sprite, px, py, sx, sy), map(map),
currentLevel(currentLevel), fx(0), fy(0)
{

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
	std::vector<std::string> level = map->GetLevel(currentLevel);
	int tx = px / 32, ty = py / 32;
	if (ty < level.size() && tx * 2 + 1 < level[ty].size())
		return level[ty][tx * 2 + 1] != 'X';
}
