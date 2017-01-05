#include <catch.hpp>

#include <argparser.hpp>

TEST_CASE("Test arg assignments 1", "[assignments]") {
    constexpr int argc = 5;
    const char* argv[argc] = {
        "test.exe",
        "-f", "First",
        "-l", "Last",
    };

    ap::parser p(argc, const_cast<char**>(argv));
    REQUIRE(p.add("-f", "--first", "My first name", ap::mode::REQUIRED));
    REQUIRE(p.add("-l", "--last",  "My last name",  ap::mode::REQUIRED));
    auto args = p.parse();

    REQUIRE(args.parsed_successfully());

    REQUIRE(args["-f"] == args["--first"]);
    REQUIRE(args["-f"] == "First");

    REQUIRE(args["-l"] == args["--last"]);
    REQUIRE(args["-l"] == "Last");
}

TEST_CASE("Test arg assignments 2", "[assignments]") {
    constexpr int argc = 2;
    const char* argv[argc] = {
        "test.exe",
        "-b",
    };

    ap::parser p(argc, const_cast<char**>(argv));
    REQUIRE(p.add("-b", "--boolean", "My boolean value", ap::mode::BOOLEAN));
    auto args = p.parse();

    REQUIRE(args.parsed_successfully());

    REQUIRE(args["-b"] == args["--boolean"]);
    REQUIRE(args["-b"] == "1");
}

TEST_CASE("Test arg assignments 3", "[assignments]") {
    constexpr int argc = 6;
    const char* argv[argc] = {
        "test.exe",
        "-f", "First",
        "-l", "Last",
        "--boolean",
    };

    ap::parser p(argc, const_cast<char**>(argv));
    REQUIRE(p.add("-f", "--first", "My first name",  ap::mode::REQUIRED));
    REQUIRE(p.add("-l", "--last",  "My last name",   ap::mode::REQUIRED));
    REQUIRE(p.add("", "--boolean", "My boolean var", ap::mode::BOOLEAN));
    auto args = p.parse();

    REQUIRE(args.parsed_successfully());

    REQUIRE(args["-f"] == args["--first"]);
    REQUIRE(args["-f"] == "First");

    REQUIRE(args["-l"] == args["--last"]);
    REQUIRE(args["-l"] == "Last");

    REQUIRE(args["--boolean"] == "1");
}

TEST_CASE("Test arg assignments 4", "[assignments]") {
    constexpr int argc = 8;
    const char* argv[argc] = {
        "test.exe",
        "-f", "First",
        "-l", "Last",
        "--boolean",
        "-s", "Hello, World!"
    };

    ap::parser p(argc, const_cast<char**>(argv));
    REQUIRE(p.add("-f", "--first", "My first name",  ap::mode::REQUIRED));
    REQUIRE(p.add("-l", "--last",  "My last name",   ap::mode::REQUIRED));
    REQUIRE(p.add("", "--boolean", "My boolean var", ap::mode::BOOLEAN));
    REQUIRE(p.add("-s", "", "My string var",         ap::mode::REQUIRED));
    auto args = p.parse();

    REQUIRE(args.parsed_successfully());

    REQUIRE(args["-f"] == args["--first"]);
    REQUIRE(args["-f"] == "First");

    REQUIRE(args["-l"] == args["--last"]);
    REQUIRE(args["-l"] == "Last");

    REQUIRE(args["--boolean"] == "1");

    REQUIRE(args["-s"] == "Hello, World!");
}
