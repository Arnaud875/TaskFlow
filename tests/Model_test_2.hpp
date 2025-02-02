#ifndef DATABASE_MODELS_MODEL_
#define DATABASE_MODELS_MODEL_

#include "Static.hpp"
#include <type_traits>

namespace Database::Models {
    template<typename Class, typename... Attributes>
    class BaseModel {
    public:
        BaseModel() = default;
        virtual ~BaseModel() = default;

        template <typename... Args>
        void Create(Args&&... args) {
            static_assert(std::is_base_of<BaseModel, Class>::value, "Class must be derived from BaseModel");
            static_assert(sizeof...(Attributes) == sizeof...(Args), "Number of arguments must match number of attributes");
        }

    protected:
        template <std::size_t... Indices, typename... Args>
        void AssignAttributesImpl(std::index_sequence<Indices...>, Args&&... args) {
            ((std::get<Indices>(attributes_) = std::forward<Args>(args)), ...);
        }

        template <typename... Args>
        void AssignAttributes(Args&&... args) {
            AssignAttributesImpl(std::make_index_sequence<sizeof...(Attributes)>{}, std::forward<Args>(args)...);
        }

        std::tuple<Attributes...> attributes_;
    };
};  // namespace Database::Models

#endif