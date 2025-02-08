#ifndef UTILS_SAFE_INVOKE_H_
#define UTILS_SAFE_INVOKE_H_

#include <optional>
#include <type_traits>
#include <iostream>
#include "Logger/Logger.hpp"

namespace Utils {
    template<typename Func, typename... Args>
    std::optional<std::conditional_t<std::is_void_v<std::invoke_result_t<Func, Args...>>,
                                     std::monostate,
                                     std::invoke_result_t<Func, Args...>>>
    SafeInvoke(Func &&function, Args &&...args) {
        using ReturnType = std::invoke_result_t<Func, Args...>;

        if constexpr (std::is_invocable_v<Func, Args...>) {
            try {
                if constexpr (std::is_void_v<ReturnType>) {
                    std::invoke(std::forward<Func>(function), std::forward<Args>(args)...);
                    return std::optional<std::monostate>(std::monostate{});
                } else {
                    return std::optional<ReturnType>(
                        std::invoke(std::forward<Func>(function), std::forward<Args>(args)...));
                }
            } catch (const std::exception &e) {
                if (Logger::Logger::HasInstance()) {
                    LOG_ERROR(e.what());
                } else {
                    std::cerr << e.what() << std::endl;
                }
                return std::nullopt;
            }
        } else {
            return std::nullopt;
        }
    }
}  // namespace Utils

#endif
