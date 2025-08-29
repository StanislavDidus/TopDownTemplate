#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <array>
#include <unordered_set>
#include <queue>

struct GridLocation
{
	int x, y;

	bool operator==(const GridLocation& other) const { return x == other.x && y == other.y; }
	bool operator!=(const GridLocation& other) const { return !(*this == other); }
	bool operator<(const GridLocation& other) const { return other.x + other.y > x + y; }
	GridLocation operator+(const GridLocation& other) const { return { x + other.x, y + other.y }; }
	GridLocation operator-(const GridLocation& other) const { return { x - other.x, y - other.y }; }
};

struct GridEntity : public GridLocation
{
	int width, height;
};


namespace std {
	/* implement hash function so we can put GridLocation into an unordered_set */
	template <>
	struct hash<GridLocation> {
		std::size_t operator()(const GridLocation& id) const noexcept {
			// I wish built-in std::hash worked on pair and tuple
			return std::hash<int>()(id.x ^ (id.y << 16));
		}
	};
}

struct SquareGrid
{
	static std::array<GridLocation, 8> DIRS;

	int width, height;

	std::unordered_set<GridLocation> walls;
	std::unordered_set<GridLocation> forests;

	SquareGrid(int width, int height) : width(width), height(height)
	{
		//for (int i = 0; i < 15; i++)
		//{
		//	//walls.insert({ 160, i * 16 });
		//}

		//for (int i = 1; i < 20; i++)
		//{
		//	walls.insert({ 320,512 - i * 16 });
		//}

		//for (int i = 0; i < 15; i++)
		//{
		//	walls.insert({ 640, i * 16 });
		//}
	}

	inline bool inBounds(GridLocation id) const
	{
		return  0 <= id.x && id.x < width
			&& 0 <= id.y && id.y < height;
	}

	inline bool isPassable(GridLocation id, int width, int height) const
	{
		for (int i = 0; i < width; i += 16) for (int j = 0; j < height; j += 16)
		{
			if (walls.find(GridLocation{ id.x + i, id.y + j }) != walls.end())
				return false;
		}
		return true;
	}

	inline int cost(GridLocation from, GridLocation to) const
	{
		return forests.find(to) == forests.end() ? 1 : 5;
	}

	void neighbours(GridLocation id, std::vector<GridLocation>& out, int width, int height) const
	{
		for (GridLocation dir : DIRS)
		{
			GridLocation next{ id.x + dir.x, id.y + dir.y };
			if (inBounds(next) && isPassable(next, width, height))
				out.push_back(next);
		}

		if ((id.x + id.y) % 2 == 0) {
			// see "Ugly paths" section for an explanation:
			std::reverse(out.begin(), out.end());
		}
	}

	template<typename T>
	inline double heuristic(T a, T b)
	{
		return std::abs(a.x - b.x) + std::abs(a.y - b.y);
	}

	template<typename Location>
	void search
	(
		Location start,
		Location goal,
		int width,
		int height,
		std::unordered_map<Location, Location>& came_from,
		std::unordered_map<Location, int>& cost_so_far
	)
	{
		num = 0;

		using pqe = std::pair<double, Location>;
		std::priority_queue<pqe, std::vector<pqe>, std::greater<pqe>> frontier;
		frontier.push({ 0,start });

		came_from[start] = start; // came_from[B] = A, A->B
		cost_so_far[start] = 0;

		while (!frontier.empty())
		{
			Location current = frontier.top().second;
			frontier.pop();

			if (current == goal)
				break;

			std::vector<Location> in;
			neighbours(current, in, width, height);
			for (const auto& next : in)
			{
				int new_price = cost_so_far[current] + cost(current, next);
				if (cost_so_far.find(next) == cost_so_far.end() || new_price < cost_so_far[next])
				{
					num++;
					cost_so_far[next] = new_price;
					double priority = cost_so_far[next] + heuristic(goal, next);
					frontier.push({ priority,next });
					came_from[next] = current;
				}
			}
		}
	}
};
