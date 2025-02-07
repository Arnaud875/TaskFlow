#ifndef DATABASE_MODELS_MODEL_
#define DATABASE_MODELS_MODEL_

#include "Database/Database.hpp"
#include "Static.hpp"

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

    // template<typename T, typename = void>
    // struct HasSaveImpl : std::false_type {};

    // template<typename T>
    // struct HasSaveImpl<T, std::void_t<decltype(std::declval<T>().SaveImpl())>> : std::true_type
    // {};

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

        // void Save() {
        //     static_assert(HasSaveImpl<T>::value, "Derived class must implement SaveImpl method");
        //     static_cast<T *>(this)->SaveImpl();
        // }

        [[nodiscard]] std::string GetLastError() const {
            return last_error_;
        };

    protected:
        template<typename... Args>
        void CreateImpl(Args &&...args) = delete;
        // void SaveImpl() = delete;

        void SetLastError(const std::string &error) {
            last_error_ = error;
        };

        Database &GetDatabase() {
            return Database::Database::GetInstance();
        }

    private:
        std::string last_error_;
    };
};  // namespace Database::Models

#endif