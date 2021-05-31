#include <queue>
#include <unordered_map>
#include <vector>

#include "grid.hpp"

using namespace std;

typedef pair<double, Location> PQElement;
typedef priority_queue<PQElement, vector<PQElement>, greater<PQElement>> PQLoc;

typedef double(heuristic_fn)(const Location&, const Location&);


Location jump(const Grid& grid, const Location initial, const Location dir,
	const Location goal);

vector<Location> successors(const Grid& grid, const Location& current,
	const Location& parent, const Location& goal);

unordered_map<Location, Location> jps(
   const Grid& grid,
   const Location& start, const Location& goal,
   heuristic_fn heuristic);

// int main()
// {
// 	unordered_set<Location> walls {
// 		{5, 0}, {5, 1}, {2, 2}, {5, 2},
// 		{2, 3}, {5, 3}, {2, 4}, {5, 4}, {2, 5}, {4, 5},
// 		{5, 5}, {6, 5}, {7, 5}, {2, 6}, {2, 7}};
// 	Grid map {10, 10, walls};

// 	Location start {1, 1};
// 	Location goal {6, 2};

// 	auto came_from = jps(map, start, goal, Tool::euclidean);
// 	auto path = Tool::reconstruct_path(start, goal, came_from);
// 	Tool::draw_grid(map, {}, {}, path, came_from, start, goal);
// }
