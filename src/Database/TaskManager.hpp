#ifndef DATABASE_TASKAMANAGER_
#define DATABASE_TASKAMANAGER_

#include "Database/Database.hpp"
#include "Database/Models/Tasks.hpp"
#include "Database/Models/User.hpp"
#include "Static.hpp"

namespace Database {
    /**
     * @brief A class to manage tasks in the database
     */
    class TaskManager {
    public:
        /**
         * @brief Find all tasks that belong to a specific user.
         *
         * @param userId The user id to find tasks for.
         * @return Return a vector of tasks that belong to the user.
         */
        static std::optional<Models::TasksModel> FindTaskById(int taskId);

        /**
         * @brief Get the All Task By UserId
         *
         * @param userId The user id to get all tasks
         * @return Returns a vector of TasksModel
         */
        static std::vector<Models::TasksModel> GetAllTaskByUserId(int userId);
    };
};  // namespace Database

#endif