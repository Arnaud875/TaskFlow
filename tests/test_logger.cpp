#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include "Utils/Logger/Logger.hpp"
#include "Utils/Logger/LoggerFormat.hpp"
#include <filesystem>

// if patate == exist :
// peel potato 
// THENboil potato 
// <3 

/**
 * Example format string:
 *  - ("Hello {} !", "World") = "Hello World !"
 *  - ("I have {} years old.", 19) = "I have 19 years old."
 *  - ("Classe object: {}", instance_object) = Error in compilation, unsupported type
 */

using namespace Utils::Logger;

inline auto matches(const std::string& str) {
    return Catch::Matchers::Matches(str);
}

TEST_CASE("format_empty_string", "[logger]") {
    REQUIRE_THAT("Hello, world !", matches(FormatStringLogs("Hello, world !")));
}

TEST_CASE("format_string_basic", "[logger]") {
    REQUIRE_THAT("Hello, world !", matches(FormatStringLogs("Hello, {} !", "world")));
    REQUIRE_THAT("Test", matches(FormatStringLogs("{}", "Test")));
    REQUIRE_THAT("Begin Middle End", matches(FormatStringLogs("{} {} {}", "Begin", "Middle", "End")));
}

TEST_CASE("format_string_numbers", "[logger]") {
    REQUIRE_THAT("Number: 42", matches(FormatStringLogs("Number: {}", 42)));
    REQUIRE_THAT("Float: 3.14", matches(FormatStringLogs("Float: {}", 3.14)));
    REQUIRE_THAT("Age: -5", matches(FormatStringLogs("Age: {}", -5)));
}

TEST_CASE("format_string_mixed_types", "[logger]") {
    REQUIRE_THAT("Name: John, Age: 30", matches(FormatStringLogs("Name: {}, Age: {}", "John", 30)));
    REQUIRE_THAT("Pi is approximately 3.14", matches(FormatStringLogs("Pi is approximately {}", 3.14)));
}

TEST_CASE("format_string_edge_cases", "[logger]") {
    REQUIRE_THAT("", matches(FormatStringLogs("")));
}

TEST_CASE("format_string_multiple_replacements", "[logger]") {
    REQUIRE_THAT("1 2 3", matches(FormatStringLogs("{} {} {}", 1, 2, 3)));
    REQUIRE_THAT("a b c", matches(FormatStringLogs("{} {} {}", "a", "b", "c")));
}

TEST_CASE("format_string_error_cases", "[logger]") {
    REQUIRE_THROWS(FormatStringLogs("{} {}", "too", "many", "arguments"));
}

TEST_CASE("save_logs_to_file", "[logger]") {
    Logger::CreateInstance();

    if (std::filesystem::exists("./logs.txt")) {
        std::filesystem::remove("./logs.txt");
    }

    for (std::size_t i = 0; i < 150; i++) {
        LOG_INFO("Test message {}", i);
    }
}