#ifndef UTILS_LOGGER_LOGGER_H_
#define UTILS_LOGGER_LOGGER_H_

#include "LoggerFormat.hpp"
#include "Static.hpp"
#include "Utils/Core/Singleton.hpp"
#include "Utils/Meta/SourceLocation.hpp"
#include "Utils/Utils.hpp"
#include <array>
#include <fstream>

// TODO: La fonction n'est pas vraiment thread-safe de même pour la sauvegarde des logs.

namespace Utils::Logger {
    /**
     * @brief Class that logs messages to the console
     * @note This class has a unique instance
     */
    class Logger : public Core::Singleton<Logger> {
        friend class Core::Singleton<Logger>;
        Logger() = default;

    public:
        /**
         * @brief Enum class for the logs level
         */
        enum class LogsLevel : uint8_t {
            INFO,
            WARNING,
            ERROR,
            FATAL,
            DEBUG,
            COUNT  // Size of the enum
        };

        /**
         * @brief Logs the message to the console
         * @tparam Args Variadic template
         * @param message Message to log
         */
        template<typename... Args>
        void Logs(std::string_view message,
                  LogsLevel level,
                  const Meta::SourceLocation &loc,
                  Args &&...args) noexcept {
            // Pre-allocate memory for the buffer
            static thread_local std::string buffer;
            buffer.clear();
            buffer.reserve(256);

            buffer.append("\033[32m");
            buffer.append(GetCurrentTimeStr());
            buffer.append("\033[0m [");
            buffer.append(logsLevelString[static_cast<size_t>(level)]);
            buffer.append("] [");
            buffer.append(loc.fileName());
            buffer.append(":");
            buffer.append(loc.functionName());
            buffer.append("(");

            // Using snprintf to format integer to string (More optimized
            // than std::to_string)
            char lineBuffer[16];
            const int lineLength = std::snprintf(lineBuffer, sizeof(lineBuffer), "%d", loc.line());

            buffer.append(lineBuffer, lineLength);
            buffer.append(")] ");

            if constexpr (sizeof...(args) > 0) {
                buffer.append(FormatStringLogs(message, std::forward<Args>(args)...));
            } else {
                buffer.append(message);
            }

            buffer.append("\n");
            std::fwrite(buffer.data(), 1, buffer.size(), stdout);

            if (logsBufferIndex_ == logsBuffer_.size()) {
                WriteLogsToFile();
                logsBufferIndex_ = 0;
            }

            logsBuffer_[logsBufferIndex_] = std::move(buffer);
            logsBufferIndex_++;
        }

        ~Logger() {
            WriteLogsToFile(logsBufferIndex_);
        };

    private:
        void WriteLogsToFile(std::size_t endIndex = -1) noexcept {
            // TODO: Peut⁻être largement améliorer
            std::ofstream logsFile("./logs.txt", std::ios::app);
            if (!logsFile.is_open()) {
                std::cerr << "Failed to open logs file" << std::endl;
                return;
            }

            for (std::size_t i = 0; i < logsBuffer_.size(); i++) {
                if (endIndex != -1 && i == endIndex) {
                    break;
                }

                logsFile << logsBuffer_[i];
            }

            logsFile.close();
        }

        std::array<std::string, 100> logsBuffer_;
        std::size_t logsBufferIndex_ = 0;

        static constexpr std::array<std::string_view, static_cast<size_t>(LogsLevel::COUNT)>
            logsLevelString = {"\033[1;34mINFO\033[0m",
                               "\033[33mWARNING\033[0m",
                               "\033[1;31mERROR\033[0m",
                               "\033[31mFATAL\033[0m",
                               "\033[34mDEBUG\033[0m"};
    };

#define LOGS(message, level, ...)                                                                  \
    Utils::Logger::Logger::GetInstance().Logs(                                                     \
        message, level, Utils::Meta::SourceLocation::current(), ##__VA_ARGS__)
#define LOG_INFO(message, ...) LOGS(message, Utils::Logger::Logger::LogsLevel::INFO, ##__VA_ARGS__)
#define LOG_WARNING(message, ...)                                                                  \
    LOGS(message, Utils::Logger::Logger::LogsLevel::WARNING, ##__VA_ARGS__)
#define LOG_ERROR(message, ...)                                                                    \
    LOGS(message, Utils::Logger::Logger::LogsLevel::ERROR, ##__VA_ARGS__)
#define LOG_FATAL(message, ...)                                                                    \
    LOGS(message, Utils::Logger::Logger::LogsLevel::FATAL, ##__VA_ARGS__)
#define LOG_DEBUG(message, ...)                                                                    \
    LOGS(message, Utils::Logger::Logger::LogsLevel::DEBUG, ##__VA_ARGS__)
}  // namespace Utils::Logger

#endif