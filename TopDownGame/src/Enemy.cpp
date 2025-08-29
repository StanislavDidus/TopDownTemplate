#include "Enemy.h"
#include "Map.h"
#include "EventBus.h"

int num = 0;

template<typename T>
inline double distance(T a, T b)
{
	int dx = a.x - b.x;
	int dy = a.y - b.y;
	return sqrt(dx * dx + dy * dy);
}

Enemy::Enemy(Tmpl8::Sprite* sprite, int px, int py, int sx, int sy, Map* map, int neededLevel, int tag, int health, int damage, float attackSpeed, int money, int exp, float movementSpeed) : Entity(sprite, px, py, sx, sy, health, money, exp, movementSpeed), map(map),
damage(damage), attackSpeed(attackSpeed), lastAttackTime(0.f), neededLevel(neededLevel), timer(0.f), x(0.f), y(0.f), tag(tag), currentGoal(nullptr)
{
	srand(time(NULL));
	

	start = { static_cast<int>(px), static_cast<int>(py) };
	goal = { 0,0 };

	ChangeDirection();

	/*start = {int(px), int(py)};
	goal = { 100,400 };
	grid = { 800,512 };*/
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

	/*float nx, ny;

	nx = px + x / 3;
	ny = py + y / 3;

	if (map->CheckCollision(nx, py + sy) && map->CheckCollision(nx + sx, py + sy) && map->CheckCollision(nx, py) && map->CheckCollision(nx + sx, py))
		px = nx;
	if (map->CheckCollision(px, ny + sy) && map->CheckCollision(px + sx, ny + sy) && map->CheckCollision(px, ny) && map->CheckCollision(px + sx, ny))
		py = ny;

	px = Clamp(px, 0, 800 - sx);
	py = Clamp(py, 0, 512 - sy);
	*/

	if (timer >= 2.f)
	{
		timer = 0.f;
		//ChangeDirection();
	}

	if (currentGoal != nullptr)
	{
		Tmpl8::vec2 dir = Tmpl8::vec2{ static_cast<float>(currentGoal->x), static_cast<float>(currentGoal->y) } - Tmpl8::vec2{ px, py };

		int dx = currentGoal->x - static_cast<int>(px);
		int dy = currentGoal->y - static_cast<int>(py);

		px += (dx != 0) ? (dx / abs(dx)) * movementSpeed : 0;
		py += (dy != 0) ? (dy / abs(dy)) * movementSpeed : 0;

		flipped = dx > 0 ? 0 : 1;

		if (distance(*currentGoal, GridLocation{ static_cast<int>(px), static_cast<int>(py) }) == 0)
		{
			NextMark();
		}
	}
}

void Enemy::render(Tmpl8::Surface* screen)
{
	sprite->DrawScaled(static_cast<int>(px), static_cast<int>(py), sx, sy, screen, flipped);

	for (GridLocation l : path)
	{
		//screen->Bar(l.x, l.y, l.x + 16, l.y + 16, Tmpl8::Pixel(0xFFFFFF));
	}

	for (GridLocation l : map->getGrid(neededLevel)->walls)
	{
		//screen->Bar(l.x, l.y, l.x + 16, l.y + 16, Tmpl8::RedMask);
	}
}

void Enemy::ChangeDirection()
{
	/*int i = rand() % 21 - 10;
	x = i / 10.f;

	int j = rand() % 21 - 10;
	y = j / 10.f;

	if (x < 0)
		flipped = true;
	else
		flipped = false;*/

	path.clear();

	goal = GenerateGoal();
	
	start = GridLocation{ static_cast<int>(px), static_cast<int>(py) };
	std::unordered_map<GridLocation, GridLocation> came_from;
	std::unordered_map<GridLocation, int> cost_so_far;

	map->getGrid(neededLevel)->search(start, goal, sx, sy, came_from, cost_so_far);
	std::cout << "Tiles searched: " << num << "\n";
	//std::cout << "Goal is set to: " << goal.x << ", " << goal.y << "\n";

	auto current = goal;
	while (current != start)
	{
		path.push_back(current);
		current = came_from[current];
	}
	path.push_back(start);
	std::reverse(path.begin(), path.end());

	StartFollowing(path);
}

GridLocation Enemy::GenerateGoal()
{
	GridLocation g;
	//g.x = (rand() % 45 + 3) * 16;
	//g.y = (rand() % 27 + 3) * 16;
	g.x = ((rand() % 7*2+1) - 7) * 16 + static_cast<int>(px);
	g.y = ((rand() % 7*2+1) - 7) * 16 + static_cast<int>(py);

	g.x = std::max(0,std::min(g.x,800-64));
	g.y = std::max(0, std::min(g.y, 512-64));

	if (!map->getGrid(neededLevel)->isPassable(g, sx, sy))
	{
		return GenerateGoal();
	}
	return g;
}


void Enemy::StartFollowing(std::vector<GridLocation>& path)
{
	if (path.size() == 0)
	{
		return;
	}

	currentGoal = &path[0];
}

void Enemy::NextMark()
{
	currentGoal = nullptr;
	path.erase(path.begin());
	if (path.size() == 0)
		ChangeDirection();
	else
		StartFollowing(path);
}
