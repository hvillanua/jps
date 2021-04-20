#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include <unordered_set>

struct Location {
  int x, y;

  bool operator==(const Location& a) const
  {
    return x == a.x && y == a.y;
  };
  bool operator!= (const Location a) const
  {
    return !(*this == a);
  };
  bool operator< (const Location a) const
  {
    return std::tie(x, y) < std::tie(a.x, a.y);
  };
  Location operator+ (const Location a) const
  {
    return Location{x + a.x, y + a.y};
  };
  Location operator- (const Location a) const
  {
    return Location{x - a.x, y - a.y};
  };
  std::ostream & operator<<(std::ostream & stream) const
  {
    stream << '(' << x << ", " << y << ')';
    return stream;
  };
};

/* implement hash function so we can put Location into an unordered_set */
namespace std {
  template <> struct hash<Location> {
    typedef Location argument_type;
    typedef std::size_t result_type;
    std::size_t operator()(const Location& id) const noexcept {
      return std::hash<int>()(id.x ^ (id.y << 4));
    }
  };
}

class Grid {
  public:
    
    std::unordered_set<Location> walls;

    Grid(int width_, int height_, std::unordered_set<Location> walls_) : 
      width(width_), height(height_), walls(walls_) {};

    int get_width() const {return width;};

    int get_heigth() const {return height;};

    bool in_bounds(const Location loc) const;

    bool passable(const Location loc) const;

    bool valid(const Location loc) const;

    std::vector<Location> neighbours(const Location current) const;

    std::vector<Location> pruned_neighbours(const Location current, const Location* parent) const;

  private:
    int width, height;
    static std::array<Location, 8> DIRS;
};

#endif