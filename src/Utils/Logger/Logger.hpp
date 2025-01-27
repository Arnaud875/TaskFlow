#ifndef UTILS_LOGGER_LOGGER_H_
#define UTILS_LOGGER_LOGGER_H_

#include "LoggerFormat.hpp"
#include "Static.hpp"
#include "Utils/Singleton.hpp"
#include "Utils/SourceLocation.hpp"
#include <unordered_map>

#define LOGS(message, level, ...)                                                                  \
    Utils::Logger::Logger::GetInstance().Logs(                                                     \
        message, level, Utils::SourceLocation::current(), ##__VA_ARGS__)

#define LOG_INFO(message, ...) LOGS(message, Utils::Logger::Logger::LogsLevel::INFO, ##__VA_ARGS__)
#define LOG_WARNING(message, ...)                                                                  \
    LOGS(message, Utils::Logger::Logger::LogsLevel::WARNING, ##__VA_ARGS__)
#define LOG_ERROR(message, ...)                                                                    \
    LOGS(message, Utils::Logger::Logger::LogsLevel::ERROR, ##__VA_ARGS__)
#define LOG_DEBUG(message, ...)                                                                    \
    LOGS(message, Utils::Logger::Logger::LogsLevel::DEBUG, ##__VA_ARGS__)

namespace Utils::Logger {
    class Logger : public Singleton<Logger> {
        friend class Singleton<Logger>;
        Logger() = default;

    public:
        enum class LogsLevel { INFO, WARNING, ERROR, DEBUG };
        std::unordered_map<LogsLevel, std::string> logsLevelString = {
            {LogsLevel::INFO, "INFO"},
            {LogsLevel::WARNING, "WARNING"},
            {LogsLevel::ERROR, "ERROR"},
            {LogsLevel::DEBUG, "DEBUG"}};

        template<typename... Args>
        void
        Logs(std::string_view message, LogsLevel level, const SourceLocation &loc, Args &&...args) {
            std::ostringstream oss;
            oss << logsLevelString[level] << " ";
            oss << loc.fileName() << ":";
            oss << loc.functionName() << "(";
            oss << loc.line() << "): ";
            oss << FormatStringLogs(message, std::forward<Args>(args)...);
            std::cout << oss.str() << std::endl;
        }

        ~Logger() = default;
    };
}  // namespace Utils::Logger

#endif