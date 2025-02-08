#ifndef DATABASE_MODELS_MODEL_
#define DATABASE_MODELS_MODEL_

#include "Database/Database.hpp"
#include "Static.hpp"

namespace Database::Models {
    template<typename T, typename = void, typename... Args>
    struct HasCreateImpl : std::false_type {};

    /**
     * @brief A helper struct to check if a class has CreateImpl method
     *
     * @tparam T The class to check
     * @tparam Args The arguments of the CreateImpl method
     */
    template<typename T, typename... Args>
    struct HasCreateImpl<
        T,
        std::void_t<decltype(std::declval<T>().CreateImpl(std::declval<std::decay_t<Args>>()...))>,
        Args...> : std::true_type {};

    template<typename T, typename... Args>
    inline constexpr bool HAS_CREATE_IMPL_V = HasCreateImpl<T, void, Args...>::value;

    /**
     * @brief The BaseModel class to be inherited by all models
     *
     * @tparam T The derived class
     */
    template<typename T>
    class BaseModel {
    public:
        BaseModel() = default;
        virtual ~BaseModel() = default;

        /**
         * @brief Check if the derived class has CreateImpl method and call it
         *
         * @tparam Args The type arguments of the CreateImpl method
         * @param args The arguments of the CreateImpl method
         */
        template<typename... Args>
        void Create(Args &&...args) {
            static_assert(HAS_CREATE_IMPL_V<T, std::decay_t<Args>...>,
                          "Derived class must implement CreateImpl method");
            static_cast<T *>(this)->CreateImpl(std::forward<Args>(args)...);
        }

    protected:
        /**
         * @brief CreateImpl method to be implemented by the derived class
         *
         * @tparam Args The type arguments of the CreateImpl method
         * @param args The arguments of the CreateImpl method
         */
        template<typename... Args>
        void CreateImpl(Args &&...args) = delete;

        /**
         * @brief Get the unique instance of the Database class
         *
         * @return The unique instance of the Database class
         */
        Database &GetDatabase() {
            return Database::Database::GetInstance();
        }
    };
};  // namespace Database::Models

#endif