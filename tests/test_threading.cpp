#include <catch2/catch_test_macros.hpp>

TEST_CASE("Basic assertions", "[catch2]") {
    // Assertions de base
    REQUIRE(1 + 1 == 2);
    CHECK(2 * 2 == 4);  // CHECK continue même en cas d'échec

    // Test avec sections
    SECTION("Vector operations") {
        std::vector<int> v{1, 2, 3};
        REQUIRE(v.size() == 3);
        REQUIRE_FALSE(v.empty());
    }
}

TEST_CASE("Exception testing", "[catch2]") {
    REQUIRE_THROWS([] { throw std::runtime_error("error"); }());
    REQUIRE_NOTHROW([] { return true; }());
}

// Tag unique pour un groupe de tests basiques
TEST_CASE("Basic math operations", "[math]") {
    REQUIRE(1 + 1 == 2);
}

// Tags multiples pour catégoriser un test sous plusieurs aspects
TEST_CASE("Vector performance test", "[vector][performance][slow]") {
    std::vector<int> large_vector(1000000);
    REQUIRE(large_vector.capacity() >= 1000000);
}

// Tags pour les tests qui ne sont pas prêts
TEST_CASE("Feature in development", "[math][!mayfail]") {
    REQUIRE(true);
}

// Tags pour regrouper des tests par module
TEST_CASE("Database connection", "[db][network]") {
    REQUIRE(true);
}