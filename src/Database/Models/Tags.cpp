#include "Tags.hpp"
#include "Database/TaskManager.hpp"
#include "Utils/Meta/SafeInvoke.hpp"
#include <regex>

bool Database::Models::TagsModel::Delete() {
    if (tagId_ == 0) {
        throw std::runtime_error("The model is not initialized.");
    };

    if (!isPersisted_) {
        return false;
    }

    bool result =
        static_cast<bool>(Utils::Meta::SafeInvoke(&Database::DeleteRow,
                                                  GetDatabase(),
                                                  SQLParams{"Tags", {{"id", std::to_string(tagId_)}}}));

    if (!result) {
        return false;
    }

    isPersisted_ = false;

    // Delete tag references from TaskTags
    result = static_cast<bool>(
        Utils::Meta::SafeInvoke(&Database::DeleteRow,
                                GetDatabase(),
                                SQLParams{"TaskTags", {{"tag_id", std::to_string(tagId_)}}}));
    return result;
}

bool Database::Models::TagsModel::Save() {
    if (!isPersisted_) {
        const SQLParams params = {
            "Tags", {{"user_id", std::to_string(userId_)}, {"name", name_}, {"color", color_}}};
        const auto result = Utils::Meta::SafeInvoke(&Database::InsertValues, GetDatabase(), params);
        if (!result) {
            return false;
        }

        isPersisted_ = true;
    } else {
        const std::optional<TagsModel> currentTagOpt = TaskManager::FindTagById(tagId_);
        if (!currentTagOpt.has_value()) {
            return false;
        }

        TagsModel currentTag = currentTagOpt.value();
        SQLParams params = {"Tags", {{"user_id", std::to_string(userId_)}}};

        if (currentTag.GetName() != name_) {
            params.attributes.emplace_back(SQLAttribute{"name", name_});
        }

        if (currentTag.GetColor() != color_) {
            params.attributes.emplace_back(SQLAttribute{"color", color_});
        }

        if (params.attributes.empty()) {
            return true;
        }

        const auto result = Utils::Meta::SafeInvoke(&Database::UpdateValues,
                                                    GetDatabase(),
                                                    params,
                                                    std::make_pair("id", std::to_string(tagId_)));

        if (!result) {
            return false;
        }
    }

    return true;
}

void Database::Models::TagsModel::SetName(const std::string &name) {
    if (name.empty()) {
        throw std::invalid_argument("Name must not be empty.");
    }

    if (name.size() > 20) {
        throw std::invalid_argument("Name must not exceed 20 characters.");
    }

    name_ = name;
}
void Database::Models::TagsModel::SetColor(const std::string &color) {
    if (color.empty()) {
        throw std::invalid_argument("Color must not be empty.");
    }

    if (color.size() > 20) {
        throw std::invalid_argument("Color must not exceed 20 characters.");
    }

    // Check if the color is a valid hex color
    std::regex hexColor("^#([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})$");
    if (!std::regex_match(color, hexColor)) {
        throw std::invalid_argument("Color must be a valid hex color.");
    }

    color_ = color;
}