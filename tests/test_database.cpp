#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <filesystem>
#include <iostream>
#include "Utils/Singleton.hpp"
#include "Utils/Logger/Logger.hpp"
#include "Database/Database.hpp"

using namespace Database;
using namespace Utils::Logger;

namespace fs = std::filesystem;

class DatabaseTest {
public:
    void SetUp() {
    }
};

void EndProgram() {
    Database::Database::GetInstance().Close();
}

TEST_CASE("Test database connection", "[database]") {
    Logger& logger = Logger::SafeSingletonCreate("Logger");
    Database::Database& database = Database::Database::SafeSingletonCreate("Database");

    std::atexit(EndProgram);

    if (fs::exists(DATABASE_FILE_NAME)) {
        fs::remove(DATABASE_FILE_NAME);
    }

    REQUIRE_NOTHROW(database.Connect());
}