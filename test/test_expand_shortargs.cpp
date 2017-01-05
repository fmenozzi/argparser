#include <catch.hpp>

#include <argparser.hpp>

TEST_CASE("Test expand shortargs 1", "[expand shortargs]") {
    constexpr int argc = 4;
    const char* argv[argc] = {
        "test.exe",
        "-f", "First",
        "-abc",
    };

    ap::parser p(argc, const_cast<char**>(argv));
    
    REQUIRE(p.argc() == argc + 2);
}

TEST_CASE("Test expand shortargs 2", "[expand shortargs]") {
    constexpr int argc = 5;
    const char* argv[argc] = {
        "test.exe",
        "-ab",
        "-f", "First",
        "-def"
    };

    ap::parser p(argc, const_cast<char**>(argv));
    
    REQUIRE(p.argc() == argc + 3);
}
