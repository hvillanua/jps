#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>

#include "grid.hpp"
#include "tools.hpp"

using namespace std;

typedef pair<double, Location> PQElement;
typedef priority_queue<PQElement, vector<PQElement>, greater<PQElement>> PQLoc;

typedef double(heuristic_fn)(const Location&, const Location&);


inline double heuristic(const Location& a, const Location& b)
{
	return abs(a.x - b.x) + abs(a.y - b.y);
}

Location jump(const Location initial, const Location dir, const Location start, const Location goal)
{
	return Location{0, 0};
}

vector<Location> successors(const Grid& grid, const Location& current,
                                 const Location& parent, const Location& start,
                                 const Location& goal)
{
	vector<Location> successors;
	auto neighbours = grid.pruned_neighbours(current, parent);
	for(const auto& n : neighbours)
	{
		auto jump_point = jump(current, n - current, start, goal);
		if(jump_point != Location{}) successors.push_back(jump_point);
	}
	return successors;
}

unordered_map<Location, Location> a_star(
   const Grid& grid,
   const Location& start, const Location& goal,
   heuristic_fn heuristic)
{

	PQLoc open_set;
	unordered_map<Location, Location> came_from {};
	unordered_map<Location, double> cost_so_far {};

	open_set.emplace(0, start);
	came_from[start] = start;
	cost_so_far[start] = 0;

	while(!open_set.empty())
	{
		const auto current = open_set.top().second;
		open_set.pop();

		if(current == goal) break;
		for(const Location& next : grid.neighbours(current))
		{
			const auto new_cost = cost_so_far[current] + heuristic(current, next);
			if(cost_so_far.find(next) == cost_so_far.end() || new_cost < cost_so_far[next])
			{
				cost_so_far[next] = new_cost;
				came_from[next] = current;
				open_set.emplace(new_cost + heuristic(next, goal), next);
			}
		}
	}
	return came_from;
}


int main()
{
	cout << Location{1, 1} << endl;
	// {
	// {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
	// {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
	// {0, 0, 1, 0, 0, 1, 0, 0, 0, 0},
	// {0, 0, 1, 0, 0, 1, 0, 0, 0, 0},
	// {0, 0, 1, 0, 0, 1, 0, 0, 0, 0},
	// {0, 0, 1, 0, 1, 1, 1, 1, 0, 0},
	// {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
	// {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
	// {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	// {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	// };
	unordered_set<Location> walls {
		{5, 0}, {5, 1}, {2, 2}, {5, 2},
		{2, 3}, {5, 3}, {2, 4}, {5, 4}, {2, 5}, {4, 5},
		{5, 5}, {6, 5}, {7, 5}, {2, 6}, {2, 7}};
	// {2, 0}, {2, 1}, {2, 8}, {2, 9}};
	Grid map {10, 10, walls};

	Location start {1, 1};
	Location goal {6, 2};

	auto came_from = a_star(map, start, goal, heuristic);
	auto path = Tool::reconstruct_path(start, goal, came_from);
	Tool::draw_grid(map, {}, {}, path, start, goal);
}
