#include "User.hpp"
#include <bcrypt.h>
#include <sqlite3.h>

std::optional<Database::Models::UserModel> Database::Models::UserModel::FindByUserId(int userId) {
    const auto result = Utils::SafeInvoke(&Database::FindRowByAttributes,
                                          Database::GetInstance(),
                                          SQLParams{"users", {{"id", std::to_string(userId)}}});

    if (!result.has_value() || result.value().empty()) {
        return std::nullopt;
    }

    try {
        const auto &userResult = result.value();
        UserModel userModel;
        userModel.Create(UserAttributes{std::stoi(userResult.at("id")),
                                        userResult.at("username"),
                                        userResult.at("email"),
                                        userResult.at("password"),
                                        std::stoi(userResult.at("created_at")),
                                        std::stoi(userResult.at("updated_at")),
                                        true,
                                        true});
        return std::move(userModel);
    } catch (const std::exception &e) {
        return std::nullopt;
    }
}

bool Database::Models::UserModel::Save() {
    if (userId_ == 0)
        throw std::invalid_argument("The model is not initialized.");

    if (!isPersisted_) {
        const SQLParams params = {
            "Users", {{"username", username_}, {"email", email_}, {"password", hashedPassword_}}};
        const auto result = Utils::SafeInvoke(&Database::InsertValues, GetDatabase(), params);

        if (!result) {
            return false;
        }

        isPersisted_ = true;
    } else {
        const std::optional<UserModel> currentUserOpt = FindByUserId(userId_);
        UserModel currentUser;

        if (!currentUserOpt.has_value()) {
            return false;
        }

        currentUser = currentUserOpt.value();
        SQLParams params = {"Users", {}};

        if (currentUser.GetUsername() != username_) {
            params.attributes.emplace_back(SQLAttribute{"username", username_});
        }

        if (currentUser.GetEmail() != email_) {
            params.attributes.emplace_back(SQLAttribute{"email", email_});
        }

        if (!currentUser.CheckPassword(hashedPassword_)) {
            params.attributes.emplace_back(SQLAttribute{"password", hashedPassword_});
        }

        if (params.attributes.empty()) {
            return true;
        }

        const auto result = Utils::SafeInvoke(&Database::UpdateValues,
                                              GetDatabase(),
                                              params,
                                              std::make_pair("id", std::to_string(userId_)));
        if (!result) {
            return false;
        }
    }

    return true;
}

bool Database::Models::UserModel::Delete() {
    if (!isPersisted_) {
        return false;
    }

    const auto result = Utils::SafeInvoke(&Database::DeleteRow,
                                          GetDatabase(),
                                          SQLParams{"Users", {}},
                                          std::make_pair("id", std::to_string(userId_)));

    if (!result) {
        return false;
    }

    isPersisted_ = false;
    return true;
}

void Database::Models::UserModel::SetUsername(const std::string &username) {
    const std::size_t usernameSize = username.size();
    if (username.empty() || (usernameSize < 3 || usernameSize > 20)) {
        throw std::invalid_argument("Username must be between 3 and 20 characters.");
    }

    if (username == username_) {
        throw std::invalid_argument("Username is the same as the current one.");
    }

    username_ = username;
}

void Database::Models::UserModel::SetEmail(const std::string &email) {
    if (!checkEmailIsValid(email)) {
        throw std::invalid_argument("Email is not valid.");
    }

    if (email == email_) {
        throw std::invalid_argument("Email is the same as the current one.");
    }

    email_ = email;
}

void Database::Models::UserModel::SetPassword(const std::string &password) {
    if (password.empty()) {
        throw std::invalid_argument("Password must not be empty.");
    }

    const std::string hashedPassword = hashPassword(password);
    if (hashedPassword == hashedPassword_) {
        throw std::invalid_argument("Password is the same as the current one.");
    }

    hashedPassword_ = hashedPassword;
}

std::string Database::Models::UserModel::hashPassword(const std::string &password) {
    return bcrypt::generateHash(password);
}

bool Database::Models::UserModel::CheckPassword(const std::string &password) const {
    return bcrypt::validatePassword(password, hashedPassword_);
}