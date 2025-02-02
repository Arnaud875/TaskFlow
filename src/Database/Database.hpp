#ifndef DATABASE_DATABASE_H_
#define DATABASE_DATABASE_H_

#include "Utils/Singleton.hpp"
#include <sqlite3.h>

namespace Database {
    class Database : public Utils::Singleton<Database> {
        friend class Utils::Singleton<Database>;
        Database() = default;

    public:
        void Connect();
        void Close();

    private:
        bool tableExists(const std::string &tableName);
        bool executeSQLFile(const std::string &fileName);

        sqlite3 *database_ = nullptr;
    };
}  // namespace Database

#endif