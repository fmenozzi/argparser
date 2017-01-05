#include <catch.hpp>

#include <argparser.hpp>

TEST_CASE("Test argstring formatting 1", "[argstring formatting]") {
    constexpr int argc = 4;
    const char* argv[argc] = {
        "test.exe",
        "-f", "First",
        "-abc",
    };

    ap::parser p(argc, const_cast<char**>(argv));
    REQUIRE(!p.add("-g", "-bad", "Help string")); 
}

TEST_CASE("Test argstring formatting 2", "[argstring formatting]") {
    constexpr int argc = 4;
    const char* argv[argc] = {
        "test.exe",
        "-f", "First",
        "-abc",
    };

    ap::parser p(argc, const_cast<char**>(argv));
    REQUIRE(!p.add("--b", "--good", "Help string")); 
}

TEST_CASE("Test argstring formatting 3", "[argstring formatting]") {
    constexpr int argc = 4;
    const char* argv[argc] = {
        "test.exe",
        "-f", "First",
        "-abc",
    };

    ap::parser p(argc, const_cast<char**>(argv));
    REQUIRE(!p.add("b", "--good", "Help string")); 
}

TEST_CASE("Test argstring formatting 4", "[argstring formatting]") {
    constexpr int argc = 4;
    const char* argv[argc] = {
        "test.exe",
        "-f", "First",
        "-abc",
    };

    ap::parser p(argc, const_cast<char**>(argv));
    REQUIRE(!p.add("-g", "bad", "Help string"));
}
