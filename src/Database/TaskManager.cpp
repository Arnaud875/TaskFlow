#include "TaskManager.hpp"
#include "Utils/Logger/Logger.hpp"
#include "Utils/Meta/SafeInvoke.hpp"
#include <sqlite3.h>

using namespace Database;
using namespace Models;

bool TaskManager::TaskAsTag(int taskId, int tagId) {
    const auto result = Utils::Meta::SafeInvoke(
        &Database::FindRowByAttributes,
        Database::GetInstance(),
        SQLParams{"TaskTags",
                  {{"task_id", std::to_string(taskId)}, {"tag_id", std::to_string(tagId)}}});

    return result.has_value() && !result.value().empty();
}

bool TaskManager::RemoveTagOfTask(int taskId, int tagId) {
    const auto result = Utils::Meta::SafeInvoke(
        &Database::DeleteRow,
        Database::GetInstance(),
        SQLParams{"TaskTags",
                  {{"task_id", std::to_string(taskId)}, {"tag_id", std::to_string(tagId)}}});

    return result.has_value();
}

std::optional<std::vector<TasksModel>> TaskManager::GetAllTasksOfUser(int userId) {
    const auto result =
        Utils::Meta::SafeInvoke(&Database::FindAllRowsByAttributes,
                                Database::GetInstance(),
                                SQLParams{"Tasks", {{"user_id", std::to_string(userId)}}});

    if (!result.has_value() || result.value().empty()) {
        return std::nullopt;
    }

    std::vector<TasksModel> tasks;
    for (const auto &taskResult : result.value()) {
        TasksModel task;
        task.Create(TasksModel::TaskAttributes{
            std::stoi(taskResult.at("id")),
            std::stoi(taskResult.at("user_id")),
            taskResult.at("title"),
            taskResult.at("description"),
            static_cast<TasksModel::TaskPriority>(std::stoi(taskResult.at("priority"))),
            static_cast<TasksModel::TaskStatus>(std::stoi(taskResult.at("status"))),
            std::stoi(taskResult.at("limited_date")),
            std::stoi(taskResult.at("created_at")),
            std::stoi(taskResult.at("updated_at")),
            true});

        tasks.emplace_back(std::move(task));
    }

    return tasks;
}

std::optional<std::vector<Models::TagsModel>> TaskManager::GetAllTagsOfUser(int userId) {
    const auto result =
        Utils::Meta::SafeInvoke(&Database::FindAllRowsByAttributes,
                                Database::GetInstance(),
                                SQLParams{"Tags", {{"user_id", std::to_string(userId)}}});

    if (!result.has_value() || result.value().empty()) {
        return std::nullopt;
    }

    std::vector<TagsModel> tags;
    for (const auto &tagAttr : result.value()) {
        TagsModel tag;
        tag.Create(TagsModel::TagAttributes{std::stoi(tagAttr.at("id")),
                                            std::stoi(tagAttr.at("user_id")),
                                            tagAttr.at("name"),
                                            tagAttr.at("color"),
                                            true});

        tags.emplace_back(std::move(tag));
    }

    return tags;
}

std::optional<std::vector<TagsModel>> TaskManager::GetAllTagsOfTask(int taskId) {
    std::vector<TagsModel> tags;

    std::string sql = "SELECT Tags.* FROM TaskTags JOIN Tags ON TaskTags.tag_id = Tags.id WHERE "
                      "TaskTags.task_id = ?";
    sqlite3_stmt *stmt = nullptr;

    if (sqlite3_prepare_v2(
            Database::GetInstance().GetConnection(), sql.c_str(), -1, &stmt, nullptr) !=
        SQLITE_OK) {
        throw std::runtime_error(
            "Can't prepare SQL statement: " +
            std::string(sqlite3_errmsg(Database::GetInstance().GetConnection())));
    }

    sqlite3_bind_int(stmt, 1, taskId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        TagsModel tag;
        tag.Create(
            TagsModel::TagAttributes{sqlite3_column_int(stmt, 0),
                                     sqlite3_column_int(stmt, 1),
                                     reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)),
                                     reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3)),
                                     true});

        tags.emplace_back(tag);
    }

    sqlite3_finalize(stmt);
    return tags;
}

bool TaskManager::AssignTagToTask(int taskId, int tagId) {
    // Check if tag is already assigned to the task
    const auto isAssigned = Utils::Meta::SafeInvoke(
        &Database::FindRowByAttributes,
        Database::GetInstance(),
        SQLParams{"TaskTags",
                  {{"task_id", std::to_string(taskId)}, {"tag_id", std::to_string(tagId)}}});

    if (isAssigned.has_value() && !isAssigned.value().empty()) {
        return true;
    }

    const auto result = Utils::Meta::SafeInvoke(
        &Database::InsertValues,
        Database::GetInstance(),
        SQLParams{"TaskTags",
                  {{"task_id", std::to_string(taskId)}, {"tag_id", std::to_string(tagId)}}});

    return result.has_value();
}

