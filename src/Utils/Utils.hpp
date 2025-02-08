#ifndef UTILS_UTILS_H_
#define UTILS_UTILS_H_

#include "Static.hpp"
#include <ctime>
#include <string>

namespace Utils {
    /**
     * @brief Get the current date and time
     * @return The current date and time
     */
    inline std::string GetCurrentTimeStr() {
        // Get the current time
        std::time_t now = std::time(nullptr);
        std::tm *time = std::localtime(&now);

        // Format the time
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", time);

        return std::string(buffer);
    }
}  // namespace Utils

#endif