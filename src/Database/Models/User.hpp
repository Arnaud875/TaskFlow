#ifndef DATABASE_MODELS_USER_
#define DATABASE_MODELS_USER_

#include "Database/Models/Model.hpp"
#include <regex>

namespace Database::Models {
    class UserModel : public BaseModel<UserModel> {
    public:
        UserModel() = default;

        struct UserAttributes {
            int userId;
            std::string username;
            std::string email;
            std::string password;
            int createdAt = 0;
            int updatedAt = 0;
            bool isPersisted = false;
            bool isPasswordHashed = false;
        };

        void CreateImpl(const UserAttributes &param) {
            if (param.username.empty() || param.email.empty() || param.password.empty()) {
                throw std::invalid_argument("Username, email, and password must not be empty.");
            }

            userId_ = param.userId;
            createdAt_ = param.createdAt;
            updatedAt_ = param.updatedAt;
            isPersisted_ = param.isPersisted;

            SetUsername(param.username);
            SetEmail(param.email);

            // Check if password is hashed
            if (!param.isPasswordHashed) {
                SetPassword(param.password);
            } else {
                hashedPassword_ = param.password;
            }
        }

        // Getter
        [[nodiscard]] int GetUserId() const {
            return userId_;
        }
        [[nodiscard]] int GetCreatedAt() const {
            return createdAt_;
        }

        [[nodiscard]] std::string GetUsername() const {
            return username_;
        }
        [[nodiscard]] std::string GetEmail() const {
            return email_;
        }

        // Setter
        void SetUserId(int userId) {
            userId_ = userId;
        }

        void SetCreatedAt(const int createdAt) {
            createdAt_ = createdAt;
        }

        void SetPassword(const std::string &password);
        void SetUsername(const std::string &username);
        void SetEmail(const std::string &email);

        [[nodiscard]] bool CheckPassword(const std::string &password) const;

        bool Save();
        bool Delete();

        static std::optional<UserModel> FindByUserId(int userId);

    private:
        [[nodiscard]] static std::string hashPassword(const std::string &password);

        static bool checkEmailIsValid(const std::string &email) {
            const std::regex emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
            return std::regex_match(email, emailRegex);
        }

        bool isPersisted_;
        int userId_;
        int createdAt_;
        int updatedAt_;
        std::string username_;
        std::string email_;
        std::string hashedPassword_;
    };
};  // namespace Database::Models

#endif