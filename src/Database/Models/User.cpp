#include "User.hpp"
#include <bcrypt.h>

bool Database::Models::UserModel::SetUsername(const std::string &username) {
    const std::size_t usernameSize = username.size();
    if (username.empty() || (usernameSize < 3 || usernameSize > 20)) {
        SetLastError("Username must be between 3 and 20 characters.");
        return false;
    }

    if (username == username_) {
        SetLastError("Username is the same as the current one.");
        return false;
    }

    username_ = username;
    return true;
}

bool Database::Models::UserModel::SetEmail(const std::string &email) {
    if (!checkEmailIsValid(email)) {
        SetLastError("Invalid email address.");
        return false;
    }

    if (email == email_) {
        SetLastError("Email is the same as the current one.");
        return false;
    }

    email_ = email;
    return true;
}

bool Database::Models::UserModel::SetPassword(const std::string &password) {
    if (password.empty()) {
        SetLastError("Password must not be empty.");
        return false;
    }

    const std::string hashedPassword = hashPassword(password);
    if (hashedPassword == hashed_password_) {
        SetLastError("Password is the same as the current one.");
        return false;
    }

    hashed_password_ = hashedPassword;
    return true;
}

std::string Database::Models::UserModel::hashPassword(const std::string &password) {
    return bcrypt::generateHash(password);
}

bool Database::Models::UserModel::CheckPassword(const std::string &password) const {
    return bcrypt::validatePassword(password, hashed_password_);
}