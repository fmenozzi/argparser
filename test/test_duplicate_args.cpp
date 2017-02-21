#include <catch.hpp>

#include <argparser.hpp>

TEST_CASE("Test duplicate args 1", "[duplicates]") {
    constexpr int argc = 3;
    const char* argv[argc] = {
        "test.exe",
        "-f", "First",
    };

    ap::parser p(argc, const_cast<char**>(argv));
    REQUIRE( p.add("-f", "--first", "My first name", ap::mode::REQUIRED));
    REQUIRE(!p.add("-f", "--bogus", "Duplicate shortarg!", ap::mode::REQUIRED));
    auto args = p.parse();

    REQUIRE(!args.parsed_successfully());
}

TEST_CASE("Test duplicates args 2", "[duplicates]") {
    constexpr int argc = 3;
    const char* argv[argc] = {
        "test.exe",
        "-f", "First",
    };

    ap::parser p(argc, const_cast<char**>(argv));
    REQUIRE( p.add("-f", "--first", "My first name", ap::mode::REQUIRED));
    REQUIRE(!p.add("-b", "--first", "Duplicate longarg!", ap::mode::REQUIRED));
    auto args = p.parse();

    REQUIRE(!args.parsed_successfully());
}
