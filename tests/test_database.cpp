/**
 * Test for the database connection
 */

#include "Database/TaskManager.hpp"
#include "Utils/Logger/Logger.hpp"
#include "Database/TaskManager.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

using namespace Database::Models;
using namespace Utils::Logger;
using namespace std;

namespace fs = filesystem;

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
    UserModel userModel;

    SECTION("Try to connect to the database") {
        REQUIRE_NOTHROW(databaseConnection.GetDatabase().Connect(DATABASE_FILE_NAME));
    }

    SECTION("Try to insert User model into Database") {
        userModel.Create(UserModel::UserAttributes{
            1,
            "Admin",
            "admin@gmail.com",
            "admin"
        });

        REQUIRE(userModel.Save());
    }

    SECTION("Try to find a row by attribute") {
        unordered_map<string, string> result;
        optional<UserModel> userModel = UserModel::FindByUserId(1);
        REQUIRE(userModel.has_value());
        REQUIRE(userModel.value().GetUserId() == 1);
        REQUIRE(userModel.value().GetUsername() == "Admin");
        REQUIRE(userModel.value().GetEmail() == "admin@gmail.com");
        REQUIRE(userModel.value().CheckPassword("admin"));
    }

    SECTION("Try to get all rows from the database") {
        UserModel userModel;
        userModel.Create(UserModel::UserAttributes{
            2,
            "Soso",
            "soso@gmail.com",
            "soso"
        });

        REQUIRE(userModel.Save());

        decltype(declval<Database::Database>().FindAllRows({})) result; // Au top
        REQUIRE_NOTHROW(result = databaseConnection.GetDatabase().FindAllRows(Database::SQLParams{"Users", {}}));
        REQUIRE(result.size() == 2);
    }

    SECTION("Try to update the User model") {
        UserModel userModel = UserModel::FindByUserId(2).value();
        userModel.SetUsername("Soso2");
        userModel.SetPassword("soso2");

        REQUIRE(userModel.Save());

        userModel = UserModel::FindByUserId(2).value();
        REQUIRE(userModel.GetUsername() == "Soso2");
        REQUIRE(userModel.CheckPassword("soso2"));
    }

    SECTION("Try to delete the User model") {
        UserModel userModel = UserModel::FindByUserId(2).value();
        REQUIRE(userModel.Delete());

        optional<UserModel> result = UserModel::FindByUserId(2);
        REQUIRE_FALSE(result.has_value());
    }

    SECTION("Try to create Task model") {
        UserModel userModel = UserModel::FindByUserId(1).value();
        TasksModel task;

        task.Create(TasksModel::TaskAttributes{
            1,
            userModel.GetUserId(),
            "Task 1",
            "Description 1",
            TasksModel::TaskPriority::LOW,
            TasksModel::TaskStatus::PENDING,
            0,
            0,
            0
        });

        REQUIRE(task.Save());
    }

    SECTION("Try to find a task by id") {
        UserModel userModel = UserModel::FindByUserId(1).value();
        optional<TasksModel> task = Database::TaskManager::FindTaskById(1);

        REQUIRE(task.has_value());
        REQUIRE(task.value().GetTaskId() == 1);
        REQUIRE(task.value().GetTitle() == "Task 1");
        REQUIRE(task.value().GetDescription() == "Description 1");
        REQUIRE(task.value().GetPriority() == TasksModel::TaskPriority::LOW);
        REQUIRE(task.value().GetStatus() == TasksModel::TaskStatus::PENDING);
        REQUIRE(task.value().GetUserId() == userModel.GetUserId());
        REQUIRE(task.value().GetUser().GetUserId() == userModel.GetUserId());
    }

    SECTION("Try to update a task by id") {
        TasksModel task = Database::TaskManager::FindTaskById(1).value();
        task.SetDescription("Description modified");
        task.SetPriority(TasksModel::TaskPriority::HIGH);
        task.SetStatus(TasksModel::TaskStatus::IN_PROGRESS);
        task.SetLimitDate(1000);
        REQUIRE(task.Save());

        task = Database::TaskManager::FindTaskById(1).value();
        REQUIRE(task.GetDescription() == "Description modified");
        REQUIRE(task.GetPriority() == TasksModel::TaskPriority::HIGH);
        REQUIRE(task.GetStatus() == TasksModel::TaskStatus::IN_PROGRESS);
        REQUIRE(task.GetLimitDate() == 1000);
    }

    SECTION("Try to delete a task") {
        TasksModel task = Database::TaskManager::FindTaskById(1).value();
        REQUIRE(task.Delete());

        optional<TasksModel> result = Database::TaskManager::FindTaskById(1);
        REQUIRE_FALSE(result.has_value());
    }

    SECTION("Try to get all tasks of user") {
        UserModel user = UserModel::FindByUserId(1).value();
        vector<TasksModel> implementsTask;
        implementsTask.reserve(10);

        for (int i = 0; i < 10; i++) {
            TasksModel task;
            task.Create(TasksModel::TaskAttributes{
                i + 1,
                user.GetUserId(),
                "Task " + to_string(i + 1),
                "Description " + to_string(i + 1),
                TasksModel::TaskPriority::LOW,
                TasksModel::TaskStatus::PENDING,
                0,
                0,
                0
            });

            REQUIRE(task.Save());
            implementsTask.emplace_back(move(task));
        }

        vector<TasksModel> tasks = Database::TaskManager::GetAllTaskByUserId(user.GetUserId());
        REQUIRE(tasks.size() == 10);

        for (int i = 0; i < 10; i++) {
            REQUIRE(tasks[i].GetTaskId() == implementsTask[i].GetTaskId());
            REQUIRE(tasks[i].GetTitle() == implementsTask[i].GetTitle());
            REQUIRE(tasks[i].GetDescription() == implementsTask[i].GetDescription());
            REQUIRE(tasks[i].GetPriority() == implementsTask[i].GetPriority());
            REQUIRE(tasks[i].GetStatus() == implementsTask[i].GetStatus());
            REQUIRE(tasks[i].GetUserId() == implementsTask[i].GetUserId());
        }
    }

    SECTION("Try to close the database connection") {
        REQUIRE_NOTHROW(databaseConnection.GetDatabase().Close());
    }
}