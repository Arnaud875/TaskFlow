#include "Utils.hpp"

std::string Utils::GetCurrentTimeStr() {
    // Get the current time
    std::time_t now = std::time(nullptr);
    std::tm *time = std::localtime(&now);

    // Format the time
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", time);

    return std::string(buffer);
}