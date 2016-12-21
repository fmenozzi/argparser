#pragma once

#include <vector>

#include <string>
#include <algorithm>
#include <map>

namespace ap {
    // Designates whether given argument is optional or required
    enum class mode {
        REQUIRED,
        OPTIONAL
    };

    // Object returned from parse()
    class argmap {
    private:
        std::map<std::string, std::string> m_args;
        bool m_success;

    public:
        argmap(const std::map<std::string, std::string>& args, bool success)
            : m_args(args)
            , m_success(success) {}

        const std::string& operator[](const std::string& argstr) {
            return m_args[argstr];
        }

        bool success() const noexcept {
            return m_success;
        }
    };

    class parser {
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

        int                      m_argc;
        std::vector<std::string> m_argv;
        std::vector<argstruct>   m_args;

        bool m_any_adds_failed = false;

    private:
        void remove_equals(std::vector<std::string>& argv) const {
            int new_argc = std::count_if(argv.begin(), argv.end(), [](const std::string& s) {
                return s.find("=") != std::string::npos;
            }) + argv.size();

            argv.reserve(new_argc);

            auto it = argv.begin();
            while (it != argv.end()) {
                auto idx = it->find("=");
                if (idx != std::string::npos) {
                    auto arg = it->substr(0, idx);
                    auto val = it->substr(idx+1);

                    it = argv.erase(it);
                    it = argv.insert(it, val);
                    it = argv.insert(it, arg);
                }
                if (it != argv.end()) {
                    ++it;
                }
            }
        }

        bool is_multi_shortarg(const std::string& s) const noexcept {
            return s[0] == '-' && s[1] != '-' && s.size() > 2;
        }

        void expand_shortargs(std::vector<std::string>& argv) const {
            int new_argc = argv.size();
            for (const auto& arg : argv) {
                if (this->is_multi_shortarg(arg)) {
                    new_argc += arg.size() - 2;
                }
            }

            argv.reserve(new_argc);

            auto it = argv.begin();
            while (it != argv.end()) {
                auto arg = *it;
                if (this->is_multi_shortarg(arg)) {
                    it = argv.erase(it);
                    for (size_t i = arg.size()-1; i > 0; i--) {
                        it = argv.insert(it, "-" + std::string(1, arg[i]));
                    }
                } else {
                    ++it;
                }
            }
        }

    public:
        parser(int argc, char* argv[]) {
            m_argv = std::vector<std::string>(argv, argv+argc);

            // Reformat argv in case --arg=val notation is used
            this->remove_equals(m_argv);
            
            // Expand shortargs in case -ab notation is used
            this->expand_shortargs(m_argv);

            m_argc = m_argv.size();
        }

        bool add(const std::string& shortarg,
                 const std::string& longarg,
                 const std::string& helpstr,
                 mode m = mode::REQUIRED) {

            // Can't have both argstrings be empty
            if (shortarg.empty() && longarg.empty()) {
                m_any_adds_failed = true;
                return false;
            }

            // Argstrings must be formatted properly
            if (shortarg.size() != 2 || shortarg[0] != '-' || shortarg[1] == '-') {
                m_any_adds_failed = true;
                return false;
            }
            if (longarg.size() <= 2 || longarg[0] != '-' || longarg[1] != '-') {
                m_any_adds_failed = true;
                return false;
            }

            // -h, --help are reserved
            if (shortarg == "-h" || longarg == "--help") {
                m_any_adds_failed = true;
                return false;
            }

            // No empty help string
            if (helpstr.empty()) {
                m_any_adds_failed = true;
                return false;
            }

            argstruct as(shortarg, longarg, helpstr, m, false);

            return false;
        }

        argmap parse() {
            std::map<std::string, std::string> map;
            bool success = true;

            if (m_any_adds_failed) {
                success = false;
            }

            return argmap(map, success);
        }

        int argc() const noexcept {
            return m_argc;
        }

        const std::vector<std::string>& argv() const noexcept {
            return m_argv;
        }
    };
}
