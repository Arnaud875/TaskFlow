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
        optional<UserModel> userModel = Database::TaskManager::FindUserById(1);
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
        UserModel userModel = Database::TaskManager::FindUserById(2).value();
        userModel.SetUsername("Soso2");
        userModel.SetPassword("soso2");

        REQUIRE(userModel.Save());

        userModel = Database::TaskManager::FindUserById(2).value();
        REQUIRE(userModel.GetUsername() == "Soso2");
        REQUIRE(userModel.CheckPassword("soso2"));
    }

    SECTION("Try to delete the User model") {
        UserModel userModel = Database::TaskManager::FindUserById(2).value();
        REQUIRE(userModel.Delete());

        optional<UserModel> result = Database::TaskManager::FindUserById(2);
        REQUIRE_FALSE(result.has_value());
    }

    SECTION("Try to create Task model") {
        UserModel userModel = Database::TaskManager::FindUserById(1).value();
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
        UserModel userModel = Database::TaskManager::FindUserById(1).value();
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
        UserModel user = Database::TaskManager::FindUserById(1).value();
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

    SECTION("Try to create a tag") {
        TagsModel tag;
        tag.Create(TagsModel::TagAttributes{
            1,
            1,
            "Tag 1",
            "#ff5733"
        });

        REQUIRE(tag.Save());

        tag.Create(TagsModel::TagAttributes{
            2,
            1,
            "Tag 2",
            "#ff5733"
        });

        REQUIRE(tag.Save());
    }

    SECTION("Try to find a tag by id") {
        optional<TagsModel> tag = Database::TaskManager::FindTagById(1);
        REQUIRE(tag.has_value());
        
        REQUIRE(tag.value().GetName() == "Tag 1");
        REQUIRE(tag.value().GetColor() == "#ff5733");

        tag = Database::TaskManager::FindTagById(2);
        REQUIRE(tag.has_value());
        REQUIRE(tag.value().GetName() == "Tag 2");
        REQUIRE(tag.value().GetColor() == "#ff5733");
    }

    SECTION("Try to modify a tag") {
        TagsModel tag = Database::TaskManager::FindTagById(2).value();
        tag.SetName("Tag 2 modified");
        tag.SetColor("#ff5734");

        REQUIRE(tag.Save());

        tag = Database::TaskManager::FindTagById(2).value();
        REQUIRE(tag.GetName() == "Tag 2 modified");
        REQUIRE(tag.GetColor() == "#ff5734");
    }

    SECTION("Try to assign a tag to a task") {
        TasksModel task = Database::TaskManager::FindTaskById(1).value();

        for (int i = 0; i < 10; i++) {
            TagsModel tag;
            tag.Create(TagsModel::TagAttributes{
                i + 3,
                1,
                "Tag " + to_string(i + 3),
                "#ff5733"
            });

            REQUIRE(tag.Save());
            REQUIRE(Database::TaskManager::AssignTagToTask(task.GetTaskId(), tag.GetTagId()));
        }

        TagsModel tag = Database::TaskManager::FindTagById(5).value();
        REQUIRE(Database::TaskManager::AssignTagToTask(task.GetTaskId(), tag.GetTagId()));
    }

    SECTION("Try to get all tags of task") {
        TasksModel task = Database::TaskManager::FindTaskById(1).value();
        optional<vector<TagsModel>> tags;
        
        REQUIRE_NOTHROW(tags = Database::TaskManager::GetAllTagsOfTask(task.GetTaskId()));
        REQUIRE(tags.has_value());
        REQUIRE(tags.value().size() == 10);

        for (int i = 0; i < tags.value().size(); i++) {
            REQUIRE(tags.value()[i].GetTagId() == i + 3);
            REQUIRE(tags.value()[i].GetName() == "Tag " + to_string(i + 3));
            REQUIRE(tags.value()[i].GetColor() == "#ff5733");
        }
    }

    SECTION("Try to delete a tag") {
        TagsModel tag = Database::TaskManager::FindTagById(1).value();
        REQUIRE(tag.Delete());

        optional<TagsModel> result = Database::TaskManager::FindTagById(1);
        REQUIRE_FALSE(result.has_value());

        tag = Database::TaskManager::FindTagById(3).value();
        REQUIRE(tag.Delete());

        result = Database::TaskManager::FindTagById(3);
        REQUIRE_FALSE(result.has_value());

        // Check if the tags are also deleted from the task
        auto tags = Database::TaskManager::GetAllTagsOfTask(1).value();
        REQUIRE(tags.size() == 9);

        for (const auto &t : tags) {
            REQUIRE(t.GetTagId() != 3);
        }
    }

    SECTION("Checks whether when a task is deleted the tags are also deleted") {
        TasksModel task = Database::TaskManager::FindTaskById(1).value();
        REQUIRE(task.Delete());

        optional<TasksModel> result = Database::TaskManager::FindTaskById(1);
        REQUIRE_FALSE(result.has_value());
        REQUIRE(Database::TaskManager::GetAllTagsOfTask(1).value().empty());
    }

    SECTION("Try to get all tasks") {
        int userId = Database::TaskManager::FindUserById(1).value().GetUserId();
        optional<std::vector<TasksModel>> tasks = Database::TaskManager::GetAllTasksOfUser(userId);

        REQUIRE(tasks.has_value());
        REQUIRE(tasks.value().size() == 9);

        for (int i = 0; i < 9; i++) {
            REQUIRE(tasks.value()[i].GetTitle() == "Task " + to_string(i + 2));
            REQUIRE(tasks.value()[i].GetDescription() == "Description " + to_string(i + 2));
            REQUIRE(tasks.value()[i].GetLimitDate() == 0);
        }
    }

    SECTION("Try to get all tags of user") {
        int userId = Database::TaskManager::FindUserById(1).value().GetUserId();
        optional<std::vector<TagsModel>> tags = Database::TaskManager::GetAllTagsOfUser(userId);

        REQUIRE(tags.has_value());
        REQUIRE(tags.value().size() == 10);

        for (int i = 0; i < 9; i++) {
            REQUIRE(tags.value()[i].GetUserId() == userId);
        }
    }

    SECTION("Try to remove tag of task") {
        int userId = Database::TaskManager::FindUserById(1).value().GetUserId();
        TasksModel task;
        task.Create(TasksModel::TaskAttributes{11, userId, "Task 22", "Description 22", TasksModel::TaskPriority::LOW, TasksModel::TaskStatus::PENDING, 0, 0, 0});
        REQUIRE(task.Save());

        TagsModel tag = Database::TaskManager::FindTagById(5).value();
        REQUIRE(Database::TaskManager::AssignTagToTask(task.GetTaskId(), tag.GetTagId()));
        
        REQUIRE(Database::TaskManager::TaskAsTag(task.GetTaskId(), tag.GetTagId()));
        REQUIRE(Database::TaskManager::RemoveTagOfTask(task.GetTaskId(), tag.GetTagId()));
        REQUIRE(Database::TaskManager::RemoveTagOfTask(task.GetTaskId(), tag.GetTagId()));
        REQUIRE_FALSE(Database::TaskManager::TaskAsTag(task.GetTaskId(), tag.GetTagId()));
    }

    SECTION("Try to close the database connection") {
        REQUIRE_NOTHROW(databaseConnection.GetDatabase().Close());
    }
}