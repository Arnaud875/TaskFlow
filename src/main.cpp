#include "Database/Database.hpp"
#include "Utils/Logger/Logger.hpp"

int main() {
    Utils::Logger::Logger &logger = Utils::Logger::Logger::CreateInstance();
    Database::Database &database = Database::Database::CreateInstance();

    LOG_INFO("Hello, {} !", "World");

    // try {
    //     database.Connect();
    //     database.Close();
    // } catch (const std::exception &e) {
    //     LOG_FATAL(e.what());
    // }

    return 0;
}