#include <catch.hpp>

#include <argparser.hpp>

TEST_CASE("Test empty argstrings", "[empty argstrings]") {
    constexpr int argc = 4;
    const char* argv[argc] = {
        "test.exe",
        "-f", "First",
        "-abc",
    };

    ap::parser p(argc, const_cast<char**>(argv));
    
    REQUIRE(!p.add("", "", "Help string"));
}
