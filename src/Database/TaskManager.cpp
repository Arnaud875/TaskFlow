#include "TaskManager.hpp"

using namespace Database;

std::vector<Models::TasksModel> TaskManager::GetAllTaskByUserId(int userId) {
    std::vector<Models::TasksModel> tasks;
    const auto result =
        Utils::SafeInvoke(&Database::FindAllRows,
                          Database::GetInstance(),
                          SQLParams{"Tasks", {{"user_id", std::to_string(userId)}}});

    if (!result.has_value() || result.value().empty()) {
        return tasks;
    }

    for (const auto &taskResult : result.value()) {
        Models::TasksModel task;

        task.Create(Models::TasksModel::TaskAttributes{
            std::stoi(taskResult.at("id")),
            std::stoi(taskResult.at("user_id")),
            taskResult.at("title"),
            taskResult.at("description"),
            static_cast<Models::TasksModel::TaskPriority>(std::stoi(taskResult.at("priority"))),
            static_cast<Models::TasksModel::TaskStatus>(std::stoi(taskResult.at("status"))),
            std::stoi(taskResult.at("limited_date")),
            std::stoi(taskResult.at("created_at")),
            std::stoi(taskResult.at("updated_at")),
            true});

        tasks.emplace_back(std::move(task));
    }

    return std::move(tasks);
}