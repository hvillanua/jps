#include "grid.hpp"

using namespace std;

const static std::vector<Location> ALL_DIRS {
	/* East, West, North, South */
	/* NE, NW, SE, SW*/
	{1, 0}, {-1, 0},
	{0, -1}, {0, 1},
	{1, 1}, {-1, 1},
	{1, -1}, {-1, -1}
};

Location Location::direction() const
{
	return Location{x>0 ? 1 : (x<0 ? -1 : 0), y>0 ? 1 : (y<0 ? -1 : 0)};
}

bool operator==(const Location& a, const Location& b) noexcept
{
	return a.x == b.x && a.y == b.y;
}

bool operator!=(const Location& a, const Location& b) noexcept
{
	return a.x != b.x || a.y != b.y;
}

bool operator<(const Location& a, const Location& b) noexcept
{
	return std::tie(a.x, a.y) < std::tie(b.x, b.y);
}

Location operator+(const Location& a, const Location& b) noexcept
{
	return {a.x + b.x, a.y + b.y};
}

Location operator-(const Location& a, const Location& b) noexcept
{
	return {a.x - b.x, a.y - b.y};
}

Location operator-(const Location& a) noexcept
{
	return {-a.x, -a.y};
}

Location operator*(const int a, const Location& b) noexcept
{
	return {a * b.x, a * b.y};
}

Location operator*(const Location& a, const int b) noexcept
{
	return {b * a.x, b * a.y};
}

std::ostream& operator<<(std::ostream& os, const Location& a)
{
	return os << "<Location {" << a.x << ", " << a.y << "}>";
}

bool Grid::forced(const Location& loc, const Location& parent, const Location& travel_dir) const{
	const Location dir {(loc - parent).direction()};
	// Diagonal move into diagonal check
	if(travel_dir.x != 0 && travel_dir.y != 0){
		if((dir.x == travel_dir.x && dir.y == -travel_dir.y) || 
	   		(dir.x == -travel_dir.x && dir.y == travel_dir.y)){
			return true;
		}
	}
	// Horizontal or vertical move into diagonal check
	else if(dir.x != 0 && dir.y != 0){
		return true;
	}
	return false;
}

bool Grid::valid_move(const Location& loc, const Location& dir) const{
	const auto next_loc = loc + dir;
	if(dir.x != 0 && dir.y != 0){
		return in_bounds(next_loc) && passable(next_loc)
			&& (passable(loc + Location{dir.x, 0}) || passable(loc + Location{0, dir.y}));
	}
	return in_bounds(next_loc) && passable(next_loc);
};

vector<Location> Grid::neighbours(const Location& current, const std::vector<Location>& dirs) const
{
	vector<Location> results;
	for(auto& dir : dirs){
		if(valid_move(current, dir)){
			results.push_back(current + dir);
		}
	}
	return results;
}

vector<Location> Grid::pruned_neighbours(const Location& current, const Location& parent) const
{
	if(parent == NoneLoc){
		return neighbours(current, ALL_DIRS);
	}
	vector<Location> current_neighbours;
	const auto dir {(current - parent).direction()};
	// Diagonal neighbour
	if(dir.x != 0 && dir.y != 0){
		const Location dir_x {dir.x, 0};
		const Location dir_y {0, dir.y};

		// Add natural neighbours
		current_neighbours = neighbours(current, {dir, dir_x, dir_y});

		// Add forced neighbours
		for(const auto& candidate_dir : {dir_x, dir_y}){
			const auto previous = current - dir;
			if(!valid_move(previous, candidate_dir) && valid_move(previous, 2 * candidate_dir)){
 				current_neighbours.push_back(previous + 2 * candidate_dir);
			}
		}
	}
	// Horizontal or vertical neighbour
	else
	{
		// Add natural neighbours
		current_neighbours = neighbours(current, {dir});

		// Add forced neighbours
		const Location inverted_dir {dir.y, dir.x};
		if(!valid_move(current, inverted_dir) && valid_move(current, inverted_dir + dir)){
			current_neighbours.push_back(current + inverted_dir + dir);
		}
		if(!valid_move(current, -inverted_dir) && valid_move(current, -inverted_dir + dir)){
			current_neighbours.push_back(current - inverted_dir + dir);
		}
	}
	return current_neighbours;
}
