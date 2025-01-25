#ifndef UTILS_LOGGER_LOGGERFORMAT_H_
#define UTILS_LOGGER_LOGGERFORMAT_H_

#include "Static.hpp"

namespace Utils::Logger {
    template<typename... Args>
    inline std::string FormatStringLogs(const std::string &text, Args &&...args) {
        std::ostringstream oss;

        // Check if arguments are given
        if constexpr ((sizeof...(args)) == 0) {
            oss << text;
            return oss.str();
        }

        // std::tuple<std::decay_t<Args>...> tupleArgs = std::make_tuple(std::forward<Args>(args)...);
        // constexpr std::size_t tupleSize = std::tuple_size_v<decltype(tupleArgs)>;
        // const auto tupleSequence = std::make_index_sequence<sizeof...(Args)>{};

        std::size_t textLength = text.size();
        std::size_t index = 0;

        auto helperFormat = [&](auto&& arg) {
            std::size_t position = text.find("{}");

            if (position == std::string::npos)
                return;

            oss << text.substr(index, position) << arg;
            index = position + 2;
        };

        (helperFormat(std::forward<Args>(args)), ...);

        // if (index != textLength)
        //     oss << text.substr(index, textLength - index);

        return oss.str();
    }
}  // namespace Utils::Logger

#endif