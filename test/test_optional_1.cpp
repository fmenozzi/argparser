#include <catch.hpp>

#include <argparser.hpp>

TEST_CASE("Test optional args", "[optional]") {
    constexpr int argc = 3;
    const char* argv[argc] = {
        "test.exe",
        "-f", "First",
    };

    std::string first, last;

    ap::parser p(argc, const_cast<char**>(argv));
    p.add("-f", "--first", &first, "My first name");
    p.add("-l", "--last",  &last,  "My last name", ap::mode::OPTIONAL);
    bool success = p.parse();

    REQUIRE(success);
}
