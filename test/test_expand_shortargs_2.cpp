#include <catch.hpp>

#include <argparser.hpp>

TEST_CASE("Test expand shortargs", "[expand shortargs]") {
    constexpr int argc = 5;
    const char* argv[argc] = {
        "test.exe",
        "-ab",
        "-f", "First",
        "-def"
    };

    ap::parser p(argc, const_cast<char**>(argv));
    
    REQUIRE(p.argv().size() == argc + 3); 
}
