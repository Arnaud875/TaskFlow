#include "RouteManager.hpp"

void Api::RouteManager::SetupRoutes() {
    LOG_INFO("Setting up routes...");

    CROW_ROUTE(app_, "/api/test").methods("GET"_method)([]() {
        crow::response res;
        crow::json::wvalue x({{"message", "Welcome to the API!"}});

        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_header("Content-Type", "application/json");
        res.code = 200;
        res.body = x.dump();

        return res;
    });
}