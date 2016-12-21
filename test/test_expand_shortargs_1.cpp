#include <catch.hpp>

#include <argparser.hpp>

TEST_CASE("Test expand shortargs", "[expand shortargs]") {
    constexpr int argc = 4;
    const char* argv[argc] = {
        "test.exe",
        "-f", "First",
        "-abc",
    };

    ap::parser p(argc, const_cast<char**>(argv));
    
    REQUIRE(p.argc() == argc + 2);
}
