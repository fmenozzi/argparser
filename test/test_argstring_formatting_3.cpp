#include <catch.hpp>

#include <argparser.hpp>

TEST_CASE("Test argstring formatting", "[argstring formatting]") {
    constexpr int argc = 4;
    const char* argv[argc] = {
        "test.exe",
        "-f", "First",
        "-abc",
    };

    int i;

    ap::parser p(argc, const_cast<char**>(argv));
    
    REQUIRE(!p.add("b", "--good", &i, "Help string")); 
}
