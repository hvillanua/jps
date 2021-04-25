#pragma once

#include <vector>
#include <unordered_map>

#include "grid.hpp"


namespace Tool
{
	std::vector<Location> reconstruct_path(
		const Location& start,
		const Location& goal,
		const std::unordered_map<Location, Location>& came_from);

	void draw_grid(
		const Grid& grid,
		const std::unordered_map<Location, double>& distances = {},
		const std::unordered_map<Location, Location>& point_to = {},
		const std::vector<Location>& path = {},
		const std::unordered_map<Location, Location>& came_from = {},
		const Location& start = NoneLoc,
		const Location& goal = NoneLoc);
};
