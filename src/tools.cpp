#include "tools.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "grid.hpp"

using namespace std;


vector<Location> Tool::reconstruct_path(
   const Location& start,
   const Location& goal,
   const unordered_map<Location, Location>& came_from)
{
	vector<Location> path {};
	Location current = goal;
	while(current != start){
		path.push_back(current);
		if(came_from.count(current)){
			current = came_from.at(current);
		}
		else{
			// No solution, return empty path
			return {};
		}
	}
	reverse(path.begin(), path.end());
	return path;
}

// This outputs a grid. Pass in a distances map if you want to print
// the distances, or pass in a point_to map if you want to print
// arrows that point to the parent location, or pass in a path vector
// if you want to draw the path, or pass a came_from map to print jump
// point nodes
void Tool::draw_grid(
   const Grid& grid,
   const unordered_map<Location, double>& distances,
   const unordered_map<Location, Location>& point_to,
   const vector<Location>& path,
   const unordered_map<Location, Location>& came_from,
   const Location& start,
   const Location& goal)
{
	const int field_width = 3;
	cout << string(field_width * grid.get_width(), '_') << '\n';
	for(int y = 0; y != grid.get_heigth(); ++y){
		for(int x = 0; x != grid.get_width(); ++x){
			const Location id {x, y};
			if(grid.walls.find(id) != grid.walls.end()){
				cout << string(field_width, '#');
			}
			else if(start != NoneLoc && id == start){
				cout << " A ";
			}
			else if(goal != NoneLoc && id == goal){
				cout << " Z ";
			}
			else if(!path.empty() && find(path.begin(), path.end(), id) != path.end()){
				cout << " @ ";
			}
			else if(came_from.count(id)){
				cout << " J ";
			}
			else if(point_to.count(id)){
				const auto next = point_to.at(id);
				if(next.x == x + 1) { cout << " > "; }
				else if(next.x == x - 1) { cout << " < "; }
				else if(next.y == y + 1) { cout << " v "; }
				else if(next.y == y - 1) { cout << " ^ "; }
				else { cout << " * "; }
			}
			else if(distances.count(id)){
				cout << ' ' << left << setw(field_width - 1) << distances.at(id);
			}
			else{
				cout << " . ";
			}
		}
		cout << '\n';
	}
	cout << string(field_width * grid.get_width(), '~') << '\n';
}
