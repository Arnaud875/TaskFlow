#ifndef DATABASE_DATABASE_H_
#define DATABASE_DATABASE_H_

#include "Static.hpp"
#include "Utils/Singleton.hpp"
#include <sqlite3.h>

namespace Database {
    /**
     * @brief A struct to hold a SQL attribute
     */
    struct SQLAttribute {
        std::string name;
        std::string value;
    };

    /**
     * @brief A struct to hold SQL parameters
     */
    struct SQLParams {
        std::string tableName;
        std::vector<SQLAttribute> attributes;
    };

    /**
     * @brief The Database class to interact with the SQLite database
     */
    class Database : public Utils::Singleton<Database> {
        friend class Utils::Singleton<Database>;
        Database() = default;

    public:
        /**
         * @brief Connect to the local database
         *
         * @param databaseFile The name of the database file to connect to
         */
        void Connect(const std::string &databaseFile);

        /**
         * @brief Close the database
         */
        void Close();

        /**
         * @brief Create a table in the database
         *
         * @param params The parameters to create the table
         */
        void InsertValues(const SQLParams &params) const;

        /**
         * @brief Update a row in the database
         *
         * @param params The parameters to update
         * @param where The attribute and value to search for
         */
        void UpdateValues(const SQLParams &params,
                          const std::pair<std::string, std::string> &where) const;

        /**
         * @brief Delete a row from the database
         *
         * @param params The parameters to search for
         * @param where The attribute and value to search for
         */
        void DeleteRow(const SQLParams &params,
                       const std::pair<std::string, std::string> &where) const;

        /**
         * @brief Find all rows in the database
         *
         * @param params The parameters to search for
         * @return Returns a vector of rows as maps of attributes and values
         */
        std::vector<std::unordered_map<std::string, std::string>>
        FindAllRows(const SQLParams &params) const;

        /**
         * @brief Find a row in the database by its attributes
         *
         * @param params The parameters to search for
         * @return Returns the row as a map of attributes and values
         */
        std::unordered_map<std::string, std::string>
        FindRowByAttributes(const SQLParams &params) const;

        /**
         * @brief Check if the database is connected
         *
         * @return Return true if the database is connected, false otherwise
         */
        bool IsConnected() const {
            return database_ != nullptr;
        }

    private:
        /**
         * @brief Check if a table exists in the database
         *
         * @param tableName The name of the table to check
         * @return Returns true if the table exists, false otherwise
         */
        bool tableExists(const std::string &tableName) const;

        /**
         * @brief Execute an SQL file
         *
         * @param fileName The name of the file to execute
         */
        void executeSQLFile(const std::string &fileName) const;

        /**
         * @brief Convert a SQL type to a string
         *
         * @param stmt The statement to get the value from
         * @param type The type of the value
         * @param index The index of the value
         * @return Returns the value as a string
         */
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