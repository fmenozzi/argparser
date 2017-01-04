Tested on Ubuntu 16.04 with `clang 3.8.0` and `gcc 5.4.0`.

Navigate to the project root and execute the following:

* `mkdir build`
* `cd build`
* `cmake -D ARGPARSER_BUILD_TESTS=ON ..`
* `make`
* `make test`

You may want to enable multicore builds in `make` with the `-j` option.
