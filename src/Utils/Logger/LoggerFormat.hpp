#ifndef UTILS_LOGGER_LOGGERFORMAT_H_
#define UTILS_LOGGER_LOGGERFORMAT_H_

#include <string>
#include <string_view>

namespace Utils::Logger {
    /**
     * @brief Format the string with the arguments
     * @tparam Args The arguments type
     * @param text The text to format
     * @param args The arguments to format
     * @return The formatted string
     */
    template<typename... Args>
    inline std::string FormatStringLogs(std::string_view text, Args &&...args) {
        std::ostringstream oss;
        std::size_t index = 0;
        const std::size_t textLength = text.size();

        // Helper lambda function to format the string
        auto helperFormat = [&](auto &&arg) {
            const std::size_t position = text.find("{}", index);

            if (position == std::string::npos)
                throw std::runtime_error("Error: Not enough arguments for the format string");

            oss << text.substr(index, position - index) << arg;
            index = position + 2;
        };

        (helperFormat(std::forward<Args>(args)), ...);

        if (index != textLength)
            oss << text.substr(index, textLength - index);

        return oss.str();
    }
}  // namespace Utils::Logger

#endif