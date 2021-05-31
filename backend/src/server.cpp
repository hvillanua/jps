#include <iostream>

#include "server.hpp"
#include "json.hpp"
#include "tools.hpp"
#include "jps.hpp"

using namespace std;
using namespace nlohmann;


void Server::run()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
    .methods("GET"_method)
    ([](){
        auto resp {crow::response(json({{"data", "Server running!"}}).dump())};
        resp.add_header("Access-Control-Allow-Origin", "*");
        return resp;
    });

    CROW_ROUTE(app, "/grid")
    ([&](){
        auto resp {crow::response(json{{"map", map}}.dump())};
        resp.add_header("Access-Control-Allow-Origin", "*");
        Tool::draw_grid(map, {}, {}, {}, {}, NoneLoc, NoneLoc);
        return resp;
    });

    CROW_ROUTE(app, "/grid")
    .methods("OPTIONS"_method)
    ([&](){
        auto resp {crow::response()};
        resp.add_header("Access-Control-Allow-Origin", "*");
        resp.add_header("Access-Control-Allow-Methods", "OPTIONS, PUT");
        return resp;
    });

    CROW_ROUTE(app, "/grid")
    .methods("PUT"_method)
    ([&](const crow::request& req){
        auto x {crow::json::load(req.body)};
        if (!x){
            return crow::response(400);
        }
        map = json::parse(req.body).get<Grid>();
        auto resp {crow::response(json{{"map", map}}.dump())};
        resp.add_header("Access-Control-Allow-Origin", "*");
        resp.add_header("Access-Control-Allow-Methods", "OPTIONS, PUT");
        return resp;
    });

    CROW_ROUTE(app, "/grid/clear")
    .methods("OPTIONS"_method)
    ([&](){
        auto resp {crow::response()};
        resp.add_header("Access-Control-Allow-Origin", "*");
        resp.add_header("Access-Control-Allow-Methods", "OPTIONS, PUT");
        return resp;
    });

    CROW_ROUTE(app, "/grid/clear")
    .methods("PUT"_method)
    ([&](const crow::request& req){
        auto resp {crow::response()};
        resp.add_header("Access-Control-Allow-Origin", "*");
        resp.add_header("Access-Control-Allow-Methods", "OPTIONS, PUT");
        auto width {req.url_params.get("width") != nullptr ? stoi(req.url_params.get("width")) : -1};
        auto height {req.url_params.get("height") != nullptr ? stoi(req.url_params.get("height")) : -1};
        cout << "Width: " << width << " Height: " << height << '\n';
        if(width > 0 && height > 0){
            map = {width, height, defaultMapConfig.walls};
        }
        else{
            map = {defaultMapConfig.width, defaultMapConfig.height, defaultMapConfig.walls};
        }
        resp.body = json{{"map", map}}.dump();
        return resp;
    });

    CROW_ROUTE(app, "/run")
    .methods("GET"_method)
    ([&](const crow::request& req){
        auto resp {crow::response()};
        resp.add_header("Access-Control-Allow-Origin", "*");
        auto [start, goal] = unpackLocations(req.url_params);
        if(!map.valid_move(start, {0, 0}) || !map.valid_move(goal, {0, 0})){
            resp.code = 400;
            resp.body = json({
                {"error", true},
                {"errormessage", "Start or goal node are not valid."},
                {"path", {}}}).dump();
            return resp;
        }

        auto came_from {jps(map, start, goal, Tool::euclidean)};
        auto path {Tool::reconstruct_path(start, goal, came_from)};
        Tool::draw_grid(map, {}, {}, path, came_from, start, goal);

        resp.body = json{
            {"error", false},
            {"errormessage", ""},
            {"path", path}}.dump();
        return resp;
    });

    app.port(18080).multithreaded().run();
}

std::tuple<Location, Location> Server::unpackLocations(const crow::query_string& qs) const{
    auto start_x {qs.get("startx")};
    auto start_y {qs.get("starty")};
    auto start {NoneLoc};
    if(start_x != nullptr || start_y != nullptr){
        start = {stoi(start_x), stoi(start_y)};
    }
    auto goal_x {qs.get("goalx")};
    auto goal_y {qs.get("goaly")};
    auto goal {NoneLoc};
    if(goal_x != nullptr || goal_y != nullptr){
        goal = {stoi(goal_x), stoi(goal_y)};
    }
    return tuple(start, goal);
}