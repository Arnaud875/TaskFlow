#ifndef DATABASE_MODELS_USER_
#define DATABASE_MODELS_USER_

#include "Database/Models/Model.hpp"
#include <regex>

namespace Database::Models {
    /**
     * @brief A class that represents the users table in the database.
     */
    class UserModel : public BaseModel<UserModel> {
    public:
        /**
         * @brief A struct that contains the attributes of a user.
         */
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

        UserModel() = default;

        /**
         * @brief Construct a new User Model object
         *
         * @param param Required attributes to create a new user.
         */
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

        /**
         * @brief Check if the password is correct.
         *
         * @param password The password to check.
         * @return Return true if the password is correct.
         */
        [[nodiscard]] bool CheckPassword(const std::string &password) const;

        /**
         * @brief Insert the user into the database if it's not persisted yet or update it.
         */
        bool Save();

        /**
         * @brief Delete the user from the database if it's persisted.
         *
         * @return Return true if the user is deleted successfully.
         */
        bool Delete();

    private:
        /**
         * @brief Hash the password using bcrypt.
         *
         * @param password The password to hash.
         * @return Return the hashed password.
         */
        [[nodiscard]] static std::string hashPassword(const std::string &password);

        /**
         * @brief Check if the email is valid.
         *
         * @param email The email to check.
         * @return Return true if the email is valid.
         */
        [[nodiscard]]
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