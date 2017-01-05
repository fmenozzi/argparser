#include <catch.hpp>

#include <argparser.hpp>

TEST_CASE("Test unused optional args 1", "[unused]") {
    constexpr int argc = 1;
    const char* argv[argc] = {
        "test.exe",
    };

    ap::parser p(argc, const_cast<char**>(argv));
    REQUIRE(p.add("-f", "--first", "My first name"));
    REQUIRE(p.add("-l", "--last",  "My last name"));
    auto args = p.parse();

    REQUIRE(args.parsed_successfully());

    REQUIRE(args["-f"] == args["--first"]);
    REQUIRE(args["-f"] == "");

    REQUIRE(args["-l"] == args["--last"]);
    REQUIRE(args["-l"] == "");
}

TEST_CASE("Test unused optional args 2", "[unused]") {
    constexpr int argc = 3;
    const char* argv[argc] = {
        "test.exe",
        "-l", "Last",
    };

    ap::parser p(argc, const_cast<char**>(argv));
    REQUIRE(p.add("-f", "--first", "My first name"));
    REQUIRE(p.add("-l", "--last",  "My last name"));
    auto args = p.parse();

    REQUIRE(args.parsed_successfully());

    REQUIRE(args["-f"] == args["--first"]);
    REQUIRE(args["-f"] == "");

    REQUIRE(args["-l"] == args["--last"]);
    REQUIRE(args["-l"] == "Last");
}
