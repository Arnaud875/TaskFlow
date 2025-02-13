#ifndef DATABASE_MODELS_TAGS_
#define DATABASE_MODELS_TAGS_

#include "Model.hpp"

namespace Database::Models {
    class TagsModel : public BaseModel<TagsModel> {
    public:
        struct TagAttributes {
            int tagId;
            int userId;
            std::string name;
            std::string color;
            int isPersisted = false;
        };

        void CreateImpl(const TagAttributes &param) {
            tagId_ = param.tagId;
            userId_ = param.userId;
            isPersisted_ = param.isPersisted;

            SetName(param.name);
            SetColor(param.color);
        }

        bool Save();
        bool Delete();

        // Getter
        int GetTagId() const {
            return tagId_;
        }

        int GetUserId() const {
            return userId_;
        }

        std::string GetName() const {
            return name_;
        }

        std::string GetColor() const {
            return color_;
        }

        // Setter
        void SetName(const std::string &name);
        void SetColor(const std::string &color);

    private:
        bool isPersisted_ = false;
        int tagId_;
        int userId_;
        std::string name_;
        std::string color_;
    };
}  // namespace Database::Models

#endif