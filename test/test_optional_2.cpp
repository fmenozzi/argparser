#include <catch.hpp>

#include <argparser.hpp>

TEST_CASE("Test optional args", "[optional]") {
    constexpr int argc = 1;
    const char* argv[argc] = {
        "test.exe",
    };

    ap::parser p(argc, const_cast<char**>(argv));
    REQUIRE(p.add("-f", "--first", "My first name"));
    REQUIRE(p.add("-l", "--last",  "My last name"));

    REQUIRE(p.parse().success());
}
