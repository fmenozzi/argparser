#include <catch.hpp>

#include <argparser.hpp>

TEST_CASE("Test arg assignments", "[assignments]") {
    constexpr int argc = 5;
    const char* argv[argc] = {
        "test.exe",
        "-f", "First",
        "-l", "Last",
    };

    ap::parser p(argc, const_cast<char**>(argv));
    REQUIRE(p.add("-f", "--first", "My first name"));
    REQUIRE(p.add("-l", "--last",  "My last name"));
    auto args = p.parse();

    REQUIRE(args.success());

    REQUIRE(args["-f"] == args["--first"]);
    REQUIRE(args["-f"] == "First");

    REQUIRE(args["-l"] == args["--last"]);
    REQUIRE(args["-l"] == "Last");
}
