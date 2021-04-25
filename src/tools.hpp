#pragma once

#include <cmath>
#include <unordered_map>
#include <vector>

#include "grid.hpp"


namespace Tool
{
	inline double manhattan(const Location& a, const Location& b) { return abs(a.x - b.x) + abs(a.y - b.y); };
	inline double euclidean(const Location& a, const Location& b) { return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)); };
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
