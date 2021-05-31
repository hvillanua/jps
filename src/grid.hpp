#pragma once

#include <iostream>
#include <vector>
#include <unordered_set>

struct Location
{
	int x, y;
	Location direction() const;
	Location flip() const;
};

bool operator==(const Location& a, const Location& b) noexcept;
bool operator!=(const Location& a, const Location& b) noexcept;
bool operator<(const Location& a, const Location& b) noexcept;
Location operator+(const Location& a, const Location& b) noexcept;
Location operator-(const Location& a, const Location& b) noexcept;
Location operator-(const Location& a) noexcept;
Location operator*(const int a, const Location& b) noexcept;
Location operator*(const Location& a, const int b) noexcept;
std::ostream& operator<<(std::ostream& os, const Location& a);

const Location NoneLoc {-1, -1};

/* implement hash function so we can put Location into an unordered_set */
namespace std
{
	template <> struct hash<Location>
	{
		typedef Location argument_type;
		typedef std::size_t result_type;
		std::size_t operator()(const Location& id) const noexcept
		{
			return std::hash<int>()(id.x ^ (id.y << 4));
		}
	};
}

class Grid
{
private:
	int width, height;

public:
	std::unordered_set<Location> walls;

	Grid() {};
	Grid(int width_, int height_, std::unordered_set<Location> walls_) : width(width_), height(height_), walls(walls_) {};

	int get_width() const {return width;};
	int get_heigth() const {return height;};

	bool in_bounds(const Location& loc) const noexcept { return 0 <= loc.x && loc.x < width && 0 <= loc.y && loc.y < height; };
	bool passable(const Location& loc) const { return walls.find(loc) == walls.end(); };
	bool valid_move(const Location& loc, const Location& dir) const;
	bool forced(const Location& loc, const Location& parent, const Location& travel_dir) const;

	std::vector<Location> neighbours(const Location& current) const;
	std::vector<Location> pruned_neighbours(const Location& current, const Location& parent) const;
};
