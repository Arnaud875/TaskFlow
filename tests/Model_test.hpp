#ifndef DATABASE_MODELS_MODEL_
#define DATABASE_MODELS_MODEL_

#include "Static.hpp"
#include <type_traits>

namespace Database::Models {
    template<typename T, typename = void, typename... Args>
    struct has_create_impl : std::false_type {};

    template<typename T, typename... Args>
    struct has_create_impl<
        T,
        std::void_t<decltype(std::declval<T>().CreateImpl(std::declval<std::decay_t<Args>>()...))>,
        Args...> : std::true_type {};

    template<typename T, typename... Args>
    inline constexpr bool has_create_impl_v = has_create_impl<T, void, Args...>::value;

    template<typename T>
    class BaseModel {
    public:
        BaseModel() = default;
        virtual ~BaseModel() = default;

        template<typename... Args>
        void Create(Args &&...args) {
            static_assert(has_create_impl_v<T, std::decay_t<Args>...>,
                          "Derived class must implement CreateImpl method");
            static_cast<T *>(this)->CreateImpl(std::forward<Args>(args)...);
        }

    protected:
        

        template<typename... Args>
        void CreateImpl(Args &&...args) = delete;
    };
};  // namespace Database::Models

#endif