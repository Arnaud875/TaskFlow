#ifndef DATABASE_TASKAMANAGER_
#define DATABASE_TASKAMANAGER_

#include "Database/Database.hpp"
#include "Database/Models/Tasks.hpp"
#include "Database/Models/User.hpp"
#include "Static.hpp"

namespace Database {
    class TaskManager {
    public:
        static std::vector<Models::TasksModel> GetAllTaskByUserId(int userId);
    };
};  // namespace Database

#endif