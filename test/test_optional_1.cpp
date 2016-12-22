#include <catch.hpp>

#include <argparser.hpp>

TEST_CASE("Test optional args", "[optional]") {
    constexpr int argc = 3;
    const char* argv[argc] = {
        "test.exe",
        "-f", "First",
    };

    ap::parser p(argc, const_cast<char**>(argv));
    REQUIRE(p.add("-f", "--first", "My first name", false, false));
    REQUIRE(p.add("-l", "--last",  "My last name", false, false));

    REQUIRE(p.parse().success());
}
