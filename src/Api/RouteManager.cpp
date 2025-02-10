#include "RouteManager.hpp"

void Api::RouteManager::SetupRoutes() {
    LOG_INFO("Setting up routes...");

    CROW_ROUTE(app_, "/")([]() {
        return "Hello, World!";
    });
}