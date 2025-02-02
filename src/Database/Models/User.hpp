#ifndef DATABASE_MODELS_USER_
#define DATABASE_MODELS_USER_

#include "Database/Models/Model.hpp"
#include <regex>

namespace Database::Models {
    class UserModel : public BaseModel<UserModel> {
    public:
        void CreateImpl(int id,
                        int createdAt,
                        const std::string &username,
                        const std::string &email,
                        const std::string &password) {
            if (username.empty() || email.empty() || password.empty()) {
                throw std::invalid_argument("Username, email, and password must not be empty.");
            }

            id_ = id;
            created_at_ = createdAt;

            if (!SetUsername(username) || !SetEmail(email) || !SetPassword(password)) {
                throw std::invalid_argument(GetLastError());
            }
        }

        // Getter
        [[nodiscard]] int GetId() const {
            return id_;
        }
        [[nodiscard]] int GetCreatedAt() const {
            return created_at_;
        }

        [[nodiscard]] std::string GetUsername() const {
            return username_;
        }
        [[nodiscard]] std::string GetEmail() const {
            return email_;
        }

        // Setter
        void SetId(int id) {
            id_ = id;
        }

        void SetCreatedAt(const int createdAt) {
            created_at_ = createdAt;
        }

        bool SetPassword(const std::string &password);
        bool SetUsername(const std::string &username);
        bool SetEmail(const std::string &email);

        [[nodiscard]] bool CheckPassword(const std::string &password) const;

    private:
        static std::string hashPassword(const std::string &password);

        static bool checkEmailIsValid(const std::string &email) {
            const std::regex emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
            return std::regex_match(email, emailRegex);
        }

        int id_;
        int created_at_;
        std::string username_;
        std::string email_;
        std::string hashed_password_;
    };
};  // namespace Database::Models

#endif