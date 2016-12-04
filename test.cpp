#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include "argparser.hpp"

// Conversion specializations
template<typename T> struct convert_from_string_to {};
template<> struct convert_from_string_to<int> {
    static int convert(const std::string& s) { return std::stoi(s); }
};
template<> struct convert_from_string_to<bool> {
    static bool convert(const std::string& s) { return s == "1" || s == "true"; }
};

// Store functions to call for each arg string
//std::map<std::string, std::function<void(void)>> m;
std::vector<std::function<void(void)>> m;

// Assign functions to call for each arg
template<typename T>
void add(const std::string& argstr, T* ptr, const std::string& s) {
    /*
    m[argstr] = [ptr, s] () {
        *ptr = convert_from_string_to<T>::convert(s);
    };
    */
    m.emplace_back([ptr, s]() {
        *ptr = convert_from_string_to<T>::convert(s);
    });
}

// Call each function
void parse() {
    /*
    for (const auto& kv : m) {
        kv.second();
    }
    */
    for (const auto& f : m) {
        f();
    }
}

int main(int argc, char* argv[]) {
    int i = 1;
    bool b = false;

    add("-i", &i, "14");
    add("-b", &b, "true");

    std::cout << "i: " << i << "\n"
              << "b: " << b << "\n";

    parse();

    std::cout << "i: " << i << "\n"
              << "b: " << b << "\n";

    ap::parser(argc, argv);

    return 0;
}
