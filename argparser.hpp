#pragma once

#include <iostream>

#include <vector>
#include <string>
#include <map>

namespace ap {
    // Default conversion template
    template<typename T> struct convert_from_string_to {};

    // Integral conversions
    template<> struct convert_from_string_to<int> {
        static int convert(const std::string& s) { return std::stoi(s); }
    };
    template<> struct convert_from_string_to<long> {
        static long convert(const std::string& s) { return std::stol(s); }
    };
    template<> struct convert_from_string_to<unsigned long> {
        static unsigned int convert(const std::string& s) { return std::stoul(s); }
    };
    template<> struct convert_from_string_to<long long> {
        static long long convert(const std::string& s) { return std::stoll(s); }
    };
    template<> struct convert_from_string_to<unsigned long long> {
        static unsigned long long convert(const std::string& s) { return std::stoull(s); }
    };

    // Floating point conversions
    template<> struct convert_from_string_to<float> {
        static float convert(const std::string& s) { return std::stof(s); }
    };
    template<> struct convert_from_string_to<double> {
        static float convert(const std::string& s) { return std::stod(s); }
    };
    template<> struct convert_from_string_to<long double> {
        static float convert(const std::string& s) { return std::stold(s); }
    };

    // String conversion
    template<> struct convert_from_string_to<std::string> {
        static std::string convert(const std::string& s) { return s; }
    };

    // Boolean conversion
    template<> struct convert_from_string_to<bool> {
        static bool convert(const std::string& s) { return s == "1" || s == "true"; }
    };

    class parser {
    public:
        enum class mode {
            REQUIRED,
            OPTIONAL
        };

    private:
        struct argstruct {
            std::string shortarg;
            std::string longarg;
            std::string helpstr;
            mode        required;
            bool        parsed;

            argstruct(const std::string& sa,
                      const std::string& la,
                      const std::string& hs,
                      mode rq,
                      bool ps)
                : shortarg(sa)
                , longarg(la)
                , helpstr(hs)
                , required(rq)
                , parsed(ps) {}
        };

        int                                 m_argc;
        std::vector<std::string>            m_argv;
        std::vector<argstruct>              m_args;
        // TODO: Is there a reason why we have a map instead of
        //       just a vector of std::function?
        std::map<argstruct,
                 std::function<void(void)>> m_populate_args;

        void remove_equals(std::vector<std::string>& argv) {
            // TODO
        }

        void expand_shortargs(std::vector<std::string>& argv) {
            // TODO
        }

    public:
        parser(int argc, char* argv[]) {
            m_argv = std::vector<std::string>(argv, argv+argc);

            // Reformat argv in case --arg=val notation is used
            remove_equals(m_argv);
            
            // Expand shortargs in case -ab notation is used
            expand_shortargs(m_argv);
        }

        template<typename T>
        bool add(const std::string& shortarg,
                 const std::string& longarg,
                 T* ptr,
                 const std::string& helpstr,
                 mode m = mode::REQUIRED) {

            argstruct as(shortarg, longarg, helpstr, m, false);

            return false;
        }

        bool parse() {
            return false;
        }
    };
}
