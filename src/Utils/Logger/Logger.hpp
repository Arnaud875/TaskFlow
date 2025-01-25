#ifndef UTILS_LOGGER_LOGGER_H_
#define UTILS_LOGGER_LOGGER_H_

#include "Utils/Singleton.hpp"

namespace Utils::Logger {
    class Logger : public Singleton<Logger> {
        friend class Singleton<Logger>;
        Logger() = default;

    public:
        ~Logger() = default;
    };
}  // namespace Utils::Logger

#endif