#ifndef UTILS_UTILS_H_
#define UTILS_UTILS_H_

#include "Static.hpp"

namespace Utils {
    /**
     * @brief Safely invoke a function and catch any exceptions
     * @tparam Func Function type to invoke
     * @tparam Args Arguments type to pass to the function
     * @param function Function to invoke
     * @param args Arguments to pass to the function
     * @return The result of the function
     */
    template<typename Func, typename... Args>
    std::optional<std::conditional_t<std::is_void_v<std::invoke_result_t<Func, Args...>>,
                                     std::monostate,
                                     std::invoke_result_t<Func, Args...>>>
    SafeInvoke(Func &&function, Args &&...args) {
        using ReturnType = std::invoke_result_t<Func, Args...>;

        if constexpr (std::is_invocable_v<Func, Args...>) {
            try {
                // Check if the function is a void function
                if constexpr (std::is_void_v<ReturnType>) {
                    // Invoke the function and return monostate
                    std::invoke(std::forward<Func>(function), std::forward<Args>(args)...);
                    return std::optional<std::monostate>(std::monostate{});  // Return monostate
                } else {
                    // Invoke the function and return the result
                    return std::optional<ReturnType>(
                        std::invoke(std::forward<Func>(function), std::forward<Args>(args)...));
                }
            } catch (const std::exception &e) {
                // Check if logger is available and log the error
                if (Logger::Logger::HasInstance()) {
                    LOG_ERROR(e.what());
                } else {
                    // TODO: Maybe use another way to log the error
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