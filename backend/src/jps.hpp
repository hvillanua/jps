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
