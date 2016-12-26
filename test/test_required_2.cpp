#include <catch.hpp>

#include <argparser.hpp>

TEST_CASE("Test required args", "[required]") {
    constexpr int argc = 3;
    const char* argv[argc] = {
        "test.exe",
        "-f", "First",
    };

    ap::parser p(argc, const_cast<char**>(argv));
    REQUIRE(p.add("-f", "--first", "My first name", ap::mode::REQUIRED));
    REQUIRE(p.add("-l", "--last",  "My last name",  ap::mode::REQUIRED));
    
    REQUIRE(!p.parse().success());
}
