#pragma once

#include <vector>

#include <string>
#include <algorithm>
#include <map>

namespace ap {
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
            bool        booltype;
            bool        required;
            bool        parsed;

            argstruct(const std::string& sa,
                      const std::string& la,
                      const std::string& hs,
                      bool bt,
                      bool rq,
                      bool ps)
                : shortarg(sa)
                , longarg(la)
                , helpstr(hs)
                , booltype(bt)
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
                 bool booltype=false,
                 bool required=true) {

            // Can't have both argstrings be empty
            if (shortarg.empty() && longarg.empty()) {
                m_any_adds_failed = true;
                return false;
            }

            // Argstrings must be formatted properly
            if (!shortarg.empty() && (shortarg.size() != 2 || shortarg[0] != '-' || shortarg[1] == '-')) {
                m_any_adds_failed = true;
                return false;
            }
            if (!longarg.empty() && (longarg.size() <= 2 || longarg[0] != '-' || longarg[1] != '-')) {
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

            m_args.emplace_back(shortarg, longarg, helpstr, booltype, required, false);

            return true;
        }

        argmap parse() {
            std::map<std::string, std::string> map;
            bool success = true;

            if (m_any_adds_failed) {
                success = false;
            } else {
                // Check if -h, --help was passed as only arg
                if (m_argc == 2 && (m_argv[1] == "-h" || m_argv[1] == "--help")) {
                    // TODO: Print help string
                }

                // Initialize all booltype args to false
                for (const auto& arg : m_args) {
                    if (arg.booltype) {
                        map[arg.shortarg] = "0";
                        map[arg.longarg]  = "0";
                    }
                }

                // Assign args
                for (int i = 0; i < m_argc; i++) {
                    for (auto& as : m_args) {
                        if (as.shortarg == m_argv[i] || as.longarg == m_argv[i]) {
                            if (as.booltype) {
                                map[as.shortarg] = "1";
                                map[as.longarg]  = "1";

                                as.parsed = true;
                            } else if (i+1 < m_argc) {
                                map[as.shortarg] = m_argv[i+1];
                                map[as.longarg]  = m_argv[i+1];

                                as.parsed = true;

                                i++;
                            }
                        }
                    }
                }
                map.erase("");

                // Check for required args
                for (const auto& as : m_args) {
                    if (as.required && !as.parsed) {
                        success = false;
                        break;
                    }
                }
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
