#ifndef GRID_CPP
#define GRID_CPP

#include <tuple>

#include "grid.hpp"

// bool Location::operator==(const Location& a) const{
//   return x == a.x && y == a.y;
// }

// bool Location::operator!= (const Location a) const{
//   return !(*this == a);
// }

// bool Location::operator< (const Location a) const{
//   return std::tie(x, y) < std::tie(a.x, a.y);
// }

// Location Location::operator+ (const Location a) const{
//   return Location{x + a.x, y + a.y};
// }

// Location Location::operator- (const Location a) const{
//   return Location{x - a.x, y - a.y};
// }

// std::ostream & Location::operator<<(std::ostream & stream) const{ 
//   stream << '(' << x << ", " << y << ')';
//   return stream;
// }

std::array<Location, 8> Grid::DIRS = {
  /* East, West, North, South */
  /* NE, NW, SE, SW*/
  Location{1, 0}, Location{-1, 0},
  Location{0, -1}, Location{0, 1},
  Location{1, 1}, Location{-1, 1},
  Location{1, -1}, Location{-1, -1}
};

Grid::Grid(int width_, int height_, std::unordered_set<Location> walls_) : 
  width(width_), height(height_), walls(walls_) {};

bool Grid::in_bounds(const Location loc) const{
  return 0 <= loc.x && loc.x < width
      && 0 <= loc.y && loc.y < height;
}

bool Grid::passable(const Location loc) const{
  return walls.find(loc) == walls.end();
}

bool Grid::valid(const Location loc) const{
  return in_bounds(loc) && passable(loc);
}

std::vector<Location> Grid::neighbours(const Location current) const{
  std::vector<Location> results;
  for (Location dir: DIRS){
      Location next = current + dir;
      if (valid(next)){
          results.push_back(next);
      }
  }
  return results;
}

std::vector<Location> Grid::pruned_neighbours(const Location current, const Location* parent) const{
  if (parent == nullptr){
    return neighbours(current);
  }
  std::vector<Location> neighbours;
  const Location dir = current - *parent;
  // Diagonal neighbour
  if (dir.x != 0 && dir.y != 0){
    // Add natural neighbours
    std::array<Location, 3> possible_dirs = {dir, Location{dir.x, 0}, Location{0, dir.y}};
    Location new_neighbour;
    for (const auto move_dir: possible_dirs){
      new_neighbour = current + move_dir;
      if (valid(new_neighbour)){
        neighbours.push_back(new_neighbour);
      }
    }
    // Add forced neighbours
    // Create lateral direction to check for obstacles
    Location dir_x = Location{dir.x, 0};
    Location dir_y = Location{0, dir.y};
    Location candidate1 = *parent + dir_x;
    Location candidate2 = *parent + dir_y;
    if (!valid(candidate1) && valid(candidate1 + dir_x)){
      neighbours.push_back(candidate1 + dir_x);
    }
    if (!valid(candidate2) && valid(candidate2 + dir_y)){
      neighbours.push_back(candidate2 + dir_y);
    }
  }
  // Horizontal or vertical neighbour
  else{
    // Add natural neighbours
    Location new_neighbour = current + dir;
    if (valid(new_neighbour)){
      neighbours.push_back(new_neighbour);
    }
    // Add forced neighbours
    // Create lateral direction to check for obstacles
    Location lateral = Location{dir.y, dir.x};
    if (!valid(current + lateral) && valid(current + lateral + dir)){
      neighbours.push_back(current + lateral + dir);
    }
    if (!valid(current - lateral) && valid(current - lateral + dir)){
      neighbours.push_back(current - lateral + dir);
    }
  }
  return neighbours;
}

std::array<Location, 8> Grid::DIRS = {
  /* East, West, North, South */
  /* NE, NW, SE, SW*/
  Location{1, 0}, Location{-1, 0},
  Location{0, -1}, Location{0, 1},
  Location{1, 1}, Location{-1, 1},
  Location{1, -1}, Location{-1, -1}
};

#endif