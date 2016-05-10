GCC = gcc -Wall -pedantic
GPP = g++ -Wall -pedantic
CLANG = clang -Wall -pedantic
CLANGPP = clang++ -Wall -pedantic

GCC_DEBUG = @$(GCC) -g
GPP_DEBUG = @$(GPP) -g
CLANG_DEBUG = @$(CLANG) -g
CLANGPP_DEBUG = @$(CLANGPP) -g

CMD = test.c argparser.c -o test && ./test -pf --help -v --integer 2 --double=2.2 -s="Hello"

all: test-gcc clean

test-all: test-gcc test-gpp test-clang test-clangpp clean

test-gcc:
	@echo "Testing valid input with gcc..."
	@$(GCC_DEBUG) $(CMD)
test-gpp:
	@echo "Testing valid input with g++..."
	@$(GPP_DEBUG) $(CMD)

test-clang:
	@echo "Testing valid input with clang..."
	@$(CLANG_DEBUG) $(CMD)

test-clangpp:
	@echo "Testing valid input with clang++..."
	@$(CLANGPP_DEBUG) $(CMD)

clean:
	@rm -rf test
