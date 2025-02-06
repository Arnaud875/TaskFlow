/**
 * Test for the database connection
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "Utils/Singleton.hpp"
#include "Utils/Logger/Logger.hpp"
#include "Database/Database.hpp"
#include "Database/Models/User.hpp"
#include "Utils/Utils.hpp"

using namespace Database;
using namespace Utils::Logger;

namespace fs = std::filesystem;

inline std::string RandomStr(const int length) {
    static const std::string alphanum =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::string str(length, 0);
    std::generate_n(str.begin(), length, []() -> char {
        return alphanum[rand() % alphanum.size()];
    });

    return str;
}

class DatabaseConnection {
public:
    DatabaseConnection() : 
        logger_(Logger::SafeSingletonCreate("logger")),
        database_(Database::Database::SafeSingletonCreate("database")) {}

    Database::Database& GetDatabase() const {
        return database_;
    }

    static DatabaseConnection& GetInstance() {
        static DatabaseConnection instance;
        return instance;
    }

    ~DatabaseConnection() {
        Database::Database::DestroyInstance();
        Logger::DestroyInstance();
    }

private:
    Database::Database& database_;
    Logger& logger_;
};

TEST_CASE("Test database connection", "[database]") {
    DatabaseConnection& databaseConnection = DatabaseConnection::GetInstance();
    Models::UserModel userModel;

    SECTION("Try to connect to the database") {
        REQUIRE_NOTHROW(databaseConnection.GetDatabase().Connect(DATABASE_FILE_NAME));
    }

    SECTION("Try to insert User model into Database") {
        userModel.Create(Models::UserModel::UserAttributes{
            .userId = 1,
            .username = "Admin",
            .email = "admin@gmail.com",
            .password = "admin"
        });

        REQUIRE(userModel.Save());
    }

    SECTION("Try to find a row by attribute") {
        std::unordered_map<std::string, std::string> result;
        std::optional<Models::UserModel> userModel = Models::UserModel::FindByUserId(1);
        REQUIRE(userModel.has_value());
        REQUIRE(userModel.value().GetUserId() == 1);
        REQUIRE(userModel.value().GetUsername() == "Admin");
        REQUIRE(userModel.value().GetEmail() == "admin@gmail.com");
        REQUIRE(userModel.value().CheckPassword("admin"));
    }

    SECTION("Try to get all rows from the database") {
        Models::UserModel userModel;
        userModel.Create(Models::UserModel::UserAttributes{
            .username = "Soso",
            .email = "soso@gmail.com",
            .password = "soso"
        });

        REQUIRE(userModel.Save());

        decltype(std::declval<Database::Database>().FindAllRows({})) result; // Au top
        REQUIRE_NOTHROW(result = databaseConnection.GetDatabase().FindAllRows(SQLParams{"Users", {}}));
        REQUIRE(result.size() == 2);
    }

    SECTION("Try to update the User model") {
        Models::UserModel userModel = Models::UserModel::FindByUserId(2).value();
        userModel.SetUsername("Soso2");
        userModel.SetPassword("soso2");

        REQUIRE(userModel.Save());

        userModel = Models::UserModel::FindByUserId(2).value();
        REQUIRE(userModel.GetUsername() == "Soso2");
        REQUIRE(userModel.CheckPassword("soso2"));
    }

    SECTION("Try to close the database connection") {
        REQUIRE_NOTHROW(databaseConnection.GetDatabase().Close());
    }
}