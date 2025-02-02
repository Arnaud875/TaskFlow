#ifndef DATABASE_MODELS_MODEL_
#define DATABASE_MODELS_MODEL_

#include "Static.hpp"
#include <type_traits>

namespace Database::Models {
    template<typename T, typename = void, typename... Args>
    struct HasCreateImpl : std::false_type {};

    template<typename T, typename... Args>
    struct HasCreateImpl<
        T,
        std::void_t<decltype(std::declval<T>().CreateImpl(std::declval<std::decay_t<Args>>()...))>,
        Args...> : std::true_type {};

    template<typename T, typename... Args>
    inline constexpr bool HAS_CREATE_IMPL_V = HasCreateImpl<T, void, Args...>::value;

    template<typename T>
    class BaseModel {
    public:
        BaseModel() = default;
        virtual ~BaseModel() = default;

        template<typename... Args>
        void Create(Args &&...args) {
            static_assert(HAS_CREATE_IMPL_V<T, std::decay_t<Args>...>,
                          "Derived class must implement CreateImpl method");
            static_cast<T *>(this)->CreateImpl(std::forward<Args>(args)...);
        }

        [[nodiscard]] std::string GetLastError() const {
            return last_error_;
        };

    protected:
        void SetLastError(const std::string &error) {
            last_error_ = error;
        };

    private:
        std::string last_error_;
    };
};  // namespace Database::Models

#endif