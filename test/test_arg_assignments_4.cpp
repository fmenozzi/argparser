#include <catch.hpp>

#include <argparser.hpp>

TEST_CASE("Test arg assignments", "[assignments]") {
    constexpr int argc = 8;
    const char* argv[argc] = {
        "test.exe",
        "-f", "First",
        "-l", "Last",
        "--boolean",
        "-s", "Hello, World!"
    };

    ap::parser p(argc, const_cast<char**>(argv));
    REQUIRE(p.add("-f", "--first", "My first name"));
    REQUIRE(p.add("-l", "--last",  "My last name"));
    REQUIRE(p.add("", "--boolean", "My boolean var", true));
    REQUIRE(p.add("-s", "", "My string var"));
    auto args = p.parse();

    REQUIRE(args.success());

    REQUIRE(args["-f"] == args["--first"]);
    REQUIRE(args["-f"] == "First");

    REQUIRE(args["-l"] == args["--last"]);
    REQUIRE(args["-l"] == "Last");

    REQUIRE(args["--boolean"] == "1");

    REQUIRE(args["-s"] == "Hello, World!");
}
