#pragma once

#include <unordered_set>

#include "grid.hpp"


const struct DefaultMap
{
    const int width {50};
    const int height {50};
    const std::unordered_set<Location> walls {};
    const Location start {14, 25};
    const Location goal {36, 25};
} defaultMapConfig;

class Server
{
    private:
        Grid map {defaultMapConfig.width, defaultMapConfig.height, defaultMapConfig.walls};
        Location start {defaultMapConfig.start};
        Location goal {defaultMapConfig.goal};

    public:
        void run();
};

int main()
{
	Server server {};
    server.run();
    return 0;
}