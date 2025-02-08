#ifndef DATABASE_MODELS_TASKS_
#define DATABASE_MODELS_TASKS_

#include "Database/Models/Model.hpp"
#include "Database/Models/User.hpp"

namespace Database::Models {
    /**
     * @brief A class that represents the tasks table in the database.
     */
    class TasksModel : public BaseModel<TasksModel> {
    public:
        enum class TaskPriority : std::uint8_t { LOW = 0, MEDIUM = 1, HIGH = 2 };
        enum class TaskStatus : std::uint8_t { PENDING = 0, IN_PROGRESS = 1, DONE = 2 };

        /**
         * @brief A struct that contains the attributes of a task.
         */
        struct TaskAttributes {
            int taskId;
            int userId;
            std::string title;
            std::string description;
            TaskPriority priority = TaskPriority::LOW;
            TaskStatus status = TaskStatus::PENDING;
            int limitDate = 0;
            int createdAt = 0;
            int updatedAt = 0;
            bool isPersisted = false;
        };

        TasksModel() = default;

        /**
         * @brief Construct a new Tasks Model object
         *
         * @param params Required attributes to create a new task.
         */
        void CreateImpl(const TaskAttributes &params) {
            if (params.title.empty() || params.description.empty()) {
                throw std::invalid_argument("Title and description must not be empty.");
            }

            taskId_ = params.taskId;
            userId_ = params.userId;
            priority_ = params.priority;
            status_ = params.status;
            limitDate_ = params.limitDate;
            createdAt_ = params.createdAt;
            updatedAt_ = params.updatedAt;
            isPersisted_ = params.isPersisted;

            SetTitle(params.title);
            SetDescription(params.description);
        }

        /**
         * @brief Insert the task into the database if it's not persisted yet or update it.
         */
        bool Save();

        /**
         * @brief Delete the task from the database if it's persisted.
         *
         * @return Return true if the task is deleted successfully.
         */
        bool Delete();

        // Setter
        void SetTitle(const std::string &title);
        void SetDescription(const std::string &description);

        void SetTaskId(int taskId) {
            taskId_ = taskId;
        }

        void SetLimitDate(int limitDate) {
            limitDate_ = limitDate;
        }

        void SetPriority(TaskPriority priority) {
            priority_ = priority;
        }

        void SetStatus(TaskStatus status) {
            status_ = status;
        }

        // Getter
        /**
         * @brief Get the user that owns the task.
         *
         * @return Return the user object.
         */
        UserModel GetUser() const {
            return UserModel::FindByUserId(userId_).value();
        }

        [[nodiscard]] int GetTaskId() const {
            return taskId_;
        }

        [[nodiscard]] int GetUserId() const {
            return userId_;
        }

        [[nodiscard]] std::string GetTitle() const {
            return title_;
        }

        [[nodiscard]] std::string GetDescription() const {
            return description_;
        }

        [[nodiscard]] int GetLimitDate() const {
            return limitDate_;
        }

        [[nodiscard]] TaskPriority GetPriority() const {
            return priority_;
        }

        [[nodiscard]] TaskStatus GetStatus() const {
            return status_;
        }

        [[nodiscard]] int GetCreatedAt() const {
            return createdAt_;
        }

        [[nodiscard]] int GetUpdatedAt() const {
            return updatedAt_;
        }

    private:
        int taskId_;
        int userId_;
        std::string title_;
        std::string description_;
        TaskPriority priority_;
        TaskStatus status_;
        int limitDate_;
        int createdAt_;
        int updatedAt_;
        bool isPersisted_;
    };
};  // namespace Database::Models

#endif