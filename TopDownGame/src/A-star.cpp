#include "A-star.h"

std::array<GridLocation, 8> SquareGrid::DIRS =
{
	GridLocation{16,0}, GridLocation{-16,0},
	GridLocation{0,-16}, GridLocation{0,16},
	GridLocation{16,16}, GridLocation{-16,16},
	GridLocation{-16,-16}, GridLocation{16,-16}
};