#pragma once
#include "Static.hpp"

namespace Utils::Logger {
    template <typename... Args>
    inline std::string FormatStringLogs(const std::string& text, Args&&... args) {
        std::ostringstream oss;
        
        return oss.str();
    }
}