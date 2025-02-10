#include "Tasks.hpp"
#include "Database/TaskManager.hpp"
#include "Utils/Meta/SafeInvoke.hpp"

bool Database::Models::TasksModel::Delete() {
    if (taskId_ == 0) {
        throw std::invalid_argument("The model is not initialized.");
    }

    const auto result = Utils::Meta::SafeInvoke(&Database::DeleteRow,
                                                GetDatabase(),
                                                SQLParams{"Tasks", {}},
                                                std::make_pair("id", std::to_string(taskId_)));
    return static_cast<bool>(result);
}

bool Database::Models::TasksModel::Save() {
    if (taskId_ == 0) {
        throw std::invalid_argument("The model is not initialized.");
    }

    if (!isPersisted_) {
        const SQLParams params = {"Tasks",
                                  {{"user_id", std::to_string(userId_)},
                                   {"title", title_},
                                   {"description", description_},
                                   {"limited_date", std::to_string(limitDate_)},
                                   {"priority", std::to_string(static_cast<int>(priority_))},
                                   {"status", std::to_string(static_cast<int>(status_))}}};

        const auto result = Utils::Meta::SafeInvoke(&Database::InsertValues, GetDatabase(), params);

        if (!result) {
            return false;
        }

        isPersisted_ = true;
    } else {
        const std::optional<TasksModel> currentTaskOpt = TaskManager::FindTaskById(taskId_);
        TasksModel currentTask;

        if (!currentTaskOpt.has_value()) {
            return false;
        }

        currentTask = currentTaskOpt.value();
        SQLParams params = {"Tasks", {}};

        if (currentTask.GetTitle() != title_) {
            params.attributes.emplace_back(SQLAttribute{"title", title_});
        }

        if (currentTask.GetDescription() != description_) {
            params.attributes.emplace_back(SQLAttribute{"description", description_});
        }

        if (currentTask.GetLimitDate() != limitDate_) {
            params.attributes.emplace_back(
                SQLAttribute{"limited_date", std::to_string(limitDate_)});
        }

        if (currentTask.GetPriority() != priority_) {
            params.attributes.emplace_back(
                SQLAttribute{"priority", std::to_string(static_cast<int>(priority_))});
        }

        if (currentTask.GetStatus() != status_) {
            params.attributes.emplace_back(
                SQLAttribute{"status", std::to_string(static_cast<int>(status_))});
        }

        if (params.attributes.empty()) {
            return true;
        }

        const auto result = Utils::Meta::SafeInvoke(&Database::UpdateValues,
                                                    GetDatabase(),
                                                    params,
                                                    std::make_pair("id", std::to_string(taskId_)));
        if (!result) {
            return false;
        }
    }

    return true;
}

void Database::Models::TasksModel::SetTitle(const std::string &title) {
    if (title.empty()) {
        throw std::invalid_argument("Title must not be empty.");
    }

    if (title.size() > 50) {
        throw std::invalid_argument("Title must not exceed 50 characters.");
    }

    title_ = title;
}

void Database::Models::TasksModel::SetDescription(const std::string &description) {
    if (description.empty()) {
        throw std::invalid_argument("Description must not be empty.");
    }

    if (description.size() > 255) {
        throw std::invalid_argument("Description must not exceed 255 characters.");
    }

    description_ = description;
}