#pragma once

#include <unordered_set>

#include "grid.hpp"
#include "crow_all.h"


const struct DefaultMap
{
    const int width {50};
    const int height {50};
    const std::unordered_set<Location> walls {};
} defaultMapConfig;

class Server
{
    private:
        Grid map {defaultMapConfig.width, defaultMapConfig.height, defaultMapConfig.walls};
        std::tuple<Location, Location> unpackLocations(const crow::query_string& qs) const;

    public:
        void run();
};

int main()
{
	Server server {};
    server.run();
    return 0;
}