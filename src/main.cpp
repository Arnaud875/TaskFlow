#include "Database/Database.hpp"
#include "Utils/Logger/Logger.hpp"
#include "Api/RouteManager.hpp"
#include <crow.h>

int main() {
    Utils::Logger::Logger::SafeSingletonCreate("Logger");
    Database::Database &database = Database::Database::SafeSingletonCreate("Database");

    crow::SimpleApp app;
    if constexpr (DDEBUG) {
        app.loglevel(crow::LogLevel::DEBUG);
    }

    try {
        database.Connect(DATABASE_FILE_NAME);

        Api::RouteManager &routeManager = Api::RouteManager::CreateInstance(app);
        routeManager.SetupRoutes();
    } catch (const std::exception &e) {
        if (database.IsConnected()) {
            database.Close();
        }

        return 1;
    }

    app.port(3000).run();

    database.Close();
    return 0;
}