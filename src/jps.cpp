#include <cmath>
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


inline double manhattan(const Location& a, const Location& b)
{
	return abs(a.x - b.x) + abs(a.y - b.y);
}

inline double euclidean(const Location& a, const Location& b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

Location jump(const Grid& grid, const Location initial, const Location dir,
	const Location goal)
{
	auto new_loc = initial + dir;
	if(!grid.valid(new_loc)){
		return NoneLoc;
	}
	if(new_loc == goal){
		return new_loc;
	}
	for(const auto next : grid.pruned_neighbours(new_loc, initial)){
		if(grid.forced(next, new_loc, dir)){
			return new_loc;
		}
	}
	if(dir.x != 0 && dir.y !=0){
		for(const auto& new_dir: {Location{dir.x, 0}, Location{0, dir.y}}){
			auto jump_point {jump(grid, new_loc, new_dir, goal)};
			if(jump_point != NoneLoc){
				return new_loc;
			}
		}
	}
	return jump(grid, new_loc, dir, goal);
}

vector<Location> successors(const Grid& grid, const Location& current,
	const Location& parent, const Location& goal)
{
	vector<Location> successors;
	auto neighbours = grid.pruned_neighbours(current, parent);
	for(const auto& n: neighbours){
		auto jump_point = jump(grid, current, (n - current).direction(), goal);
		if(jump_point != NoneLoc){
			successors.push_back(jump_point);
		}
	}
	return successors;
}

unordered_map<Location, Location> jps(
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
	Location parent {NoneLoc};
	int expanded (0);

	while(!open_set.empty()){
		const auto current = open_set.top().second;
		if(current != start){
			parent = came_from[current];
		}
		open_set.pop();
		expanded++;

		if(current == goal){
			break;
		}
		for(const auto& next : successors(grid, current, parent, goal)){
			const auto new_cost = cost_so_far[current] + heuristic(current, next);
			if(cost_so_far.find(next) == cost_so_far.end() || new_cost < cost_so_far[next]){
				cost_so_far[next] = new_cost;
				came_from[next] = current;
				open_set.emplace(new_cost + heuristic(next, goal), next);
			}
		}
	}
	cout << "Expanded nodes: " << expanded << ". Total nodes: " << expanded + open_set.size() << '\n';
	return came_from;
}


int main()
{
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

	auto came_from = jps(map, start, goal, euclidean);
	auto path = Tool::reconstruct_path(start, goal, came_from);
	// Tool::draw_grid(map, {}, {}, path, start, {}, goal);
	Tool::draw_grid(map, {}, {}, {}, came_from, start, goal);
}
