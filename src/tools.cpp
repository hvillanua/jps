#ifndef TOOLS_CPP
#define TOOLS_CPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iomanip>

#include "grid.hpp"

std::vector<Location> reconstruct_path(
      const Location start,
      const Location goal,
      const std::unordered_map<Location, Location> came_from){
  std::vector<Location> path = {};
  Location current = goal;
  while (current != start){
    path.push_back(current);
    if (came_from.count(current)){
      current = came_from.at(current);
    }
    else{
      break;
    }
  }
  std::reverse(path.begin(), path.end());
  return path;
}

// This outputs a grid. Pass in a distances map if you want to print
// the distances, or pass in a point_to map if you want to print
// arrows that point to the parent location, or pass in a path vector
// if you want to draw the path.
// template<class grid>
void draw_grid(
      const Grid& grid,
      const std::unordered_map<Location, double>* distances=nullptr,
      const std::unordered_map<Location, Location>* point_to=nullptr,
      const std::vector<Location>* path=nullptr,
      const Location* start=nullptr,
      const Location* goal=nullptr) {
  const int field_width = 3;
  std::cout << std::string(field_width * grid.get_width(), '_') << '\n';
  for (int y = 0; y != grid.get_heigth(); ++y) {
    for (int x = 0; x != grid.get_width(); ++x) {
      Location id {x, y};
      if (grid.walls.find(id) != grid.walls.end()) {
        std::cout << std::string(field_width, '#');
      } else if (start && id == *start) {
        std::cout << " A ";
      } else if (goal && id == *goal) {
        std::cout << " Z ";
      } else if (path != nullptr && find(path->begin(), path->end(), id) != path->end()) {
        std::cout << " @ ";
      } else if (point_to != nullptr && point_to->count(id)) {
        Location next = (*point_to).at(id);
        if (next.x == x + 1) { std::cout << " > "; }
        else if (next.x == x - 1) { std::cout << " < "; }
        else if (next.y == y + 1) { std::cout << " v "; }
        else if (next.y == y - 1) { std::cout << " ^ "; }
        else { std::cout << " * "; }
      } else if (distances != nullptr && distances->count(id)) {
        std::cout << ' ' << std::left << std::setw(field_width - 1) << (*distances).at(id);
      } else {
        std::cout << " . ";
      }
    }
    std::cout << '\n';
  }
  std::cout << std::string(field_width * grid.get_width(), '~') << '\n';
}

#endif