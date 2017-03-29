**NOTE**: *The old C code is located in the `c/` directory. While it's still functional (as far as I can tell), it should be considered more-or-less deprecated for use in C++ programs.*

Simple command-line parser for C++11 programs. See `test/` for info on running test suite.

## Example

*example.cpp*:
```cpp
#include <iostream>
#include <string>

#include "argparser.hpp"

int main(int argc, char* argv[]) {
    ap::parser p(argc, argv);
    p.add("-f", "--firstname",  "First name",     ap::mode::REQUIRED);
    p.add("-l", "--lastname",   "Last name",      ap::mode::REQUIRED);
    p.add("-v", "--verbose",    "Verbose output", ap::mode::BOOLEAN);
    p.add("-s", "--salutation", "Salutation");    // Defaults to ap::mode::OPTIONAL
    
    auto args = p.parse();

    if (!args.parsed_successfully()) {
	std::cerr << "Unsuccessful parse\n";	
        return -1;
    }
   
    // Index into map with either shortarg or longarg 
    auto first = args["-f"];
    auto last  = args["--lastname"];

    // Optional args are empty if not specified
    auto salutation = (args["-s"].empty() ? "Hello" : args["-s"]);
    
    if (std::stoi(args["-v"])) {
        // Verbose output
        std::cout << salutation << ", " << first << " " << last << "!\n";
    } else {
        // Regular output
        std::cout << salutation << ", " << first << "\n";
    }
    
    return 0;
}
```

Compile:

	g++ example.cpp -o example -std=c++11
    
Run:

	$ ./example -h
	Usage: ./example [-h,--help] -f,--firstname -l,--lastname [-v,--verbose] [-s,--salutation] 
    
    Arguments:
        -h, --help          Show this help message and exit
        -f, --firstname     First name
        -l, --lastname      Last name
        -v, --verbose       Verbose output
        -s, --salutation    Salutation

	$ ./example -f John --lastname=Doe
	Hello, John

	$ ./example -f John --lastname=Doe -v
    Hello, John Doe!
    
	$ ./example -f John --lastname=Doe -v --salutation=Hey
	Hey, John Doe!
	
	$ ./example
	Unsuccessful parse
	
	$ ./example -f John
	Unsuccessful parse
	
	$ ./example -l Doe
	Unsuccessful parse

## Details
* `ap::parser::parse()` returns map-like object that provides read-only access to parsed arguments
* Args have one of three modes
	* `ap::mode::OPTIONAL`: Arg may or may not be populated (default mode)
		* Unused args are assigned the empty string
	* `ap::mode::REQUIRED`: Failure to populate arg results in unsuccessful parse
	* `ap::mode::BOOLEAN`: Used to track presence or absence of arg (i.e. does not populate value)
		* Value in resulting map is `"1"` if arg was passed and `"0"` otherwise
* Args can be assigned either with a space or an equals sign
	* In above example, `-f`, `--firstname` can be assigned values using any of the following schemes:
		* `./example -f John`
		* `./example -f=John`
		* `./example --firstname John`
		* `./example --firstname=John`
* Boolean flags can be chained together
	* e.g. if the Boolean shortargs `-a`, `-b`, `-c` are added, then any of the following invocations of the program are valid:
		* `./example -a -b -c`
		* `./example -abc`
		* `./example -ab -c`
* Rules for arg string formatting (failure to abide by rules results in unsuccessful parse):
	* Short arg must be a single dash `-` followed by a single character
		* Valid: `-a`
		* Invalid: `-`, `a`, `--a`, `-aa`
	* Long arg must be two dashes `--` followed by any number of additional characters
		* Valid: `--l`, `--longarg`
		* Invalid: `longarg`, `-longarg`, `-`, `--`
	* One of either short arg or long arg (but not both) may be the empty string
    * The help string is not optional (i.e. cannot use empty string)
    * Arg strings `-h`, `--help` are reserved
* A help string is automatically constructed (and corresponding help args `-h` and `--help` automatically provided)
	* Attempting to manually add arg strings `-h`, `--help` will result in a parse failure
	* If help args are passed, help string is displayed and the program is terminated
