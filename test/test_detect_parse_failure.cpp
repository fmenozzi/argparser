#include <catch.hpp>

#include <argparser.hpp>

TEST_CASE("Test detect parse failure", "[detect parse failure]") {
    constexpr int argc = 2;
    const char* argv[argc] = {
        "test.exe",
        "-f=",
    };

    ap::parser p(argc, const_cast<char**>(argv));
    REQUIRE(p.add("-f", "--first", "First name"));
    auto args = p.parse();

    REQUIRE(!args.parsed_successfully());
}
