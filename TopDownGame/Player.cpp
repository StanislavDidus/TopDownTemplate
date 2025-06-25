#include "Player.h"

Player::Player(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy, const std::vector<std::string>& map) : sprite(*sprite), px(px), py(py), sx(sx), sy(sy), map(map)
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

bool Player::CheckCollision(int px, int py)
{
	int tx = px / 32, ty = py / 32;
	if(ty < map.size() && tx * 2 + 1 < map[ty].size())
		return map[ty][tx * 2 + 1] != 'X';
}

void Player::update(float deltaTime)
{
	int nx = px + fx;
	int ny = py + fy;
	fx = 0, fy = 0;

	if (CheckCollision(nx, py) && CheckCollision(nx + sx, py) && CheckCollision(nx, py + sy) && CheckCollision(nx + sx, py + sy))
		px = nx;
	if (CheckCollision(px, ny) && CheckCollision(px + sx, ny) && CheckCollision(px, ny + sy) && CheckCollision(px + sx, ny + sy))
		py = ny;
}

void Player::render(Tmpl8::Surface* screen)
{
	sprite.DrawScaled(px, py, sx, sy, screen);
}
