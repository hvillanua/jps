#pragma once

#include <unordered_set>
#include <unistd.h>

#include "grid.hpp"
#include "crow_all.h"


class Server
{
    private:
        std::tuple<Location, Location> unpackLocations(const crow::query_string& qs) const;

    public:
        void run();
};

int main(int argc, char *argv[])
{
	Server server {};
    server.run();
    return 0;
}