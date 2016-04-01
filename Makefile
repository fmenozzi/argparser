GCC = gcc -Wall -pedantic
GPP = g++ -Wall -pedantic
CLANG = clang -Wall -pedantic
CLANGPP = clang++ -Wall -pedantic

GCC_DEBUG = @$(GCC) -g
GPP_DEBUG = @$(GPP) -g
CLANG_DEBUG = @$(CLANG) -g
CLANGPP_DEBUG = @$(CLANGPP) -g

all: clean test

test: test-gcc

test-all: test-gcc test-gpp test-clang test-clangpp

test-gcc:
	@echo "Testing with gcc..."
	@$(GCC_DEBUG) test.c argparser.c -o test && ./test -p --help -v -i 2 --double=2.2 -s="Hello"

test-gpp:
	@echo "Testing with g++..."
	@$(GPP_DEBUG) test.c argparser.c -o test && ./test -p --help -v -i 2 --double=2.2 -s="Hello"

test-clang:
	@echo "Testing with clang..."
	@$(CLANG_DEBUG) test.c argparser.c -o test && ./test -p --help -v -i 2 --double=2.2 -s="Hello"

test-clangpp:
	@echo "Testing with clang++..."
	@$(CLANGPP_DEBUG) test.c argparser.c -o test && ./test -p --help -v -i 2 --double=2.2 -s="Hello"

clean:
	@rm -rf test
