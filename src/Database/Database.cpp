#include "Database.hpp"
#include "DatabaseHelper.hpp"
#include "Utils/Logger/Logger.hpp"
#include <fstream>

void Database::Database::DeleteRow(const SQLParams &params,
                                   const std::pair<std::string, std::string> &where) const {
    if (params.tableName.empty() || where.first.empty() || where.second.empty()) {
        throw std::invalid_argument("Table name or values are empty");
    }

    if (!database_) {
        throw std::runtime_error("Database is not connected");
    }

    const std::string sql =
        "DELETE FROM " + params.tableName + " WHERE " + where.first + " = " + where.second + ";";
    sqlite3_stmt *stmt = nullptr;

    LOG_DEBUG("Executing SQL statement: " + sql);

    if (sqlite3_prepare_v2(database_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Can't prepare SQL statement: " +
                                 std::string(sqlite3_errmsg(database_)));
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Can't execute SQL statement: " +
                                 std::string(sqlite3_errmsg(database_)));
    }

    sqlite3_finalize(stmt);
}

void Database::Database::UpdateValues(const SQLParams &params,
                                      const std::pair<std::string, std::string> &where) const {
    if (params.tableName.empty() || params.attributes.empty()) {
        throw std::invalid_argument("Table name or values are empty");
    }

    if (where.first.empty() || where.second.empty()) {
        throw std::invalid_argument("Where clause is empty");
    }

    if (!database_) {
        throw std::runtime_error("Database is not connected");
    }

    const auto [sql, sqlValue] = FormatAttributes(params.attributes, true);
    const std::string sqlStr = "UPDATE " + params.tableName + " SET " + sql + " WHERE " +
                               where.first + " = " + where.second + ";";
    sqlite3_stmt *stmt = nullptr;

    LOG_DEBUG("Executing SQL statement: " + sqlStr);

    if (sqlite3_prepare_v2(database_, sqlStr.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Can't prepare SQL statement: " +
                                 std::string(sqlite3_errmsg(database_)));
    }

    std::size_t index = 1;
    for (const auto &attribute : params.attributes) {
        sqlite3_bind_text(stmt, index++, attribute.value.c_str(), -1, SQLITE_STATIC);
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Can't execute SQL statement: " +
                                 std::string(sqlite3_errmsg(database_)));
    }

    sqlite3_finalize(stmt);
}

void Database::Database::InsertValues(const SQLParams &params) const {
    if (params.tableName.empty() || params.attributes.empty()) {
        throw std::invalid_argument("Table name or values are empty");
    }

    if (!database_) {
        throw std::runtime_error("Database is not connected");
    }

    const auto [sql, sqlValue] = FormatAttributes(params.attributes);
    const std::string sqlStr = "INSERT INTO " + params.tableName + sql + " VALUES " + sqlValue;

    LOG_DEBUG("Executing SQL statement: " + sqlStr);

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(database_, sqlStr.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Can't prepare SQL statement: " +
                                 std::string(sqlite3_errmsg(database_)));
    }

    std::size_t index = 1;
    for (const auto &attribute : params.attributes) {
        sqlite3_bind_text(stmt, index++, attribute.value.c_str(), -1, SQLITE_STATIC);
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Can't execute SQL statement: " +
                                 std::string(sqlite3_errmsg(database_)));
    }

    sqlite3_finalize(stmt);
}

std::vector<std::unordered_map<std::string, std::string>>
Database::Database::FindAllRows(const SQLParams &params) const {
    if (params.tableName.empty()) {
        throw std::invalid_argument("Table name or values are empty");
    }

    if (!database_) {
        throw std::runtime_error("Database is not connected");
    }

    const std::string sql = "SELECT * FROM " + params.tableName + ";";
    sqlite3_stmt *stmt = nullptr;

    LOG_DEBUG("Executing SQL statement: " + sql);

    if (sqlite3_prepare_v2(database_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Can't prepare SQL statement: " +
                                 std::string(sqlite3_errmsg(database_)));
    }

    std::vector<std::unordered_map<std::string, std::string>> rows;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::unordered_map<std::string, std::string> row;
        const int columns = sqlite3_column_count(stmt);

        for (int i = 0; i < columns; ++i) {
            const char *columnName = sqlite3_column_name(stmt, i);
            const int columnType = sqlite3_column_type(stmt, i);
            row[columnName] = sqlTypeToString(stmt, columnType, i);
        }

        rows.emplace_back(row);
    }

    sqlite3_finalize(stmt);
    return rows;
}

