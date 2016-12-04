#include <catch.hpp>

#include <array>

TEST_CASE("Test args", "[args]") {
    constexpr int argc = 3;
    std::array<const char*, argc> argv = {
        "name.exe",
        "--b",
        "lakjd",
    };
    
    REQUIRE(argv.size() == argc);
}
