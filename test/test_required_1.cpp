#include <catch.hpp>

#include <argparser.hpp>

TEST_CASE("Test required args", "[required]") {
    constexpr int argc = 1;
    const char* argv[argc] = {
        "test.exe"
    };

    std::string name;

    ap::parser p(argc, const_cast<char**>(argv));
    p.add("-n", "--name", &name, "My name");
    bool success = p.parse();
    
    REQUIRE(!success);
}
