#ifndef DATABASE_DATABASE_H_
#define DATABASE_DATABASE_H_

#include "Utils/Singleton.hpp"
#include <sqlite3.h>
#include <unordered_map>

namespace Database {
    struct SQLAttribute {
        std::string name;
        std::string value;
    };

    struct SQLParams {
        std::string tableName;
        std::vector<SQLAttribute> attributes;
    };

    class Database : public Utils::Singleton<Database> {
        friend class Utils::Singleton<Database>;
        Database() = default;

    public:
        void Connect(const std::string &databaseFile);
        void Close();

        void InsertValues(const SQLParams &params) const;
        void UpdateValues(const SQLParams &params,
                          const std::pair<std::string, std::string>& where) const;

        std::vector<std::unordered_map<std::string, std::string>>
        FindAllRows(const SQLParams &params) const;

        std::unordered_map<std::string, std::string>
        FindRowByAttributes(const SQLParams &params) const;

        bool IsConnected() const {
            return database_ != nullptr;
        }

    private:
        bool tableExists(const std::string &tableName) const;
        bool executeSQLFile(const std::string &fileName) const;

        static std::string sqlTypeToString(sqlite3_stmt *&stmt, int type, int index) {
            std::string value;

            // Convert column value to string
            switch (type) {
            case SQLITE_INTEGER:
                value = std::to_string(sqlite3_column_int(stmt, index));
                break;
            case SQLITE_FLOAT:
                value = std::to_string(sqlite3_column_double(stmt, index));
                break;
            case SQLITE3_TEXT:
                value = reinterpret_cast<const char *>(sqlite3_column_text(stmt, index));
                break;
            default:
                value = "Unknown";
                break;
            }

            return value;
        };

        sqlite3 *database_ = nullptr;
    };
}  // namespace Database

#endif