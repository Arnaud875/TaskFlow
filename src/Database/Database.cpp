#include "Database.hpp"
#include "Static.hpp"
#include <fstream>

using namespace Utils::Logger;

void Database::Database::Connect() {
    const int result = sqlite3_open(DATABASE_FILE_NAME, &database_);
    if (result != SQLITE_OK) {
        throw std::runtime_error("Can't open database: " + std::string(sqlite3_errmsg(database_)));
    }

    // Check if database is already created
    if (!tableExists("Users")) {
        // Execute SQL file to create database schema
        if (!executeSQLFile(DATABASE_SCHEMA_FILE_NAME)) {
            throw std::runtime_error("Can't execute SQL file: Database/Init.sql, Error: " +
                                     Logger::GetLastError());
        }
    }

    LOG_INFO("Database connected successfully");
};

bool Database::Database::tableExists(const std::string &tableName) {
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

bool Database::Database::executeSQLFile(const std::string &fileName) {
    if (fileName.empty()) {
        Logger::SetLastError("File name is empty");
        return false;
    }

    std::ifstream file(fileName);
    if (!file.is_open()) {
        Logger::SetLastError("Can't open file: " + fileName);
        return false;
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
        Logger::SetLastError("SQL error: " + std::string(errorMessage));
        sqlite3_free(errorMessage);
        return false;
    }

    LOG_DEBUG("SQL file executed successfully: " + fileName);
    return true;
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