std::unordered_map<std::string, std::string>
Database::Database::FindRowByAttributes(const SQLParams &params) const {
    if (params.attributes.empty() || params.tableName.empty()) {
        throw std::invalid_argument("Table name or values are empty");
    }

    if (!database_) {
        throw std::runtime_error("Database is not connected");
    }

    const std::string sql =
        "SELECT * FROM " + params.tableName + " WHERE " + params.attributes[0].name + " = ?;";
    sqlite3_stmt *stmt = nullptr;

    LOG_DEBUG("Executing SQL statement: " + sql);

    if (sqlite3_prepare_v2(database_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Can't prepare SQL statement: " +
                                 std::string(sqlite3_errmsg(database_)));
    }

    // Bind value to the statement and use SQLITE_STATIC to indicate that the
    // pointer is constant and will not be deleted
    sqlite3_bind_text(stmt, 1, params.attributes[0].value.c_str(), -1, SQLITE_STATIC);

    // Check if row exists
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return {};
    }

    std::unordered_map<std::string, std::string> rows;
    const int columns = sqlite3_column_count(stmt);

    for (int i = 0; i < columns; ++i) {
        const char *columnName = sqlite3_column_name(stmt, i);
        const int columnType = sqlite3_column_type(stmt, i);
        rows[columnName] = sqlTypeToString(stmt, columnType, i);
    }

    sqlite3_finalize(stmt);
    return rows;
}

void Database::Database::Connect(const std::string &databaseFile) {
    LOG_INFO("Connecting to the local database...");

    if (database_) {
        LOG_WARNING("Database is already connected");
        return;
    }

    const int result = sqlite3_open(databaseFile.c_str(), &database_);
    if (result != SQLITE_OK) {
        throw std::runtime_error("Can't open database: " + std::string(sqlite3_errmsg(database_)));
    }

    // Check if database is already created
    if (!tableExists("Users")) {
        // Execute SQL file to create database schema
        executeSQLFile(DATABASE_SCHEMA_FILE_NAME);
    }

    LOG_INFO("Database connected successfully");
};

bool Database::Database::tableExists(const std::string &tableName) const {
    sqlite3_stmt *stmt = nullptr;
    constexpr const char *SQL = "SELECT count(*) FROM sqlite_master WHERE type='table' AND name=?;";

    if (sqlite3_prepare_v2(database_, SQL, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, tableName.c_str(), -1, SQLITE_STATIC);

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = (sqlite3_column_int(stmt, 0) > 0);
    }

    sqlite3_finalize(stmt);
    return exists;
}

void Database::Database::executeSQLFile(const std::string &fileName) const {
    if (fileName.empty()) {
        throw std::invalid_argument("File name is empty");
    }

    std::ifstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("Can't open file: " + fileName);
    }

    std::string sql;
    std::streamsize size = 0;

    file.seekg(0, std::ios::end);
    size = file.tellg();
    file.seekg(0, std::ios::beg);

    sql.resize(size);
    file.read(sql.data(), size);

    char *errorMessage = nullptr;
    const int result = sqlite3_exec(database_, sql.c_str(), nullptr, nullptr, &errorMessage);
    if (result != SQLITE_OK) {
        sqlite3_free(errorMessage);
        throw std::runtime_error("Can't execute SQL file: " + fileName);
    }

    LOG_DEBUG("SQL file executed successfully: " + fileName);
};

void Database::Database::Close() {
    if (!database_) {
        LOG_WARNING("Database is already closed or not connected");
        return;
    }

    const int result = sqlite3_close(database_);
    if (result != SQLITE_OK) {
        throw std::runtime_error("Can't close database: " + std::string(sqlite3_errmsg(database_)));
    }

    database_ = nullptr;
    LOG_INFO("Database closed successfully");
};