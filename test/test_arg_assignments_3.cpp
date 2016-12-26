#include <catch.hpp>

#include <argparser.hpp>

TEST_CASE("Test arg assignments", "[assignments]") {
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

    REQUIRE(args.success());

    REQUIRE(args["-f"] == args["--first"]);
    REQUIRE(args["-f"] == "First");

    REQUIRE(args["-l"] == args["--last"]);
    REQUIRE(args["-l"] == "Last");

    REQUIRE(args["--boolean"] == "1");
}
