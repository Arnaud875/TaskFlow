#include "TaskManager.hpp"
#include "Utils/SafeInvoke.hpp"

using namespace Database;
using namespace Models;

std::optional<TasksModel> TaskManager::FindTaskById(int taskId) {
    const auto result = Utils::SafeInvoke(&Database::FindRowByAttributes,
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
        return std::nullopt;
    }
}

std::vector<TasksModel> TaskManager::GetAllTaskByUserId(int userId) {
    std::vector<TasksModel> tasks;
    const auto result =
        Utils::SafeInvoke(&Database::FindAllRows,
                          Database::GetInstance(),
                          SQLParams{"Tasks", {{"user_id", std::to_string(userId)}}});

    if (!result.has_value() || result.value().empty()) {
        return tasks;
    }

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

    return std::move(tasks);
}