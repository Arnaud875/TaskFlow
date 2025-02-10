#ifndef UTILS_META_SAFEINVOKE_H_
#define UTILS_META_SAFEINVOKE_H_

#include "Utils/Logger/Logger.hpp"
#include <iostream>
#include <optional>
#include <type_traits>

namespace Utils::Meta {
    /**
     * @brief Calls a function and catches any exception that may be thrown
     *
     * @tparam Func Type of the function to call
     * @tparam Args Type of the arguments to pass to the function
     * @param function Function to call
     * @param args Arguments to pass to the function
     * @return Returns the result of the function if it is not void, otherwise returns
     * std::monostate
     */
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
