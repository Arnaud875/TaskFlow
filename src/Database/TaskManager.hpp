#ifndef DATABASE_TASKAMANAGER_
#define DATABASE_TASKAMANAGER_

#include "Database/Database.hpp"
#include "Database/Models/Tags.hpp"
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
         * @brief Verify if a task as already a tag.
         * @param taskId The id of the task to verify.
         * @param tagId The id of the tag to verify.
         * @return Return true if the task as the tag, otherwise return false.
         */
        static bool TaskAsTag(int taskId, int tagId);

        /**
         * @brief Remove a tag of a task.
         * @param taskId The id of the task to remove the tag from.
         * @param tagId The id of the tag to remove from the task.
         * @return Return true if the tag was removed from the task, otherwise return false.
         */
        static bool RemoveTagOfTask(int taskId, int tagId);

        /**
         * @brief Assign a tag to a task.
         * @param taskId The id of the task to assign the tag to.
         * @param tagId The id of the tag to assign to the task.
         * @return Return true if the tag was assigned to the task, otherwise return false.
         */
        static bool AssignTagToTask(int taskId, int tagId);

        /**
         * @brief Get all tasks that belong to a specific user.
         * @param userId The id of the user to get tasks for.
         * @return Return a vector of tasks that belong to the user.
         */
        static std::optional<std::vector<Models::TagsModel>> GetAllTagsOfUser(int userId);

        /**
         * @brief Get all tasks that belong to a specific user.
         * @param userId The id of the user to get tasks for.
         * @return Return a vector of tasks that belong to the user.
         */
        static std::optional<std::vector<Models::TasksModel>> GetAllTasksOfUser(int userId);

        /**
         * @brief Get all tags that belong to a specific task.
         * @param taskId The id of the task to get tags for.
         * @return Return a vector of tags that belong to the task.
         */
        static std::optional<std::vector<Models::TagsModel>> GetAllTagsOfTask(int taskId);

        /**
         * @brief Find a user by its id.
         * @param userId The id of the user to find.
         * @return Return the user if found, otherwise return an empty optional.
         */
        static std::optional<Models::UserModel> FindUserById(int userId);

        /**
         * @brief Find a tag by its id.
         * @param tagId The id of the tag to find.
         * @return Return the tag if found, otherwise return an empty optional.
         */
        static std::optional<Models::TagsModel> FindTagById(int tagId);

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