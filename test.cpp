#include <iostream>
#include <type_traits>

namespace ap {
    enum class mode {
        REQUIRED,
        OPTIONAL
    };

    class parser {
    public:
        parser(int argc, char* argv[]) {

        }

        template<typename T>
        void add(const std::string& shortarg,
                 const std::string& longarg,
                 T* ptr,
                 const std::string& helpstr,
                 mode m = mode::REQUIRED) {

            static_assert(std::is_default_constructible<T>::value, "Cannot default construct T");
            *ptr = T{};
            std::cout << "Parsemode " << (m == mode::REQUIRED ?  "REQUIRED" : "OPTIONAL") << "\n";

        }
    };
}


int main(int argc, char* argv[]) {
    bool v;
    int i;

    ap::parser parser(argc, argv);
    parser.add("-v", "--verbose", &v, "Verbose output");
    parser.add("-i", "--integer", &i, "Integer argument", ap::mode::OPTIONAL);

    std::cout << v << ", " << i << "\n";

    return 0;
}