std::optional<UserModel> TaskManager::FindUserById(int userId) {
    const auto result =
        Utils::Meta::SafeInvoke(&Database::FindRowByAttributes,
                                Database::GetInstance(),
                                SQLParams{"Users", {{"id", std::to_string(userId)}}});

    if (!result.has_value() || result.value().empty()) {
        return std::nullopt;
    }

    try {
        const auto &userResult = result.value();
        UserModel userModel;
        userModel.Create(UserModel::UserAttributes{std::stoi(userResult.at("id")),
                                                   userResult.at("username"),
                                                   userResult.at("email"),
                                                   userResult.at("password"),
                                                   std::stoi(userResult.at("created_at")),
                                                   std::stoi(userResult.at("updated_at")),
                                                   true,
                                                   true});
        return std::move(userModel);
    } catch (const std::exception &e) {
        LOG_ERROR("Failed to find user by id: {}", e.what());
        return std::nullopt;
    }
}

std::optional<TagsModel> TaskManager::FindTagById(int tagId) {
    const auto result = Utils::Meta::SafeInvoke(&Database::FindRowByAttributes,
                                                Database::GetInstance(),
                                                SQLParams{"Tags", {{"id", std::to_string(tagId)}}});

    if (!result.has_value() || result.value().empty()) {
        return std::nullopt;
    }

    try {
        const auto &tagResult = result.value();
        TagsModel tag;
        tag.Create(TagsModel::TagAttributes{tagId,
                                            std::stoi(tagResult.at("user_id")),
                                            tagResult.at("name"),
                                            tagResult.at("color"),
                                            true});
        return std::move(tag);
    } catch (const std::exception &e) {
        LOG_ERROR("Failed to find tag by id: {}", e.what());
        return std::nullopt;
    }
}

std::optional<TasksModel> TaskManager::FindTaskById(int taskId) {
    const auto result =
        Utils::Meta::SafeInvoke(&Database::FindRowByAttributes,
                                Database::GetInstance(),
                                SQLParams{"Tasks", {{"id", std::to_string(taskId)}}});

    if (!result.has_value() || result.value().empty()) {
        return std::nullopt;
    }

    try {
        const auto &taskResult = result.value();
        TasksModel task;

        task.Create(TasksModel::TaskAttributes{
            std::stoi(taskResult.at("id")),
            std::stoi(taskResult.at("user_id")),
            taskResult.at("title"),
            taskResult.at("description"),
            static_cast<TasksModel::TaskPriority>(std::stoi(taskResult.at("priority"))),
            static_cast<TasksModel::TaskStatus>(std::stoi(taskResult.at("status"))),
            std::stoi(taskResult.at("limited_date")),
            std::stoi(taskResult.at("created_at")),
            std::stoi(taskResult.at("updated_at")),
            true});

        return std::move(task);
    } catch (const std::invalid_argument &e) {
        LOG_ERROR("Failed to find task by id: {}", e.what());
        return std::nullopt;
    }
}

std::vector<TasksModel> TaskManager::GetAllTaskByUserId(int userId) {
    std::vector<TasksModel> tasks;
    const auto result =
        Utils::Meta::SafeInvoke(&Database::FindAllRows,
                                Database::GetInstance(),
                                SQLParams{"Tasks", {{"user_id", std::to_string(userId)}}});

    if (!result.has_value() || result.value().empty()) {
        return tasks;
    }

    for (const auto &taskResult : result.value()) {
        TasksModel task;

        try {
            task.Create(TasksModel::TaskAttributes{
                std::stoi(taskResult.at("id")),
                std::stoi(taskResult.at("user_id")),
                taskResult.at("title"),
                taskResult.at("description"),
                static_cast<TasksModel::TaskPriority>(std::stoi(taskResult.at("priority"))),
                static_cast<TasksModel::TaskStatus>(std::stoi(taskResult.at("status"))),
                std::stoi(taskResult.at("limited_date")),
                std::stoi(taskResult.at("created_at")),
                std::stoi(taskResult.at("updated_at")),
                true});
        } catch (const std::exception &e) {
            LOG_ERROR("Failed to get all tasks by user id: {}", e.what());
            continue;
        }

        tasks.emplace_back(std::move(task));
    }

    return std::move(tasks);
}