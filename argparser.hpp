#pragma once

#include <vector>

#include <string>
#include <algorithm>
#include <map>

#include <cstdio>

namespace ap {
    // Behavior for individual args passed to add()
    enum class mode {
        REQUIRED,
        OPTIONAL,
        BOOLEAN,
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

        void print_help_string() {
            int help_len = std::string("-h, --help").size();
			int max_len = help_len;
			int rightpad = 4;

			const char* leftpadstr = "    ";

            // Print usage line
			fprintf(stdout, "Usage: %s [-h,--help] ", m_argv[0].c_str());
			for (const auto& as : m_args) {
				auto sa = as.shortarg;
                auto la = as.longarg;

                auto lbrak = as.required ? "" : "[";
                auto rbrak = as.required ? "" : "]";

				if (!sa.empty()) {
					if (!la.empty()) {
						fprintf(stdout, "%s%s,%s%s ", lbrak, sa.c_str(), la.c_str(), rbrak);
                    } else {
						fprintf(stdout, "%s%s%s ", lbrak, sa.c_str(), rbrak);
                    }
				} else {
					fprintf(stdout, "%s%s%s ", lbrak, la.c_str(), rbrak);
				}
			}
			fprintf(stdout, "\n\n");

            // Determine max len
            for (const auto& as : m_args) {
				int shortlen = as.shortarg.empty() ? 0 : as.shortarg.size();
				int longlen  = as.longarg.empty()  ? 0 : as.longarg.size();

				int arg_len = (shortlen && longlen) ? (shortlen + 2 + longlen) : (shortlen + longlen);
				if (arg_len > max_len)
					max_len = arg_len;
			}

			fprintf(stdout, "Arguments:\n");
			fprintf(stdout, "%s-h, --help", leftpadstr);
			for (int i = 0; i < (int)(max_len + rightpad - help_len); i++) {
				fprintf(stdout, " ");
            }
			fprintf(stdout, "Show this help message and exit\n");
            for (const auto& as : m_args) {
				auto sa = as.shortarg;
                auto la = as.longarg;

				if (!sa.empty()) {
					if (!la.empty()) {
						fprintf(stdout, "%s%s, %s", leftpadstr, sa.c_str(), la.c_str());
                    } else {
						fprintf(stdout, "%s%s", leftpadstr, sa.c_str());
                    }
				} else {
					fprintf(stdout, "%s%s", leftpadstr, la.c_str());
				}

				int shortlen = sa.empty() ? 0 : sa.size();
				int longlen = la.empty() ? 0 : la.size();
				int arg_len = (shortlen && longlen) ? (shortlen + 2 + longlen) : (shortlen + longlen);
				for (int j = 0; j < max_len + rightpad - arg_len; j++) {
					fprintf(stdout, " ");
				}

				fprintf(stdout, "%s\n", as.helpstr.c_str());
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
                 mode m=mode::OPTIONAL) {

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

            bool booltype = (m == mode::BOOLEAN);
            bool required = (m == mode::REQUIRED);

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
                    this->print_help_string();
                }

                // Initialize all booltype args to false and all other
                // args to the empty string
                for (const auto& arg : m_args) {
                    auto default_val = arg.booltype ? "0" : "";

                    map[arg.shortarg] = default_val;
                    map[arg.longarg]  = default_val;
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
