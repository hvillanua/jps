#include <tuple>

#include "grid.hpp"

using namespace std;


const static Location DIRS[] {
	/* East, West, North, South */
	/* NE, NW, SE, SW*/
	{1, 0}, {-1, 0},
	{0, -1}, {0, 1},
	{1, 1}, {-1, 1},
	{1, -1}, {-1, -1}
};


std::ostream& operator<<(std::ostream& os, const Location& a)
{
	return os << "<Location {" << a.x << ", " << a.y << "}>";
};


vector<Location> Grid::neighbours(const Location& current) const
{
	vector<Location> results;
	for(auto& dir : DIRS)
		if(valid(current + dir)) results.push_back(current + dir);
	return results;
}

vector<Location> Grid::pruned_neighbours(const Location& current, const Location& parent) const
{
	if(parent == NoneLoc)
		return neighbours(current);

	vector<Location> neighbours;
	const auto dir = current - parent;
	// Diagonal neighbour
	if(dir.x != 0 && dir.y != 0)
	{
		const Location dir_x {dir.x, 0};
		const Location dir_y {0, dir.y};

		// Add natural neighbours
		for(const auto move_dir : {dir, dir_x, dir_y})
			if(valid(current + move_dir)) neighbours.push_back(current + move_dir);

		// Add forced neighbours
		// Create lateral direction to check for obstacles
		for(const auto& candidate : {parent + dir_x, parent + dir_y})
			if(!valid(candidate) && valid(candidate + dir_x)) neighbours.push_back(candidate + dir_x);
	}
	// Horizontal or vertical neighbour
	else
	{
		// Add natural neighbours
		if(valid(current + dir)) neighbours.push_back(current + dir);

		// Add forced neighbours
		// Create lateral direction to check for obstacles
		const Location inverted_dir {dir.y, dir.x};
		if(!valid(current + inverted_dir) && valid(current + inverted_dir + dir)) neighbours.push_back(current + inverted_dir + dir);
		if(!valid(current - inverted_dir) && valid(current - inverted_dir + dir)) neighbours.push_back(current - inverted_dir + dir);
	}
	return neighbours;
}
