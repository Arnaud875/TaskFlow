#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include "Database/Models/User.hpp"

static Database::Models::UserModel user;

TEST_CASE("Create user model and test setter function", "[models::users]") {
    user.Create(1, 0, "test", "test@gmail.com", "thisismypassword");

    REQUIRE(user.GetId() == 1);
    REQUIRE(user.GetCreatedAt() == 0);
    REQUIRE(user.GetUsername() == "test");
    REQUIRE(user.GetEmail() == "test@gmail.com");

    REQUIRE(user.SetUsername("test2") == true);
    REQUIRE(user.GetUsername() == "test2");

    REQUIRE(user.SetUsername("a") == false); // Invalid username
    REQUIRE(user.SetEmail("test") == false); // Invalid email

    REQUIRE(user.SetEmail("hello@gmail.com") == true);
    REQUIRE(user.GetEmail() == "hello@gmail.com");
}

TEST_CASE("Check user password hashing", "[models::users]") {
    REQUIRE(user.CheckPassword("thisismypassword") == true);

    user.SetPassword("thisismypassword2");
    REQUIRE(user.CheckPassword("thisismypassword2") == true);

    REQUIRE(user.CheckPassword("thisismypassword") == false); // Invalid password
}