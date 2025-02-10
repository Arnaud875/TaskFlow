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
    std::string GetCurrentTimeStr();
}  // namespace Utils

#endif