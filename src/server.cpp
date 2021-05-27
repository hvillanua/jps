#include <iostream>

#include "server.hpp"
#include "crow_all.h"
#include "json.hpp"
#include "tools.hpp"
#include "jps.hpp"

using namespace std;
using namespace nlohmann;


void Server::run()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        return "Server running!";
    });

    CROW_ROUTE(app, "/grid")
    ([&](){
        json j = json{{"start", start}, {"goal", goal}, {"map", map}};
        Tool::draw_grid(map, {}, {}, {}, {}, start, goal);
        return crow::response(j.dump());
    });

    CROW_ROUTE(app, "/grid")
    .methods("PUT"_method)
    ([&](const crow::request& req){
        auto x = crow::json::load(req.body);
        if (!x){
            return crow::response(400);
        }

        auto j = json::parse(req.body);
        map = j.get<Grid>();
        return crow::response{"Saved!"};
    });

    CROW_ROUTE(app, "/grid/clear")
    .methods("PUT"_method)
    ([&](const crow::request& req){
        map = {defaultMapConfig.width, defaultMapConfig.height, defaultMapConfig.walls};
        start = {defaultMapConfig.start};
        goal = {defaultMapConfig.goal};
        return crow::response{"Cleared!"};
    });

    CROW_ROUTE(app, "/run")
    ([&](const crow::request& req){
        auto start_req = req.url_params.get_dict("start");
        auto goal_req = req.url_params.get_dict("goal");
        Location start_aux {};
        Location goal_aux {};
        if(start_req.find("x") != start_req.end() && start_req.find("y") != start_req.end()){
            start_aux = {stoi(start_req["x"]), stoi(start_req["y"])};
        }
        else{
            return crow::response(400);
        }
        if(goal_req.find("x") != goal_req.end() && goal_req.find("y") != goal_req.end()){
            goal_aux = {stoi(goal_req["x"]), stoi(goal_req["y"])};
        }
        else{
            return crow::response(400);
        }

        auto came_from = jps(map, start_aux, goal_aux, Tool::euclidean);
        auto path = Tool::reconstruct_path(start_aux, goal_aux, came_from);
        Tool::draw_grid(map, {}, {}, path, came_from, start_aux, goal_aux);

        json j = json{{"path", path}};
        return crow::response{j.dump()};
    });

    app.port(18080).multithreaded().run();
}