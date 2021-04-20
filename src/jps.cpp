#ifndef JPS_CPP
#define JPS_CPP

#include <iostream>
#include <queue>
#include <vector>

#include "grid.hpp"
#include "tools.cpp"

typedef std::pair<double, Location> PQElement;
typedef std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> PQLoc;

inline double heuristic(const Location a, const Location b) {
  return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

Location jump(const Location initial, const Location dir, const Location start,
              const Location goal){
  return Location{0, 0};
}

std::vector<Location> successors(const Grid &grid, const Location current,
                                 const Location parent, const Location start,
                                 const Location goal){
  std::vector<Location> successors;
  auto neighbours = grid.pruned_neighbours(current, &parent);
  for (const auto &n: neighbours){
    const Location dir = n - current;
    Location jump_point = jump(current, dir, start, goal);
    if (jump_point != Location{}){
      successors.push_back(jump_point);
    }
  }
  return successors;
}

std::unordered_map<Location, Location> a_star(
      const Grid &grid,
      const Location start, const Location goal,
      double (h) (const Location, const Location)){
  
  PQLoc open_set;
  std::unordered_map<Location, Location> came_from = {};
  std::unordered_map<Location, double> cost_so_far = {};
  
  open_set.emplace(0, start);
  came_from[start] = start;
  cost_so_far[start] = 0;

  while (!open_set.empty()){
    const Location current = open_set.top().second;
    open_set.pop();

    if (current == goal){
        break;
    }
    for (const Location &next: grid.neighbours(current)){
        double new_cost = cost_so_far[current] + h(current, next);
        if (cost_so_far.find(next) == cost_so_far.end() || new_cost < cost_so_far[next]){
            cost_so_far[next] = new_cost;
            came_from[next] = current;
            double priority = new_cost + h(next, goal);
            open_set.emplace(priority, next);
        }
    }
  }
  return came_from;
}


int main()
{
  if (__cplusplus == 201703L) std::cout << "C++17\n";
  else if (__cplusplus == 201402L) std::cout << "C++14\n";
  else if (__cplusplus == 201103L) std::cout << "C++11\n";
  else if (__cplusplus == 199711L) std::cout << "C++98\n";
  else std::cout << "pre-standard C++\n";

  const int width = 10;
  const int height = 10;
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
  std::unordered_set<Location> walls = {Location{5, 0}, Location{5, 1}, Location{2, 2}, Location{5, 2},
  Location{2, 3}, Location{5, 3}, Location{2, 4}, Location{5, 4}, Location{2, 5}, Location{4, 5},
  Location{5, 5}, Location{6, 5}, Location{7, 5}, Location{2, 6}, Location{2, 7}};
  // Location{2, 0}, Location{2, 1}, Location{2, 8}, Location{2, 9}};
  Grid map = Grid(width, height, walls);

  Location start = Location{1, 1};
  Location goal = Location{6, 2};

  auto came_from = a_star(map, start, goal, heuristic);
  auto path = reconstruct_path(start, goal, came_from);
  draw_grid(map, nullptr, nullptr, &path, &start, &goal);
}

#endif